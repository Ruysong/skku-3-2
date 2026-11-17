#include <array>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

static const uint32_t DATA_BASE = 0x10000000u;
static const uint32_t CONSOLE = 0x20000000u;
static const unsigned DATA_SIZE = 65536;

static uint32_t bits(uint32_t instruction, unsigned high, unsigned low) {
    const unsigned width = high - low + 1;
    const uint32_t mask = static_cast<uint32_t>((uint64_t{1} << width) - 1);
    return (instruction >> low) & mask;
}

static int32_t sign_extend(uint32_t value, unsigned width) {
    const uint32_t mask = static_cast<uint32_t>((uint64_t{1} << width) - 1);
    const uint32_t sign_bit = uint32_t{1} << (width - 1);
    int64_t result = static_cast<int64_t>(value & mask);
    if ((value & sign_bit) != 0) {
        result -= int64_t{1} << width;
    }
    return static_cast<int32_t>(result);
}

static int32_t store_offset(uint32_t instruction) {
    const uint32_t imm = (bits(instruction, 31, 25) << 5) |
                         bits(instruction, 11, 7);
    return sign_extend(imm, 12);
}

static int32_t branch_offset(uint32_t instruction) {
    const uint32_t imm = (bits(instruction, 31, 31) << 12) |
                         (bits(instruction, 7, 7) << 11) |
                         (bits(instruction, 30, 25) << 5) |
                         (bits(instruction, 11, 8) << 1);
    return sign_extend(imm, 13);
}

static int32_t jump_offset(uint32_t instruction) {
    const uint32_t imm = (bits(instruction, 31, 31) << 20) |
                         (bits(instruction, 19, 12) << 12) |
                         (bits(instruction, 20, 20) << 11) |
                         (bits(instruction, 30, 21) << 1);
    return sign_extend(imm, 21);
}

// Interpret a register value as a signed 32-bit integer.
static int64_t signed_value(uint32_t value) {
    if (value & 0x80000000u) {
        return static_cast<int64_t>(value) - (int64_t{1} << 32);
    }
    return value;
}

static uint32_t arithmetic_right(uint32_t value, unsigned amount) {
    if (amount == 0) return value;
    uint32_t result = value >> amount;
    if (value & 0x80000000u) {
        result |= 0xffffffffu << (32 - amount);
    }
    return result;
}

static bool read_binary(const std::string& path,
                        std::vector<unsigned char>& bytes) {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        std::cerr << "Cannot open input file: " << path << '\n';
        return false;
    }
    bytes.assign(std::istreambuf_iterator<char>(input),
                 std::istreambuf_iterator<char>());
    return !input.bad();
}

// Instruction and data words use little-endian byte order.
static uint32_t read_word(const unsigned char* bytes) {
    return static_cast<uint32_t>(bytes[0]) |
           (static_cast<uint32_t>(bytes[1]) << 8) |
           (static_cast<uint32_t>(bytes[2]) << 16) |
           (static_cast<uint32_t>(bytes[3]) << 24);
}

static bool data_address(uint32_t address) {
    return address >= DATA_BASE && address - DATA_BASE <= DATA_SIZE - 4;
}

static bool execute(uint32_t instruction, uint32_t& pc,
                    std::array<uint32_t, 32>& registers,
                    std::array<unsigned char, DATA_SIZE>& memory) {
    const uint32_t opcode = bits(instruction, 6, 0);
    const uint32_t rd = bits(instruction, 11, 7);
    const uint32_t funct3 = bits(instruction, 14, 12);
    const uint32_t rs1 = bits(instruction, 19, 15);
    const uint32_t rs2 = bits(instruction, 24, 20);
    const uint32_t funct7 = bits(instruction, 31, 25);
    const int32_t imm = sign_extend(bits(instruction, 31, 20), 12);
    // Read sources before writing rd, including when rd equals rs1 in jalr.
    const uint32_t a = registers[rs1];
    const uint32_t b = registers[rs2];
    uint32_t next_pc = pc + 4;
    uint32_t result = 0;
    bool write_register = true;

    switch (opcode) {
        case 0x33:
            switch (funct3) {
                case 0:
                    if (funct7 == 0) result = a + b;
                    else if (funct7 == 0x20) result = a - b;
                    else return false;
                    break;
                case 1:
                    if (funct7 != 0) return false;
                    result = a << (b & 31);
                    break;
                case 2:
                    if (funct7 != 0) return false;
                    result = signed_value(a) < signed_value(b);
                    break;
                case 4:
                    if (funct7 != 0) return false;
                    result = a ^ b;
                    break;
                case 5:
                    if (funct7 == 0) result = a >> (b & 31);
                    else if (funct7 == 0x20) result = arithmetic_right(a, b & 31);
                    else return false;
                    break;
                case 6:
                    if (funct7 != 0) return false;
                    result = a | b;
                    break;
                case 7:
                    if (funct7 != 0) return false;
                    result = a & b;
                    break;
                default: return false;
            }
            break;

        case 0x13:
            switch (funct3) {
                case 0: result = a + static_cast<uint32_t>(imm); break;
                case 2: result = signed_value(a) < imm; break;
                case 4: result = a ^ static_cast<uint32_t>(imm); break;
                case 6: result = a | static_cast<uint32_t>(imm); break;
                case 7: result = a & static_cast<uint32_t>(imm); break;
                case 1:
                    if (funct7 != 0) return false;
                    result = a << rs2;
                    break;
                case 5:
                    if (funct7 == 0) result = a >> rs2;
                    else if (funct7 == 0x20) result = arithmetic_right(a, rs2);
                    else return false;
                    break;
                default: return false;
            }
            break;

        case 0x37:
            result = instruction & 0xfffff000u;
            break;
        case 0x17:
            result = pc + (instruction & 0xfffff000u);
            break;
        case 0x6f:
            result = pc + 4;
            next_pc = pc + static_cast<uint32_t>(jump_offset(instruction));
            break;
        case 0x67:
            if (funct3 != 0) return false;
            result = pc + 4;
            next_pc = (a + static_cast<uint32_t>(imm)) & ~uint32_t{1};
            break;

        case 0x63: {
            bool taken;
            switch (funct3) {
                case 0: taken = a == b; break;
                case 1: taken = a != b; break;
                case 4: taken = signed_value(a) < signed_value(b); break;
                case 5: taken = signed_value(a) >= signed_value(b); break;
                default: return false;
            }
            if (taken) next_pc = pc + static_cast<uint32_t>(branch_offset(instruction));
            write_register = false;
            break;
        }

        case 0x03: {
            if (funct3 != 2) return false;
            const uint32_t address = a + static_cast<uint32_t>(imm);
            if (address == CONSOLE) {
                int64_t input;
                if (!(std::cin >> input)) return false;
                result = static_cast<uint32_t>(input);
            } else {
                if (!data_address(address)) return false;
                result = read_word(memory.data() + (address - DATA_BASE));
            }
            break;
        }

        case 0x23: {
            if (funct3 != 2) return false;
            const uint32_t address = a + static_cast<uint32_t>(store_offset(instruction));
            if (address == CONSOLE) {
                std::cout.put(static_cast<char>(b & 0xff));
            } else {
                if (!data_address(address)) return false;
                const uint32_t offset = address - DATA_BASE;
                for (unsigned i = 0; i < 4; ++i) {
                    memory[offset + i] = static_cast<unsigned char>(b >> (8 * i));
                }
            }
            write_register = false;
            break;
        }

        default: return false;
    }

    if (write_register && rd != 0) registers[rd] = result;
    pc = next_pc;
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 3 && argc != 4) {
        std::cerr << "Usage: " << argv[0] << " inst.bin [data.bin] N\n";
        return 1;
    }

    uint64_t limit;
    try {
        limit = std::stoull(argv[argc - 1]);
    } catch (...) {
        std::cerr << "Invalid instruction limit\n";
        return 1;
    }

    std::vector<unsigned char> instructions;
    if (!read_binary(argv[1], instructions)) return 1;
    std::array<uint32_t, 32> registers{};
    std::array<unsigned char, DATA_SIZE> memory;
    memory.fill(0xff);

    if (argc == 4) {
        std::vector<unsigned char> data;
        if (!read_binary(argv[2], data)) return 1;
        if (data.size() > memory.size()) {
            std::cerr << "Data file is too large\n";
            return 1;
        }
        for (std::size_t i = 0; i < data.size(); ++i) memory[i] = data[i];
    }

    uint32_t pc = 0;
    for (uint64_t count = 0; count < limit; ++count) {
        if (pc % 4 != 0 || uint64_t{pc} + 4 > instructions.size()) break;
        const uint32_t instruction = read_word(instructions.data() + pc);
        if (!execute(instruction, pc, registers, memory)) {
            std::cerr << "Cannot execute instruction at PC " << pc << '\n';
            return 1;
        }
    }

    for (unsigned i = 0; i < registers.size(); ++i) {
        std::cout << 'x' << std::dec << i << ": 0x"
                  << std::hex << std::nouppercase << std::setfill('0')
                  << std::setw(8) << registers[i] << '\n';
    }
    return 0;
}
