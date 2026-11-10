
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

// cp에서 배운 read/write 복사 루프
int copy_data(int src_fd, int dst_fd) {
    char buf[4096];
    ssize_t n;

    while ((n = read(src_fd, buf, sizeof(buf))) > 0) {
        ssize_t written = 0;

        while (written < n) {
            ssize_t w = write(dst_fd, buf + written,
                              n - written);

            if (w == -1) {
                if (errno == EINTR) continue;
                perror("write");
                return -1;
            }

            if (w == 0) {
                fprintf(stderr, "write: no progress\n");
                return -1;
            }

            written += w;
        }
    }

    if (n == -1) {
        perror("read");
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s SOURCE DEST\n", argv[0]);
        return 1;
    }

    const char *source = argv[1];
    const char *destination = argv[2];

    // 1. 우선 이름/경로 변경 시도
    if (rename(source, destination) == 0) {
        return 0;
    }

    /*
     * EXDEV = Cross-Device Link 오류
     *
     * 서로 다른 파일 시스템이라 rename이
     * 불가능한 경우에만 복사 방식으로 전환한다.
     */
    if (errno != EXDEV) {
        perror("rename");
        return 1;
    }

    // 2. 원본 열기
    int src_fd = open(source, O_RDONLY);

    if (src_fd == -1) {
        perror(source);
        return 1;
    }

    /*
     * 일반 파일만 복사하도록 제한한다.
     *
     * S_ISREG: Is Regular File
     * 일반 파일인지 검사하는 매크로
     *
     * st_mode: 파일 종류와 권한 정보
     */
    struct stat info;

    if (fstat(src_fd, &info) == -1) {
        perror("fstat");
        close(src_fd);
        return 1;
    }

    if (!S_ISREG(info.st_mode)) {
        fprintf(stderr,
                "mv: only regular files supported for cross-filesystem moves\n");
        close(src_fd);
        return 1;
    }

    /*
     * O_EXCL = Exclusive
     *
     * O_CREAT와 함께 사용하면 대상 파일이
     * 이미 존재할 경우 open()이 실패한다.
     *
     * 기존 대상 파일을 실수로 덮어쓰지 않도록
     * 입문판에서는 이 방식을 선택한다.
     *
     * info.st_mode & 0777:
     * 원본의 기본 권한 비트만 추출한다.
     * 실제 생성 권한에는 umask가 적용된다.
     */
    int dst_fd = open(
        destination,
        O_WRONLY | O_CREAT | O_EXCL,
        info.st_mode & 0777
    );

    if (dst_fd == -1) {
        perror(destination);
        close(src_fd);
        return 1;
    }

    // 3. 파일 복사
    int success = (copy_data(src_fd, dst_fd) == 0);

    if (close(src_fd) == -1) {
        perror("close source");
        success = 0;
    }

    if (close(dst_fd) == -1) {
        perror("close destination");
        success = 0;
    }

    /*
     * 복사 실패 시 원본은 유지한다.
     * 이번에 생성한 불완전한 대상 파일은 제거한다.
     */
    if (!success) {
        if (unlink(destination) == -1) {
            perror("cleanup destination");
        }
        return 1;
    }

    /*
     * 4. 복사가 끝난 뒤에만 원본 삭제
     *
     * unlink()는 디렉터리에서 해당 파일 이름의
     * 연결을 제거하는 시스템 콜 인터페이스다.
     */
    if (unlink(source) == -1) {
        perror("unlink source");
        return 1;
    }

    return 0;
}
