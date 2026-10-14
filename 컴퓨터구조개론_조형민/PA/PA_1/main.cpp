#include <array>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

// Extract the inclusive range [high:low] from a 32-bit instruction.
static uint32_t bits(uint32_t instruction, unsigned high, unsigned low) {
    const unsigned width = high - low + 1;
    const uint32_t mask = static_cast<uint32_t>((uint64_t{1} << width) - 1);
    return (instruction >> low) & mask;
}

// Interpret a value as a signed two's-complement number of 'width' bits.
static int32_t sign_extend(uint32_t value, unsigned width) {
    const uint32_t mask = static_cast<uint32_t>((uint64_t{1} << width) - 1);
    const uint32_t sign_bit = uint32_t{1} << (width - 1);
    int64_t result = static_cast<int64_t>(value & mask);
    if ((value & sign_bit) != 0) {
        result -= int64_t{1} << width;
    }
    return static_cast<int32_t>(result);
}

static std::string reg(uint32_t number) {
    return "x" + std::to_string(number);
}

static std::string three_regs(const std::string& name, uint32_t rd,
                              uint32_t rs1, uint32_t rs2) {
    return name + " " + reg(rd) + ", " + reg(rs1) + ", " + reg(rs2);
}

static std::string reg_reg_imm(const std::string& name, uint32_t rd,
                               uint32_t rs1, int32_t imm) {
    return name + " " + reg(rd) + ", " + reg(rs1) + ", " + std::to_string(imm);
}

static std::string reg_offset(const std::string& name, uint32_t first,
                              uint32_t base, int32_t offset) {
    return name + " " + reg(first) + ", " + std::to_string(offset) +
           "(" + reg(base) + ")";
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

// Convert the U-format immediate (already shifted left by 12) to a
// signed decimal value without relying on unsigned-to-signed wraparound.
static int64_t upper_immediate(uint32_t instruction) {
    const uint32_t value = instruction & 0xfffff000u;
    int64_t result = static_cast<int64_t>(value);
    if ((value & 0x80000000u) != 0) {
        result -= int64_t{1} << 32;
    }
    return result;
}

static std::string decode(uint32_t instruction) {
    const uint32_t opcode = bits(instruction, 6, 0);
    const uint32_t rd = bits(instruction, 11, 7);
    const uint32_t funct3 = bits(instruction, 14, 12);
    const uint32_t rs1 = bits(instruction, 19, 15);
    const uint32_t rs2 = bits(instruction, 24, 20);
    const uint32_t funct7 = bits(instruction, 31, 25);

    switch (opcode) {
        case 0x33: {  // Register-register arithmetic (R-format)
            std::string name;
            switch (funct3) {
                case 0x0:
                    if (funct7 == 0x00) name = "add";
                    else if (funct7 == 0x20) name = "sub";
                    break;
                case 0x1: if (funct7 == 0x00) name = "sll"; break;
                case 0x2: if (funct7 == 0x00) name = "slt"; break;
                case 0x3: if (funct7 == 0x00) name = "sltu"; break;
                case 0x4: if (funct7 == 0x00) name = "xor"; break;
                case 0x5:
                    if (funct7 == 0x00) name = "srl";
                    else if (funct7 == 0x20) name = "sra";
                    break;
                case 0x6: if (funct7 == 0x00) name = "or"; break;
                case 0x7: if (funct7 == 0x00) name = "and"; break;
                default: break;
            }
            return name.empty() ? "unknown instruction" :
                                  three_regs(name, rd, rs1, rs2);
        }

        case 0x13: {  // Immediate arithmetic and shifts (I-format)
            std::string name;
            const int32_t imm = sign_extend(bits(instruction, 31, 20), 12);
            switch (funct3) {
                case 0x0: name = "addi"; break;
                case 0x2: name = "slti"; break;
                case 0x3: name = "sltiu"; break;
                case 0x4: name = "xori"; break;
                case 0x6: name = "ori"; break;
                case 0x7: name = "andi"; break;
                case 0x1:
                    if (funct7 == 0x00) name = "slli";
                    break;
                case 0x5:
                    if (funct7 == 0x00) name = "srli";
                    else if (funct7 == 0x20) name = "srai";
                    break;
                default: break;
            }
            if (name.empty()) return "unknown instruction";
            if (funct3 == 0x1 || funct3 == 0x5) {
                return reg_reg_imm(name, rd, rs1,
                                   static_cast<int32_t>(bits(instruction, 24, 20)));
            }
            return reg_reg_imm(name, rd, rs1, imm);
        }

        case 0x03: {  // Loads (I-format)
            std::string name;
            switch (funct3) {
                case 0x0: name = "lb"; break;
                case 0x1: name = "lh"; break;
                case 0x2: name = "lw"; break;
                case 0x4: name = "lbu"; break;
                case 0x5: name = "lhu"; break;
                default: return "unknown instruction";
            }
            return reg_offset(name, rd, rs1,
                              sign_extend(bits(instruction, 31, 20), 12));
        }

        case 0x23: {  // Stores (S-format)
            std::string name;
            switch (funct3) {
                case 0x0: name = "sb"; break;
                case 0x1: name = "sh"; break;
                case 0x2: name = "sw"; break;
                default: return "unknown instruction";
            }
            return reg_offset(name, rs2, rs1, store_offset(instruction));
        }

        case 0x63: {  // Conditional branches (SB-format)
            std::string name;
            switch (funct3) {
                case 0x0: name = "beq"; break;
                case 0x1: name = "bne"; break;
                case 0x4: name = "blt"; break;
                case 0x5: name = "bge"; break;
                case 0x6: name = "bltu"; break;
                case 0x7: name = "bgeu"; break;
                default: return "unknown instruction";
            }
            return name + " " + reg(rs1) + ", " + reg(rs2) + ", " +
                   std::to_string(branch_offset(instruction));
        }

        case 0x6f:  // Jump and link (UJ-format)
            return "jal " + reg(rd) + ", " + std::to_string(jump_offset(instruction));

        case 0x67:  // Jump and link register (I-format)
            if (funct3 != 0x0) return "unknown instruction";
            return reg_offset("jalr", rd, rs1,
                              sign_extend(bits(instruction, 31, 20), 12));

        case 0x37:  // Load upper immediate (U-format)
            return "lui " + reg(rd) + ", " +
                   std::to_string(upper_immediate(instruction));

        case 0x17:  // Add upper immediate to PC (U-format)
            return "auipc " + reg(rd) + ", " +
                   std::to_string(upper_immediate(instruction));

        default:
            return "unknown instruction";
    }
}

static bool disassemble_file(const std::string& file_name) {
    std::ifstream input(file_name, std::ios::binary);
    if (!input) {
        std::cerr << "Cannot open input file: " << file_name << '\n';
        return false;
    }

    std::array<unsigned char, 4> bytes{};
    uint64_t index = 0;
    while (true) {
        input.read(reinterpret_cast<char*>(bytes.data()), bytes.size());
        const std::streamsize count = input.gcount();
        if (count == 0) {
            if (input.bad()) {
                std::cerr << "Error while reading input file\n";
                return false;
            }
            break;
        }
        if (count != 4) {
            std::cerr << "Input file contains an incomplete 32-bit instruction\n";
            return false;
        }

        const uint32_t instruction =
            static_cast<uint32_t>(bytes[0]) |
            (static_cast<uint32_t>(bytes[1]) << 8) |
            (static_cast<uint32_t>(bytes[2]) << 16) |
            (static_cast<uint32_t>(bytes[3]) << 24);

        std::cout << "inst " << index << ": "
                  << std::hex << std::nouppercase << std::setfill('0')
                  << std::setw(8) << instruction << std::dec << ' '
                  << decode(instruction) << '\n';
        ++index;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <binary-file>\n";
        return 1;
    }
    return disassemble_file(argv[1]) ? 0 : 1;
}
