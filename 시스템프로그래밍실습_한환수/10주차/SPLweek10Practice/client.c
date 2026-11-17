
#include <stdio.h>
// printf(), snprintf(), fgets() 사용.

#include <stdint.h>
// uint32_t 사용.

#include <string.h>
// strlen(), strcmp(), strcspn() 사용.
// 문자열 길이 확인, 문자열 비교 등을 해야 하니까.

#include <unistd.h>
// read(), write(), close() 사용.

#include <fcntl.h>
// open(), O_RDONLY 사용.
// 클라이언트는 원본 파일을 읽어야 하니까.

#include <sys/socket.h>
// socket(), connect() 사용.

#include <netinet/in.h>
// sockaddr_in, AF_INET, htons() 등 사용.

#include <arpa/inet.h>
// inet_pton() 사용.
// "127.0.0.1"이라는 문자열 IP를 이진 주소로 변환해야 하니까.


// ===============================================
// 1. 메시지 종류
// ===============================================

typedef enum {
    SendFileName,  // 파일명 보낼게!
    Ack,           // 받았어!
    SendFileData,  // 파일 내용 보낼게!
    EndOfFile      // 이 파일 끝났어!
} Operation;

// 서버의 enum과 반드시 같은 순서여야 함.
// 서로 다른 번호로 해석하면 통신이 꼬이니까.


// ===============================================
// 2. 실제 전송할 메시지 구조체
// ===============================================

typedef struct {
    uint32_t operation;
    // 메시지 종류.

    char data[80];
    // 파일명 또는 실제 파일 데이터.

    uint32_t size;
    // data에 담긴 유효 바이트 수.
} Data;

// 서버와 클라이언트가 같은 구조체 형식을 쓰는 거임.
// 그래야 받은 바이트를 같은 의미로 해석할 수 있음.


// ===============================================
// 3. 끝까지 write하는 함수
// ===============================================

int write_full(int fd, const void* buf, size_t count) {

    const char* p = buf;
    // 보낼 데이터의 시작 주소.

    size_t done = 0;
    // 지금까지 보낸 바이트 수.

    while (done < count) {

        ssize_t n = write(fd, p + done, count - done);
        // 아직 안 보낸 부분부터 남은 바이트 수만큼 전송.

        if (n <= 0) return -1;

        done += (size_t)n;
        // 성공적으로 보낸 바이트 수를 누적.
    }

    return 0;
}


// ===============================================
// 4. 끝까지 read하는 함수
// ===============================================

int read_full(int fd, void* buf, size_t count) {

    char* p = buf;
    // 받을 데이터를 저장할 메모리 주소.

    size_t done = 0;

    while (done < count) {

        ssize_t n = read(fd, p + done, count - done);
        // 아직 덜 받은 데이터만큼 계속 읽는 거임.

        if (n <= 0) return -1;

        done += (size_t)n;
    }

    return 0;
}


// ===============================================
// 5. 클라이언트 main() 시작
// ===============================================

int main(void) {

    // -------------------------------------------
    // STEP 1. 소켓 생성
    // -------------------------------------------

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    // 자 일단 우리도 TCP 소켓을 만들어야겠지.
    //
    // IPv4 + TCP 소켓 생성.
    // 성공하면 fd가 반환됨.
    //
    // 이 fd를 사용해서 서버와 통신할 거임.


    // -------------------------------------------
    // STEP 2. 연결할 서버 주소 설정
    // -------------------------------------------

    struct sockaddr_in addr = { 0 };

    addr.sin_family = AF_INET;
    // IPv4 사용할 거임.

    addr.sin_port = htons(1398);
    // 서버가 기다리고 있는 포트 번호 1398.

    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    // 우리가 찾아갈 서버 IP는 127.0.0.1.
    //
    // 이건 내 컴퓨터 자신을 가리키는 루프백 주소.
    // 같은 Ubuntu 환경에서 서버를 실행할 거니까.
    //
    // inet_pton()은 문자열 IP를 이진 주소로 변환하고
    // 그 결과를 addr.sin_addr에 기록해 줌.


    // -------------------------------------------
    // STEP 3. connect()
    // -------------------------------------------

    if (connect(client_fd,
        (struct sockaddr*)&addr,
        sizeof(addr)) < 0) {

        printf("Connection failed\n");
        close(client_fd);
        return 1;
    }

    // 아하 이제 서버에게 연결 요청을 하는 거구나.
    //
    // 서버는 accept()에서 기다리고 있었겠지.
    // TCP 연결이 성립하면 connect()가 성공하고
    // 서버 쪽 accept()도 연결된 소켓을 반환할 수 있음.
    //
    // 단, 아직 파일명을 보내지는 않았어.
    // connect()는 연결을 만드는 함수일 뿐이니까!

    printf("Connected to server!\n");


    // -------------------------------------------
    // STEP 4. 파일명을 반복해서 입력받기
    // -------------------------------------------

    while (1) {
        // 이제 파일을 여러 개 보낼 거임.
        // quit를 입력할 때까지 계속 반복.

        char filename[80];
        // 사용자가 입력할 파일명을 저장할 배열.

        printf("File name (quit to exit): ");

        if (fgets(filename, sizeof(filename), stdin) == NULL) {
            break;
        }

        // fgets()는 키보드에서 문자열 한 줄을 읽어 옴
        // fgets()는 Enter로 입력된 '\n'도 저장할 수 있음.


        filename[strcspn(filename, "\n")] = '\0';
        // 아하 Enter 때문에 생긴 '\n'을 없애는 거임.
        //
        // strcspn()으로 '\n' 위치를 찾고
        // 거기에 '\0'을 넣어서 문자열을 끝내는 것.
        //
        // 예: "Aladdin\n" → "Aladdin"


        if (strcmp(filename, "quit") == 0) {
            break;
        }

        // strcmp()는 문자열 두 개를 비교하는 함수.
        // 두 문자열이 같으면 0을 반환함.
        //
        // 따라서 quit가 입력되면 바깥쪽 while 종료!


        if (strlen(filename) == 0 || strlen(filename) >= 50) {
            printf("Invalid file name\n");
            continue;
        }

        // strlen()은 문자열 길이 확인.
        // 파일명이 비어 있거나 너무 길면 다시 입력받음.
        //
        // continue는 이번 반복의 나머지를 건너뛰고
        // 다음 반복으로 넘어가는 명령어.


        // ---------------------------------------
        // STEP 5. 원본 파일 열기
        // ---------------------------------------

        int file_fd = open(filename, O_RDONLY);
        // 아하 여기서 실제 원본 파일을 여는구나!
        //
        // filename = "Aladdin"이라면
        // open("Aladdin", O_RDONLY)인 셈.
        //
        // O_RDONLY는 읽기 전용.
        // 우리는 파일을 수정하는 게 아니라 보내는 거니까.
        //
        // 성공하면 새로운 fd가 배정됨.
        //
        // client_fd : 네트워크 소켓 fd
        // file_fd   : 원본 파일 fd

        if (file_fd < 0) {
            printf("Cannot open file\n");
            continue;
        }

        // 파일이 없거나 열 수 없다면
        // 서버에 파일명을 보내기 전에 다시 입력받음.


        // ---------------------------------------
        // STEP 6. 파일명 메시지 전송
        // ---------------------------------------

        Data msg = { 0 };
        // 보낼 메시지 구조체 하나 생성.

        msg.operation = SendFileName;
        // 이 메시지는 파일명이라는 뜻!

        snprintf(msg.data, sizeof(msg.data), "%s", filename);
        // 사용자가 입력한 파일명을
        // Data 구조체의 data 배열에 넣어 주는 거임.

        msg.size = (uint32_t)strlen(filename) + 1;
        // 파일명 문자열의 실제 길이 + '\0' 1바이트.
        //
        // 예: "Aladdin"은 7글자니까 size = 8.
        //
        // uint32_t로 변환하는 건
        // 구조체의 size 멤버 자료형에 맞추기 위함.


        if (write_full(client_fd, &msg, sizeof(msg)) < 0) {
            close(file_fd);
            break;
        }

        // 아하 이제 서버에 파일명을 보냈음!
        //
        // client_fd : 서버로 연결된 소켓
        // &msg      : 전송할 구조체의 메모리 주소
        // sizeof(msg): 구조체 전체 크기
        //
        // 서버는 read_full()에서 이 메시지를 받겠지.


        // ---------------------------------------
        // STEP 7. 서버의 ACK 기다리기
        // ---------------------------------------

        Data ack = { 0 };

        if (read_full(client_fd, &ack, sizeof(ack)) < 0 ||
            ack.operation != Ack) {

            close(file_fd);
            break;
        }

        // 아하 서버가 파일명 메시지를 받았다고
        // ACK를 보내 줄 때까지 기다리는 거임.
        //
        // read_full()이 데이터를 다 받으면 반환하고
        // operation을 확인해서 ACK가 맞는지 검사.
        //
        // ||는 논리 OR.
        // 둘 중 하나라도 참이면 if문 실행.

        printf("ACK received!\n");


        // ---------------------------------------
        // STEP 8. 실제 파일 내용 전송하기
        // ---------------------------------------

        int failed = 0;
        // 파일을 읽거나 보내는 중 실패했는지 표시.
        // 처음에는 정상 상태니까 0.

        while (1) {
            // 파일 하나를 80바이트씩 나눠 보내기 위한 반복문.

            Data file_msg = { 0 };
            // 파일 데이터 한 조각을 담을 메시지.

            ssize_t n = read(file_fd,
                file_msg.data,
                sizeof(file_msg.data));

            // 자 이제 진짜 원본 파일을 읽는 거구나!
            //
            // file_fd : 원본 파일
            // file_msg.data : 읽은 내용을 담을 배열
            // sizeof(file_msg.data) : 최대 80바이트
            //
            // n에는 실제로 읽은 바이트 수가 들어옴.


            if (n == 0) break;
            // read()가 0을 반환했다?
            // 아하 원본 파일의 끝에 도달했구나!
            // 그럼 더 읽을 게 없으니까 안쪽 while 종료.


            if (n < 0) {
                failed = 1;
                break;
            }

            // 음수가 나오면 파일 읽기 오류.
            // 실패했다고 표시하고 안쪽 반복문 종료.


            file_msg.operation = SendFileData;
            // 이번 메시지는 파일 내용이라는 뜻.

            file_msg.size = (uint32_t)n;
            // 실제 읽은 바이트 수 기록.
            //
            // 예: 80바이트면 80.
            // 마지막에 30바이트만 읽혔다면 30.


            if (write_full(client_fd,
                &file_msg,
                sizeof(file_msg)) < 0) {

                failed = 1;
                break;
            }

            // 이제 파일 내용 한 조각을 서버로 보내는 거임.
            //
            // 아까 read(file_fd, ...)로 파일에서 읽고
            // 지금 write_full(client_fd, ...)로 소켓에 보냄.
            //
            // 이게 이번 과제에서 가장 중요한 흐름!
        }


        // ---------------------------------------
        // STEP 9. 원본 파일 닫기
        // ---------------------------------------

        close(file_fd);
        // 파일 내용을 다 읽었으니까 원본 파일 fd는 닫기.
        //
        // 하지만 client_fd는 닫으면 안 됨!
        // 서버에게 EOF도 보내야 하고
        // 다음 파일도 같은 TCP 연결로 보낼 거니까.

        if (failed) break;
        // 읽기/전송 실패가 있었다면
        // 더 이상 진행하지 않고 바깥쪽 while 종료.


        // ---------------------------------------
        // STEP 10. EndOfFile 메시지 전송
        // ---------------------------------------

        Data eof_msg = { 0 };

        eof_msg.operation = EndOfFile;
        // 아 이 파일은 이제 끝났어요!
        // 라는 메시지 종류로 지정.

        if (write_full(client_fd,
            &eof_msg,
            sizeof(eof_msg)) < 0) {
            break;
        }

        // 서버는 EndOfFile을 받으면
        // 현재 저장 중인 파일을 닫고
        // 다음 파일명을 기다리겠지.

        printf("File sent!\n");

        // 바깥쪽 while의 처음으로 돌아가서
        // 다음 파일명을 입력받음.
    }


    // -------------------------------------------
    // STEP 11. 클라이언트 종료
    // -------------------------------------------

    close(client_fd);
    // 사용자가 quit를 입력했거나
    // 통신이 실패해서 반복문이 끝났을 때
    // 마지막으로 TCP 연결 소켓 닫기.

    return 0;
}
