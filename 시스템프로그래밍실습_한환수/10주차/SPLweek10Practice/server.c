
#include <stdio.h>
// printf(), snprintf() 등을 사용하려고.
// 화면에 출력하거나 문자열을 만들어야 하니까.
#include <stdint.h>
// uint32_t 사용하려고.
// 정확히 32비트인 부호 없는 정수 자료형임.
#include <string.h>
// memchr() 사용하려고.
// 메모리 안에서 특정 바이트를 찾는 함수임.
#include <unistd.h>
// read(), write(), close() 사용하려고.
// 소켓이든 파일이든 fd로 읽고 쓰고 닫을 수 있음.
#include <fcntl.h>
// open(), O_WRONLY, O_CREAT, O_TRUNC 등을 사용하려고.
// 서버에서 새로운 복사 파일을 생성해야 하니까.
#include <sys/socket.h>
// socket(), bind(), listen(), accept() 사용하려고.
// 소켓 프로그래밍 핵심 헤더!
#include <netinet/in.h>
// sockaddr_in, AF_INET, INADDR_ANY, htons() 등을 사용하려고.
// IPv4 주소와 포트를 설정하기 위한 것임.

// ===============================================
// 1. 우리가 주고받을 메시지의 종류
// ===============================================

typedef enum {
    SendFileName,  // 0: 파일명을 보낼게요!
    Ack,           // 1: 파일명 받았어요!
    SendFileData,  // 2: 실제 파일 내용을 보낼게요!
    EndOfFile      // 3: 파일 전송 끝났어요!
} Operation;

// enum은 그냥 메시지 종류에 이름 붙여 놓은 거임.
// 서버가 메시지를 받았을 때
// "아 이거 파일명이구나", "아 이건 파일 데이터구나"
// 이렇게 해석할 수 있게 하는 것.
// ===============================================
// 2. 실제 메시지 하나를 담을 구조체
// ===============================================

typedef struct {
    uint32_t operation;
    // 지금 이 메시지가 무슨 종류인지.
    // 위에서 정의한 Operation 값이 들어감.

    char data[80];
    // 실제 파일명 또는 파일 데이터가 들어가는 공간.
    // 한 메시지당 최대 80바이트를 담을 수 있음.

    uint32_t size;
    // data[80] 중에서 실제로 유효한 바이트가 몇 개인지.
    // 파일 데이터가 30바이트면 size = 30.
} Data;


// ===============================================
// 3. 요청한 바이트를 끝까지 write하는 함수
// ===============================================

int write_full(int fd, const void* buf, size_t count) {

    const char* p = buf;
    // buf는 원본 데이터의 시작 주소임.
    // void*는 자료형 정보가 없는 포인터라서
    // 바이트 단위로 움직이기 편하게 char*로 받는 거임.
    // const는 이 포인터로 원본 데이터를 수정하지 않겠다는 뜻.

    size_t done = 0;
    // 지금까지 몇 바이트를 썼는지 기록하는 변수.

    while (done < count) {
        // 아직 다 못 썼으면 계속 반복!

        ssize_t n = write(fd, p + done, count - done);
        // fd       : 어디에 쓸 거야?
        // p + done : 아직 안 쓴 데이터는 어디부터야?
        // count-done : 앞으로 몇 바이트 더 써야 해?
        //
        // write()는 요청한 바이트를 한 번에 다 처리하지
        // 못할 수도 있어서 이렇게 반복하는 거임.

        if (n <= 0) return -1;
        // 쓰기에 실패했거나 진행할 수 없다면 실패로 처리.
        // 이번에는 복잡한 오류 처리는 생략.

        done += (size_t)n;
        // 실제로 쓴 바이트만큼 done 증가.
    }

    return 0;
    // 요청한 바이트를 모두 썼다는 뜻.
}


// ===============================================
// 4. 요청한 바이트를 끝까지 read하는 함수
// ===============================================

int read_full(int fd, void* buf, size_t count) {

    char* p = buf;
    // 이번에는 데이터를 받아서 버퍼에 써 넣어야 하니까
    // const가 없는 char*를 사용하는 거임.

    size_t done = 0;
    // 지금까지 읽어 온 바이트 수.

    while (done < count) {

        ssize_t n = read(fd, p + done, count - done);
        // fd       : 어느 소켓/파일에서 읽어?
        // p + done : 읽은 데이터를 어디부터 저장해?
        // count-done : 아직 몇 바이트가 부족해?

        if (n <= 0) return -1;
        // 0이면 상대가 송신을 종료한 것.
        // -1이면 오류.
        // 지금은 둘 다 실패로 처리.

        done += (size_t)n;
        // 읽은 바이트 수를 누적.
    }

    return 0;
    // 필요한 바이트를 전부 읽었음.
}


// ===============================================
// 5. 서버의 main() 시작
// ===============================================

int main(void) {

    // -------------------------------------------
    // STEP 1. 서버 소켓 생성
    // -------------------------------------------

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // 아하 일단 통신할 소켓을 생성하는 거구나.
    //
    // AF_INET     : IPv4 사용
    // SOCK_STREAM : TCP 바이트 스트림 방식
    // 0           : 기본 프로토콜 선택
    //
    // 성공하면 fd가 반환됨.
    // 이 fd는 서버가 연결 요청을 받는 데 사용할 거임.


    // -------------------------------------------
    // STEP 2. 서버 주소 설정
    // -------------------------------------------

    struct sockaddr_in addr = { 0 };
    // IP 주소와 포트를 담는 구조체 생성.
    // {0}으로 전체를 0으로 초기화.

    addr.sin_family = AF_INET;
    // IPv4 주소 체계를 사용하겠음.

    addr.sin_port = htons(1398);
    // 1398번 포트에서 기다리겠음!
    //
    // htons = Host To Network Short
    // 16비트 포트 번호를 네트워크 바이트 순서로 변환.

    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 내 컴퓨터의 모든 로컬 IPv4 인터페이스에서 연결받겠음.
    //
    // INADDR_ANY는 특정 상대방의 IP가 아님.
    // 서버가 어떤 로컬 주소에서 연결을 받을지 정하는 것.
    //
    // htonl = Host To Network Long
    // 32비트 값을 네트워크 바이트 순서로 변환.


    // -------------------------------------------
    // STEP 3. bind()
    // -------------------------------------------

    if (bind(server_fd,
        (struct sockaddr*)&addr,
        sizeof(addr)) < 0) {

        printf("Bind failed\n");
        close(server_fd);
        return 1;
    }

    // bind()는
    // "이 server_fd 소켓을 1398번 포트에 연결해 줘!"
    // 라고 운영체제에 요청하는 것.
    //
    // &addr는 주소 구조체 변수의 메모리 주소.
    // (struct sockaddr*)는 범용 소켓 주소 포인터로 형변환.
    // sizeof(addr)는 주소 구조체의 크기.
    //
    // 성공하면 이제 해당 포트가 소켓에 바인딩된 상태.


    // -------------------------------------------
    // STEP 4. listen()
    // -------------------------------------------

    if (listen(server_fd, 5) < 0) {
        close(server_fd);
        return 1;
    }

    // 아 이제 연결 요청을 받아들일 준비를 하는 거구나.
    //
    // 5는 backlog.
    // 연결 수락을 기다리는 대기열 크기와 관련된 값임.
    // 동시에 통신할 수 있는 클라이언트가
    // 정확히 5명이라는 뜻은 아님.

    printf("Server listening on port 1398...\n");


    // -------------------------------------------
    // STEP 5. accept()
    // -------------------------------------------

    int conn_fd = accept(server_fd, NULL, NULL);
    // 자 이제 누가 connect() 해주길 기다리는 거임!
    //
    // 클라이언트와 연결이 성립하면
    // 실제 통신에 사용할 새로운 fd를 반환받음.
    //
    // server_fd : 연결 요청을 받아들이는 소켓
    // conn_fd   : 연결된 클라이언트와 통신하는 소켓
    //
    // NULL, NULL은 상대방 주소를 따로 받지 않겠다는 뜻.

    if (conn_fd < 0) {
        close(server_fd);
        return 1;
    }


    // -------------------------------------------
    // STEP 6. 여러 파일을 계속 받기
    // -------------------------------------------

    while (1) {
        // 이 바깥쪽 while은 파일 하나가 아니라
        // 파일 여러 개를 계속 받기 위한 반복문임.
        //
        // 클라이언트가 연결을 끊으면 종료할 거임.


        // ---------------------------------------
        // STEP 6-1. 파일명 메시지 받기
        // ---------------------------------------

        Data received = { 0 };
        // 메시지 한 개를 받을 구조체 생성.
        // 지금은 아직 전부 0으로 초기화된 상태.

        if (read_full(conn_fd,
            &received,
            sizeof(received)) < 0) {
            break;
        }

        // 아하 여기서 서버가 기다리는 거구나.
        //
        // 클라이언트가 SendFileName을 보내면
        // 이 구조체 안으로 바이트들이 들어오겠지.
        //
        // &received는 구조체 시작 주소.
        // sizeof(received)는 구조체 전체 크기.


        if (received.operation != SendFileName) {
            break;
        }

        // 지금은 파일명을 받아야 하는 단계인데
        // 다른 종류의 메시지가 왔으면 중단.
        //
        // 예를 들어 SendFileData가 먼저 오면
        // 통신 순서가 잘못된 것이니까.


        // ---------------------------------------
        // STEP 6-2. 받은 파일명 확인
        // ---------------------------------------

        char* end = memchr(received.data,
            '\0',
            sizeof(received.data));

        // memchr()은 메모리에서 특정 바이트를 찾아줌.
        //
        // received.data 안에서 문자열 끝 문자 '\0'를 찾음.
        // 왜? 서버가 안전하게 파일명을 문자열로 쓰려면
        // 어디서 문자열이 끝나는지 확인해야 하니까.
        //
        // 못 찾으면 NULL 반환.

        if (end == NULL) break;

        size_t name_len = (size_t)(end - received.data);
        // end는 '\0'의 주소.
        // received.data는 문자열 시작 주소.
        //
        // 둘을 빼면 문자열의 길이가 나옴.
        //
        // 예: "Aladdin"
        // 시작 주소가 100이고 '\0' 위치가 107이면
        // 107 - 100 = 7글자.

        if (name_len == 0 || name_len >= 50 ||
            received.size != name_len + 1) {
            break;
        }

        // 비어 있는 파일명이나 너무 긴 파일명은 거절.
        // size에는 마지막 '\0'까지 포함돼야 하니까
        // name_len + 1인지 검사하는 거임.

        printf("File name: %s\n", received.data);


        // ---------------------------------------
        // STEP 7. _copy 파일명 만들기
        // ---------------------------------------

        char copy_name[100];
        // 저장할 파일명을 담을 문자 배열.

        snprintf(copy_name,
            sizeof(copy_name),
            "%s_copy",
            received.data);

        // 예를 들어 received.data = "Aladdin"이면
        //
        // copy_name = "Aladdin_copy"
        //
        // snprintf()는 문자열을 버퍼에 기록하는 함수.
        // 버퍼 크기를 넘지 않게 해 줌.


        // ---------------------------------------
        // STEP 8. 실제 복사 파일 생성
        // ---------------------------------------

        int file_fd = open(copy_name,
            O_WRONLY | O_CREAT | O_TRUNC,
            0644);

        // 아하 여기서 새로운 fd가 하나 더 생기는 거구나!
        //
        // conn_fd : TCP 소켓 fd
        // file_fd : Aladdin_copy 파일 fd
        //
        // O_WRONLY : 파일에 쓰기
        // O_CREAT  : 없으면 새로 생성
        // O_TRUNC  : 기존에 있으면 내용 비우기
        //
        // 0644는 파일 생성 권한을 나타내는 8진수.
        // 소유자는 읽기/쓰기, 나머지는 읽기 가능.
        // 실제 권한은 umask의 영향을 받을 수 있음.

        if (file_fd < 0) break;


        // ---------------------------------------
        // STEP 9. 클라이언트에게 ACK 보내기
        // ---------------------------------------

        Data ack = { 0 };

        ack.operation = Ack;
        // 이 메시지는 ACK라고 종류를 표시함.
        // Ack는 enum에서 1에 해당함.

        if (write_full(conn_fd, &ack, sizeof(ack)) < 0) {
            close(file_fd);
            break;
        }

        // 아하 파일 생성 준비가 됐으니까
        // 이제 클라이언트에게 "파일명 받았어요!"
        // 하고 ACK를 보내 주는 거구나.
        //
        // 여기서는 file_fd가 아니라 conn_fd!
        // 네트워크로 전송하는 것이니까.


        // ---------------------------------------
        // STEP 10. 파일 내용을 반복 수신
        // ---------------------------------------

        int completed = 0;
        // 파일을 끝까지 받았는지 표시하는 변수.
        // 아직 EOF를 못 받았으므로 처음에는 0.

        while (1) {
            // 이 안쪽 while은 파일 하나의 내용을
            // 여러 조각으로 나눠 받기 위한 반복문임.

            Data msg = { 0 };

            if (read_full(conn_fd, &msg, sizeof(msg)) < 0) {
                break;
            }

            // 클라이언트가 보낸 Data 메시지 하나를 받음.
            // 한 번에 파일 내용이 최대 80바이트씩 들어 있겠지.


            if (msg.operation == EndOfFile) {
                completed = 1;
                break;
            }

            // 아 파일 하나의 전송이 끝났구나!
            // 이제 안쪽 while만 빠져나오면 됨.
            // 바깥쪽 while은 아직 살아 있어서
            // 다음 파일명을 또 받을 수 있음.


            if (msg.operation != SendFileData ||
                msg.size > sizeof(msg.data)) {
                break;
            }

            // 파일 데이터가 아닌 이상한 메시지가 오거나
            // 데이터 크기가 80바이트를 넘으면 중단.


            if (write_full(file_fd, msg.data, msg.size) < 0) {
                break;
            }

            // 이번 코드의 진짜 핵심!
            //
            // conn_fd로 받은 데이터를
            // file_fd가 가리키는 실제 파일에 기록하는 거임.
            //
            // msg.data : 받은 파일 내용
            // msg.size : 실제 받은 유효 바이트 수
            //
            // 예를 들어 30바이트만 들어 있다면
            // 정확히 30바이트만 파일에 기록함.
        }


        // ---------------------------------------
        // STEP 11. 파일 하나 저장 완료
        // ---------------------------------------

        close(file_fd);
        // 이 파일은 다 받았으니까 파일 fd 닫기.
        // TCP 소켓은 아직 닫지 않음!

        if (!completed) break;
        // EOF를 정상적으로 못 받았다면 종료.
        // !completed는 completed == 0이라는 뜻.

        printf("File saved: %s\n", copy_name);

        // 이제 바깥쪽 while의 처음으로 돌아감.
        // 클라이언트가 다음 파일명을 보내면
        // 다시 파일을 생성해서 받을 거임.
    }


    // -------------------------------------------
    // STEP 12. 연결 정리
    // -------------------------------------------

    close(conn_fd);
    // 클라이언트와 통신하던 연결 소켓 닫기.

    close(server_fd);
    // 연결 요청을 받던 리스닝 소켓도 닫기.

    return 0;
}
