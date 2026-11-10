#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>   // Error Number: 오류 번호
#include <limits.h>  // Integer Limits: 정수 범위 상수


int print_file(int fd,int num){//즉 이제 numㅇ로 몇줄 볼지 알수 있느거지
    if (num == 0) return 0; //0이면 끝내 걍 ㅋㅋ
    char buf[4096];
    ssize_t n ;
    int stop_here;
    int count =0;
    //여기서 부터 읽기 전체 와일문.
    while((n= read(fd,buf,sizeof(buf))) > 0){
        //전처럼 읽으려고. 근데 이번엔 읽다가 줄바꿈 나오면 카운트 올릴거고. 
     
        for(int i=0;i<n;i++){
            if (buf[i] =='\n'){
                count++;
                  if(count ==num) {
             stop_here = i+1;
            break;
        }
            }
        }     
        ssize_t limit = n;
            if(count ==num){
               limit =stop_here;
            }
       
        //여기서 부터 쓰기해보자
        ssize_t written=0;
        while(written < limit){
        
            ssize_t w = write(1,buf+written, limit-written);
            if (w ==-1){
                //아쿠 망했다.
                perror("write");
                return -1;
            }
            else if(w==0){
                fprintf(stderr, "write: no progress\n");               
                return -1;
            }

             written += w;
           
        }     if(count ==num) break; // 이러면 읽기는 끝내고 나와버리는거지.     
    }
    if (n == -1) {
        perror("read");
        return -1;
    }
    return 0;
}

int main(int argc,char *argv[]){
    int status =0;
    int fd;
    int opt; //옵션에 대한 변수처리
   
    int num=10; //원래 기본10줄 표시, 근데 새롭게 -n이 들어오면 바꿀거.
    
    while ((opt = getopt(argc, argv, "n:")) != -1) {
                // 발견한 옵션 처리에 다한 처리
        //지금 opt에는 반환값인 n이 들어있음.이건 옵션 종류를 말함.

         if(opt != 'n') return 1; //잘못된 옵션이면 끝내 걍
        char *end;
        errno = 0;

        long value = strtol(optarg, &end, 10);

        if (optarg == end || *end != '\0' ||
            errno == ERANGE || value < 0 || value > INT_MAX) {
            fprintf(stderr, "Invalid line count: %s\n", optarg);
            return 1;
        }
        num = (int)value;
      /*   
        여기서 &end는 end라는 포인터 변수의 주소를 전달한다는 뜻이고, 
        *end는 변환이 멈춘 위치의 문자를 읽는다는 뜻이야.검사 조건은 다음과 같아.
조건	            의미
optarg == end	숫자를 하나도 변환하지 못함
*end != '\0'	숫자 뒤에 문자가 남아 있음
errno == ERANGE	변환값이 long의 표현 범위를 벗어남
value < 0	음수
value > INT_MAX	int로 저장할 수 없을 만큼 큼
fprintf(stderr, ...)는 표준 오류 출력으로 메시지를 보내는 함수야. */
    }

    if(argc == optind) {
        fd =0;
        if(print_file(fd,num)==-1) {
            status =1;
        }
        if(fd!= -1 && fd!= 0) close (fd);
}   

    else{      

        for(int i= optind;i<argc;i++) {
            if(strcmp(argv[i], "-")==0) { //짝대기 처리
                fd =0;
                if(print_file(fd,num)==-1) {
                    status =1;
                }

            }
            else {
                char *filename = argv[i];
                //이러면 이제 옵션 끝나고 이름시작할떄의 그것.optind부터.
                fd = open(filename,O_RDONLY); //이제 fd값 정해졌을거고. 
                //오픈 실패할 경우도 정리는 해야지
                    if (fd == -1)  {
                    perror(filename);
                    status =1; //실패상태 이거인거로 알고 그냥 넘어가는거지.
                    continue;
                }
            if(print_file(fd,num)==-1) {
                status =1;
            }}
        
       if (fd != 0) {
        if (close(fd) == -1) {
            perror("close");
            status = 1;
    }
}
    }
}  
    return status;

}