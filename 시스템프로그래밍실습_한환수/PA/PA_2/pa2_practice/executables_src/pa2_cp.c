
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int copy_file(int src_fd, int dst_fd) {
    char buf[4096];
    ssize_t n;

    while ((n = read(src_fd, buf, sizeof(buf))) > 0) {
        ssize_t written = 0;

        while (written < n) {
            ssize_t w = write(dst_fd, buf + written, n - written);

            if (w == -1) {
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

    char *source = argv[1];
    char *destination = argv[2];

    int src_fd = open(source, O_RDONLY);

    if (src_fd == -1) {
        perror(source);
        return 1;
    }

    /*
     * 대상 파일을 열지만 아직 비우지는 않는다.
     *
     * O_WRONLY : 쓰기 전용
     * O_CREAT  : 없으면 생성
     *
     * 0644는 새 파일 생성 시 요청하는 권한이다.
     * 실제 권한에는 umask가 적용된다.
     *
     * 지금 O_TRUNC를 사용하지 않는 이유는
     * 동일 파일 여부를 먼저 검사해야 하기 때문이다.
     */
    int dst_fd = open(destination, O_WRONLY | O_CREAT, 0644);

    if (dst_fd == -1) {
        perror(destination);
        close(src_fd);
        return 1;
    }

    /*
     * fstat(fd, &info)
     *
     * 열린 파일의 메타데이터를 struct stat에 저장한다.
     *
     * st_dev : 장치 번호
     * st_ino : inode 번호
     *
     * 두 값이 모두 같으면 동일한 파일이다.
     */
    struct stat src_info;
    struct stat dst_info;

    if (fstat(src_fd, &src_info) == -1) {
        perror("fstat source");
        close(src_fd);
        close(dst_fd);
        return 1;
    }

    if (fstat(dst_fd, &dst_info) == -1) {
        perror("fstat destination");
        close(src_fd);
        close(dst_fd);
        return 1;
    }

    if (src_info.st_dev == dst_info.st_dev &&
        src_info.st_ino == dst_info.st_ino) {

        fprintf(stderr, "cp: source and destination are the same file\n");

        close(src_fd);
        close(dst_fd);
        return 1;
    }

    /*
     * ftruncate(fd, length)
     *
     * 이미 열린 파일의 크기를 length로 변경한다.
     * 여기서는 크기를 0바이트로 만든다.
     *
     * 주의:
     * ftruncate() 자체는 현재 파일 오프셋을
     * 0으로 이동시키지 않는다.
     *
     * 지금은 대상 파일을 새로 열었으므로
     * 오프셋이 처음(0)에 있다.
     */
    if (ftruncate(dst_fd, 0) == -1) {
        perror("ftruncate");
        close(src_fd);
        close(dst_fd);
        return 1;
    }

    int status = 0;

    if (copy_file(src_fd, dst_fd) == -1) {
        status = 1;
    }

    if (close(src_fd) == -1) {
        perror("close source");
        status = 1;
    }

    if (close(dst_fd) == -1) {
        perror("close destination");
        status = 1;
    }

    return status;
}
