
#include <stdio.h>     // perror(), fprintf(), stderr
#include <stdlib.h>    // malloc(), realloc(), free(), strtol()
#include <unistd.h>    // read(), write(), close(), getopt()
#include <fcntl.h>     // open(), O_RDONLY
#include <string.h>    // strcmp(), memcpy()
#include <errno.h>     // errno, ERANGE
#include <limits.h>    // INT_MAX
#include <stdint.h>    // SIZE_MAX

/*
  * 1. read()로 파일을 끝까지 읽는다.
 * 2. 읽은 데이터를 동적 메모리(data)에 계속 추가한다.
 * 3. 파일 끝에서 거꾸로 '\n'을 센다.
 * 4. 마지막 num줄의 시작 위치를 찾는다.
 * 5. 해당 부분만 write()로 출력한다.
 * 6. 사용한 동적 메모리를 free()한다.
 */

int print_tail(int fd, int num) {

    // [1] 한 번의 read()에서 사용할 임시 버퍼
    // head에서 사용했던 buf[4096]과 동일하다.
    char buf[4096];
    /*
     * [2] 파일 전체를 저장할 동적 메모리
 
    * 예:
     * capacity = 8
     * size = 5

     * data
     *  |
     *  v
     * [A][B][C][D][E][ ][ ][ ]
     *              사용 5 / 전체 8
     * size_t:
     *   메모리 크기나 배열 인덱스를 나타내는
     *   부호 없는 정수 자료형이다.
     */
    char *data = NULL;
    size_t size = 0;
    size_t capacity = 0;

    // read()가 실제로 읽은 바이트 수를 저장
    // ssize_t는 음수(-1)도 표현할 수 있는 자료형
    ssize_t n;

    /*
     * [3] read()로 데이터를 반복해서 가져오기
 
     */
    while ((n = read(fd, buf, sizeof(buf))) > 0) {

        /*
         * [4] 새 데이터를 저장할 공간이 얼마나 필요한가?
         *
         * 예:
         * 현재 size = 6000
         * 새로 읽은 n = 4096
         * needed = 10096
         * SIZE_MAX:
         *   size_t 자료형으로 표현 가능한 최댓값
         *
         * size + n 계산이 범위를 넘지 않게 먼저 확인한다.
         */

        if ((size_t)n > SIZE_MAX - size) {
            fprintf(stderr, "file too large\n");
            free(data);
            return -1;
        }

        size_t needed = size + (size_t)n;

        /*
         * [5] 필요한 공간이 기존 capacity보다 크면 확장
         *
         * size     = 실제 사용량
         * capacity = 확보한 공간
         *
         * needed <= capacity:
         *   공간이 충분하므로 재할당 필요 없음
         *
         * needed > capacity:
         *   realloc()으로 공간을 확장해야 함
         */

        if (needed > capacity) {

            /*
             * 처음에는 capacity가 0이므로
             * 4096바이트부터 시작한다.
             *
             * 이후에는 기존 크기를 바탕으로 확장한다.
             */

            size_t new_capacity =
                (capacity == 0) ? 4096 : capacity;

            /*
             * 필요한 크기가 될 때까지 2배씩 증가
             */

            while (new_capacity < needed) {

                /*
                 * 2배 계산이 size_t의 최댓값을
                 * 넘어갈 위험이 있다면,
                 * 필요한 크기까지만 확보한다.
                 */

                if (new_capacity > SIZE_MAX / 2) {
                    new_capacity = needed;
                    break;
                }

                new_capacity *= 2;
            }
            /*
             * [6] realloc()
             *
          
             */

            char *temp = realloc(data, new_capacity);

            if (temp == NULL) {
                perror("realloc");
                free(data);
                return -1;
            }

            // 성공했으므로 새 주소를 data에 저장
            data = temp;

            // 이제 확보한 메모리 크기를 갱신
            capacity = new_capacity;
        }

        /*
         * [7] memcpy()로 이번에 읽은 데이터를 뒤에 추가
         *
         * memcpy(destination, source, number_of_bytes)
         *
         * destination : 복사할 목적지 주소
         * source      : 복사할 원본 주소
         * number      : 복사할 바이트 수
         *
         * memcpy(data + size, buf, n);
         *
         * data + size:
         *   기존 데이터의 바로 다음 위치를 가리킨다.
         *
         * 예:
         *
         * 기존 data:
         * [A][B][C][D][ ][ ][ ][ ]
         *             ^
         *             size = 4
         *
         * 새 buf:
         * [X][Y][Z]
         *
         * memcpy 이후:
         * [A][B][C][D][X][Y][Z][ ]
         */

        memcpy(data + size, buf, (size_t)n);

        // 실제 저장된 데이터 크기 증가
        size += (size_t)n;
    }

    /*
     * [8] read() 반복문이 종료된 이유 확인

     */

    if (n == -1) {
        perror("read");
        free(data);
        return -1;
    }

    /*
     * [9] 마지막 num줄이 시작하는 위치 찾기
     *
     * start는 data에서 출력할 시작 인덱스다.
     *
     * num줄보다 파일의 줄 수가 적으면
     * 처음부터 전부 출력해야 하므로
     * 기본값은 0이다.
     */

    size_t start = 0;

    /*
     * -n 0이면 아무것도 출력하지 않는다.
     *
     * start = size이면
     * 출력 길이(size - start)가 0이 된다.
     */

    if (num == 0) {
        start = size;
    }
    else if (size > 0) {

        /*
         * pos:
         *   뒤에서부터 탐색할 현재 위치
         *
         * 처음에는 size로 설정한다.
         * 실제 마지막 인덱스는 size - 1이다.
         */

        size_t pos = size;

        // 지금까지 발견한 줄바꿈 개수
        int count = 0;

        /*
         * [10] 파일 마지막이 '\n'이면 건너뛰기
         *
         * 예:
         * "A\nB\nC\n"
         *
         * 끝의 '\n'은 C줄의 종료 문자이지,
         * 새로운 빈 줄 하나가 시작했다는 뜻이 아니다.
         *
         * 그래서 마지막 '\n'은
         * 역방향 줄 계산에서 제외한다.
         *
         * 단, 출력 데이터에서는 제거하지 않는다.
         */

        if (data[size - 1] == '\n') {
            pos--;
        }

        /*
         * [11] 뒤에서 앞으로 탐색
         *
         * pos--를 먼저 실행한 후
         * data[pos]를 검사한다.
         *
         * 예: pos = 8
         *      pos-- -> 7
         *      data[7] 검사
         *
         * size_t는 부호 없는 정수이므로
         * pos > 0 조건을 먼저 확인하는 게 중요하다.
         */

        while (pos > 0) {
            pos--;

            if (data[pos] == '\n') {
                count++;

                /*
                 * 뒤에서 num번째 경계 '\n'을 찾았다면
                 * 그 다음 위치부터 출력한다.
                 */

                if (count == num) {
                    start = pos + 1;
                    break;
                }
            }
        }
    }

    /*
     * [12] 실제로 출력할 범위 계산
     *
     * start  : 출력 시작 위치
     * length : 출력할 전체 바이트 수
     *
     * 예:
     *
     * data = "A\nB\nC\nD\nE\n"
     * size = 10
     *
     * num = 2
     * start = 6
     *
     * data + start -> "D\nE\n"
     * length = 10 - 6 = 4
     */

    size_t written = 0;
    size_t length = size - start;

    /*
     * [13] write()로 필요한 부분을 전부 출력
     *
     * write()는 요청한 바이트를
     * 한 번에 모두 쓰지 못할 수 있다.
     *
     * 그래서 cat/head에서 했던 것처럼
     * written을 증가시키며 반복한다.
     */

    while (written < length) {

        ssize_t w = write(
            STDOUT_FILENO,
            data + start + written,
            length - written
        );

        /*
         *
         * data + start + written:
         *   아직 출력하지 않은 데이터의 시작 주소
         *
         * length - written:
         *   아직 출력해야 할 바이트 수
         */

        if (w == -1) {
            perror("write");
            free(data);
            return -1;
        }

        if (w == 0) {
            fprintf(stderr, "write: no progress\n");
            free(data);
            return -1;
        }

        written += (size_t)w;
    }

    /*
     * [14] 동적 메모리 해제

     */

    free(data);

    return 0;
}

/*
 * main()
 *
 * head에서 작성했던 흐름을 재활용한다.
 *
 * 1. getopt()로 -n 옵션 분석
 * 2. 입력 파일이 없으면 표준 입력
 * 3. 여러 파일이면 반복
 * 4. "-"는 표준 입력
 * 5. 일반 파일은 open() -> print_tail() -> close()
 */

int main(int argc, char *argv[]) {

    int num = 10;       // 기본적으로 마지막 10줄
    int status = 0;     // 최종 종료 상태: 0 성공, 1 실패
    int opt;


    while ((opt = getopt(argc, argv, "n:")) != -1) {

        if (opt != 'n') {
            return 1;
        }

        char *end;
        errno = 0;

        long value = strtol(optarg, &end, 10);

        /*
         * 입력 검증:
         * 1. 숫자가 하나도 없음
         * 2. 숫자 뒤에 문자가 남음
         * 3. long 범위 초과
         * 4. 음수
         * 5. int 범위 초과
         */

        if (optarg == end || *end != '\0' ||
            errno == ERANGE || value < 0 || value > INT_MAX) {

            fprintf(stderr, "Invalid line count: %s\n", optarg);
            return 1;
        }

        num = (int)value;
    }


    if (optind == argc) {

        if (print_tail(STDIN_FILENO, num) == -1) {
            status = 1;
        }
    }
    else {

    

        for (int i = optind; i < argc; i++) {

            int fd;

            if (strcmp(argv[i], "-") == 0) {
                fd = STDIN_FILENO;
            }
            else {

                // 일반 파일을 읽기 전용으로 연다.
                fd = open(argv[i], O_RDONLY);

                if (fd == -1) {
                    perror(argv[i]);
                    status = 1;
                    continue;
                }
            }

            // 현재 입력에서 마지막 num줄 출력
            if (print_tail(fd, num) == -1) {
                status = 1;
            }

            if (fd != STDIN_FILENO) {
                if (close(fd) == -1) {
                    perror("close");
                    status = 1;
                }
            }
        }
    }

    return status;
}
