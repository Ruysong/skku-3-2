#include <stdio.h> //스탠다드 아이오
#include <fcntl.h> //에프 컨트롤 
#include <unistd.h> //유닉스 스탠더드
#include <string.h>

int print_file(int fd){
     char buf[4096]; //버퍼 잡고 읽을거임
        ssize_t n ;

    while ((n = read(fd, buf, sizeof(buf))) > 0) {
    ssize_t written = 0;  // 지금까지 쓴 바이트 수

    while (written < n) {
        ssize_t w = write(1, buf + written, n - written);
        // written이 n보다 작은 동안 반복
        // write(1, buf + written, n - written)
        // 성공적으로 쓴 바이트 수만큼 written 증가
        if (w == -1) {
            perror("write");
            return -1;
        }

        if (w == 0) {
           
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


int main(int argc,char *argv[]){
    int status = 0;  // 정상 상태 , 만약 저기 print함수 등에서 에러나면 1로 바뀜.
/* 
    char *filename = argv[];//1에 파일이름 있으니까. 
    // 선언하는 포인터니까 별이 저기 붙음. 
     */
    int fd ;
    
  if (argc == 1) {
    // 하나밖에 없구만. 그렇다면 표준 입력을 읽고 출력
    fd =0;
    int result = print_file(fd);

if (result == -1) {
    // 오류가 발생했다는 사실을 기록
    status = 1;  // 실패한 적이 있다고 기록
}
    }
    else {
    for (int i = 1; i < argc; i++) {
        if((strcmp(argv[i],"-")==0)){
                // 표준 입력을 읽고 출력
        fd =0; //이렇게 해야 안헷갈. fd는 표준입출력
        int result = print_file(fd);

        if (result == -1) {
            // 오류가 발생했다는 사실을 기록
            status = 1;  // 실패한 적이 있다고 기록
        }
    }
        else { 
        fd =open(argv[i],O_RDONLY);

        if (fd == -1) { //일단 오픈이 실패했는지 아닌지 확인
            perror(argv[i]);
            status = 1; //이거도 fd가 아예 문제가 있는거니께
            continue;
        }
        int result = print_file(fd);

        if (result == -1) {
            // 오류가 발생했다는 사실을 기록
            status = 1;  // 실패한 적이 있다고 기록
        }
      if (close(fd) == -1) {
    perror("close");
    status = 1;
}        
    } 
}
    }

 
   return status;

}