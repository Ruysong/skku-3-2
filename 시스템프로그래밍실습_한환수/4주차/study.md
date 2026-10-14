# 시스템프로그래밍실습 - 4주차 스터디: File I/O

작성 코드 복습: [4주차 줄 번호 구현 설명](../lab-study.md#4주차-file-io와-줄-번호), [PA1 검색기 상세](../PA/PA_1/study.md).

## 한 줄 결론

4주차는 C program이 Linux kernel의 Unix I/O interface로 file에 접근하는 방법을 배우는 주차다. Pathname을 열어 file descriptor를 받고, byte를 read/write하며, current offset, error, metadata를 관리한다. Lab은 이 low-level I/O만 사용해 text file의 각 줄 앞에 line number를 붙이는 문제다.

    input pathname
          |
          v
      open -> file descriptor -> offset
          |                       |
          v                       v
      read -> byte buffer -> newline detection
          |                       |
          +-> write(line number + separator + original bytes)
          |
          v
        close

## 목차

1. 원본 자료와 전체 구조
2. File, directory, pathname, cwd
3. Unix I/O: fd, open, close, read, write, lseek
4. Short count와 byte stream
5. Metadata: stat, fstat, lstat
6. Standard I/O와 buffering
7. Unix I/O vs Standard I/O
8. errno error handling
9. Week 4 Lab - Aladdin.txt.no
10. PA1 연결
11. 혼동 방지와 복습 질문

---

## 1. 원본 자료와 전체 구조

- 원본: week4-file_io.pdf (54쪽)
- 배포 skeleton: week4_code/w4_skeleton_code.c
- Lab: Aladdin.txt를 Aladdin.txt.no로 변환
- 제출: source code와 Makefile을 tar로 묶어 iCampus에 제출. TA는 make로 build한다.
- 핵심 제약: Lab은 Unix File I/O만 사용한다. 표준 I/O 및 편의 string/int conversion function으로 해결하면 목적을 벗어난다.

    FILE / DIRECTORY
       |
       +-- type, hierarchy, pathname, cwd, metadata
       |
    UNIX I/O
       |
       +-- system call, fd, open/close, read/write, lseek
       |
    STANDARD I/O
       |
       +-- stream + buffer, fopen/fclose, fgets/fputs
       |
    ERROR HANDLING
       |
       +-- return value, errno, perror, strerror
       |
    LAB: text line prefixing

## 2. File, directory, pathname

Linux file은 순서 있는 bytes의 sequence다. Unix 관점에서는 regular file뿐 아니라 terminal, disk partition, socket, named pipe도 file interface로 다룰 수 있다. Text file과 binary file의 구분은 application 관점이며 kernel read/write가 text를 특별 대우하지 않는다. Text line은 newline으로 끝나는 character sequence라는 convention이다.

| type | 역할 |
| --- | --- |
| regular file | arbitrary data |
| directory | filename과 file을 연결하는 entry 집합 |
| socket | 다른 machine process와 통신 |
| named pipe | 다른 process와 통신 |
| symbolic link | 다른 pathname을 가리킴 |
| device file | device를 표현 |

Directory에는 dot(자기 자신), double dot(parent) entry가 있다. 모든 file은 root slash에서 시작하는 hierarchy 안에 있고, process마다 current working directory(cwd)가 존재한다.

| pathname | 기준 | 예시 |
| --- | --- | --- |
| absolute | root부터 | /home/student/week4/Aladdin.txt |
| relative | cwd부터 | Aladdin.txt, ../week3/main.c |

Lab input은 executable argument로 주어지는 txt pathname이다. Filename을 단순 display string이 아니라 cwd에 상대적일 수 있는 location specification으로 이해해야 한다.

## 3. Unix I/O

Unix I/O는 kernel이 export하는 low-level interface다. 열린 file에는 integer file descriptor와 current file offset이 연결된다.

    pathname -> open -> fd -> read/write -> byte stream
                           |
                           +-> lseek: next offset 이동
                           +-> close: lifecycle 종료

### open과 descriptor

Open success는 non-negative integer fd, error는 minus one이다. Process는 보통 아래 세 fd를 이미 가진다.

| fd | macro | 기본 대상 |
| --- | --- | --- |
| 0 | STDIN_FILENO | keyboard |
| 1 | STDOUT_FILENO | normal terminal output |
| 2 | STDERR_FILENO | error output |

| flag | 의미 |
| --- | --- |
| O_RDONLY | read only |
| O_WRONLY | write only |
| O_RDWR | read and write |
| O_CREAT | 없으면 create |
| O_APPEND | file end에 append |

O_CREAT를 쓰면 permission mode가 필요하다. Permission은 read 4, write 2, execute 1의 합이며 owner/group/others에 각각 적용한다. 예를 들어 0664는 owner와 group에 read/write, others에 read를 준다.

### close

Close는 file access 종료를 kernel에 알린다. Open 성공 뒤에는 정상 path뿐 아니라 error path에서도 fd lifecycle을 생각해야 한다. 이미 닫힌 fd를 다시 close하는 일은 특히 threaded program에서 위험하다. Close return value도 확인한다.

### read와 write

Read는 current offset에서 file bytes를 memory buffer로 복사하고, 실제 읽은 만큼 offset을 옮긴다.

| read return | 의미 |
| --- | --- |
| positive | 실제 읽은 byte 수 |
| 0 | EOF |
| minus one | error |

Write는 memory bytes를 current offset에 복사하고, 실제 쓴 만큼 offset을 옮긴다. Requested count보다 적게 읽거나 쓴 short count가 항상 error는 아니다. Disk file에서는 드물 수 있지만 terminal, pipe, socket, EOF에서는 자연스럽다. General Unix I/O code는 one read/write가 buffer 전체를 처리했다고 가정하지 않는다.

### lseek과 offset

Offset은 다음 read/write 위치다. Lseek는 content를 읽지 않고 이 위치를 명시적으로 옮긴다. File end 뒤로 seek한 뒤 write하면 file hole이 생길 수 있다. Lab은 sequential read/write라 lseek가 중심은 아니지만, read/write가 offset을 자동으로 이동시킨다는 점은 핵심이다.

## 4. Short count와 byte stream

Unix I/O는 text line API가 아니라 byte API다. 따라서 line numbering을 하려면 buffer에서 newline을 찾아 logical line boundary를 관리해야 한다.

    chunk 1: Once upon a ti
    chunk 2: me + newline + Next line + newline

첫 line은 두 chunk를 이어 처리해야 한다. Input의 한 line이 system memory보다 작다는 전제와 read chunk가 line boundary와 일치한다는 것은 다른 이야기다. Byte count, newline, EOF를 분리해서 생각해야 한다.

| 상황 | short count |
| --- | --- |
| regular disk read | EOF 외에는 보통 없음 |
| regular disk write | 보통 없음 |
| terminal text read | 가능 |
| network socket read/write | 가능 |
| EOF | read 0 |

PDF의 best practice는 short count를 항상 허용하는 것이다.

## 5. Metadata

Metadata는 data about data다. Kernel이 per-file로 유지하고 stat family로 얻는다.

| function | 지정 방식 | link 처리 |
| --- | --- | --- |
| stat | pathname | target information |
| fstat | open fd | open file information |
| lstat | pathname | link 자체 information |

Stat family는 struct stat에 file size, permission, type 같은 property를 채운다. Content를 read해 계산하는 것이 아니라 kernel metadata를 읽는 것이다. Metadata access는 Unix I/O가 필요한 대표 상황이다.

## 6. Standard I/O와 buffering

Standard I/O는 Unix I/O 위에 구현된 higher-level library다. Open file을 fd 하나가 아니라 fd와 memory buffer를 포함하는 stream으로 모델링한다.

| 목적 | Unix I/O | Standard I/O |
| --- | --- | --- |
| open/close | open, close | fopen, fclose |
| byte read/write | read, write | fread, fwrite |
| text line | byte 직접 처리 | fgets, fputs |
| formatted I/O | 직접 format 처리 | fprintf, fscanf |
| directory | low-level direct API | opendir, readdir, closedir |

C process도 stdin, stdout, stderr 세 stream을 가진다. 이는 fd 0, 1, 2와 대응하지만 stream에는 user-space buffer가 추가된다.

Buffering은 character 하나마다 kernel call을 하는 overhead를 줄인다.

    without buffer: getchar -> read system call -> 1 char
    with buffer: read block -> user buffer -> 여러 char 소비 -> refill

Output buffer는 fflush, fclose, exit, main return 등에서 flush될 수 있다. Printf 호출이 항상 즉시 kernel write와 일대일 대응하지는 않는다.

## 7. 언제 무엇을 쓸까

| 관점 | Unix I/O | Standard I/O |
| --- | --- | --- |
| 장점 | general, low-level control, metadata | buffering, short count 편의 |
| 단점 | small I/O overhead, line handling 까다로움 | metadata API 없음, socket에 부적합 |
| 적합 | socket, pipe, metadata | disk/terminal text file |

Standard I/O는 Unix I/O의 경쟁자가 아니라 higher abstraction이다. 같은 file에 둘을 무심코 섞으면 stream buffer와 offset 상태가 어긋날 수 있다. 이번 Lab이 Unix I/O만 요구하는 이유는 descriptor와 byte flow를 직접 이해시키기 위해서다.

## 8. Error handling

System call 실패는 보통 minus one return이며 detail은 errno에 남는다.

| 요소 | 역할 |
| --- | --- |
| return value | success/failure 1차 판단 |
| errno | 최근 failure error code |
| strerror(errno) | error 설명 string |
| perror(context) | context와 error를 stderr에 출력 |

    open -> failure
       |
       +-> return value 즉시 확인
       +-> perror("open input")
       +-> 이미 연 resource cleanup
       +-> nonzero return

Errno는 failure return을 확인한 직후 해석한다. 성공한 call 뒤에 남은 오래된 errno를 보고 error를 판단하면 안 된다.

## 9. Week 4 Lab - Aladdin.txt.no

Goal: Aladdin.txt의 각 text line 앞에 number, space, vertical bar, space를 붙여 Aladdin.txt.no를 만든다.

    input:
    Once upon a time
    Aladdin found a lamp

    output:
    1 | Once upon a time
    2 | Aladdin found a lamp

- Input은 command-line argument로 받는 txt filename
- Output filename은 input name 뒤에 dot no를 추가
- Line number는 1부터 시작
- Original bytes와 line ending 구조를 보존
- 마지막 line에 newline이 없으면 output에 임의 newline을 추가하지 않음

Skeleton의 의미:

| 요소 | 역할 |
| --- | --- |
| argv[1] | input filename |
| w4_strlen | library strlen 대신 length 계산 |
| get_output_filename | dot no output pathname 생성 |
| to_string(uint64_t) | line number character sequence 생성 |
| uint64_t line_number | 1-based counter |
| write(STDERR_FILENO, ...) | missing argument error |

To_string은 내부 array를 돌려주는 helper이며 helper array 수정 금지 지시를 지킨다.

Lab check:
- Unix File I/O만 사용
- Standard I/O 및 편의 string/int conversion function 사용은 0점 위험
- Unsigned long long은 최대 20 digit 고려
- Output create permission을 올바르게 지정
- Source와 Makefile을 tar archive해 iCampus 제출
- TA는 make로 build
- cat -e Aladdin.txt.no로 line ending/line count 확인
- Exact deadline은 iCampus submission timestamp가 최종 기준

    argument check
      -> input open
      -> output path/create
      -> byte read
      -> 각 logical line의 start에 prefix write
      -> original byte write
      -> EOF/error 구분
      -> both fd close
      -> make build and output verification
      -> archive submit

위 흐름은 submission code가 아니라 resource lifecycle checklist다.

## 10. PA1 연결

PA1은 arbitrary-size text file에서 interactive search를 수행하는 pa1 program이다. Lab의 descriptor, streaming byte processing, pathname, error handling이 그대로 이어진다.

| query type | output |
| --- | --- |
| single word | 모든 line:index |
| multiple words | 모든 word가 있는 line number |
| quoted phrase | 모든 phrase line:index |
| word1*word2 | order를 만족하는 line number |
| PA1EXIT | exit |

PA1에서 특히 중요한 명세:
- File 전체는 memory보다 훨씬 클 수 있어 전체 load 금지
- 한 line/word는 memory보다 작아 streaming processing 가능
- Word match는 case-insensitive지만 punctuation은 word의 일부
- Empty line도 line number에 포함, number는 1-based
- Start index는 whitespace/punctuation을 포함한 0-based
- Result가 없어도 query마다 blank output line 유지
- Single word/phrase는 모든 location을 순서대로 출력
- Multiword/pattern은 line number를 한 번만 순서대로 출력
- Makefile은 반드시 pa1 binary 생성
- Linux build/run, allowed header만 사용, hand-implemented header 최소 하나
- PDF 본문과 ARCS는 deadline을 2026-10-04 23:59로 적는다. PDF submission page의 2026-04-05는 문맥상 오류로 보이므로 iCampus timestamp를 최종 확인한다.

PA1은 개인 과제다. 이 스터디에는 specification과 testing 기준만 보관하고, 제출 code는 본인이 이해하고 설명할 수 있는 별도 작업으로 관리한다.

## 11. 혼동 방지

- Pathname은 file content가 아니라 hierarchy에서 file을 찾는 이름이다.
- Open return은 FILE pointer가 아니라 integer fd다.
- Read 0은 EOF, minus one은 error다.
- Requested count보다 적은 result가 short count일 수 있다.
- Lseek는 content를 읽지 않고 offset만 바꾼다.
- Stat, fstat, lstat는 pathname, fd, link 자체라는 차이가 있다.
- Stdout stream과 STDOUT_FILENO fd는 대응하지만 abstraction이 다르다.
- Standard I/O는 buffer 때문에 편하지만 Unix I/O는 metadata/socket/pipe에 더 general하다.
- Errno는 error return 뒤에만 해석한다.
- Lab은 counter 출력만이 아니라 line boundary와 original bytes 보존 문제다.

## 12. 복습 질문

1. Regular file, directory, socket, named pipe는 각각 무엇인가?
2. Absolute/relative pathname은 cwd와 어떤 관계인가?
3. 0, 1, 2 fd는 무엇인가?
4. O_RDONLY, O_WRONLY, O_RDWR, O_CREAT, O_APPEND의 뜻은?
5. Read의 positive, zero, minus one return은?
6. Short count가 정상일 수 있는 상황은?
7. Offset은 언제 자동 이동하고 lseek는 무엇을 하는가?
8. Stat, fstat, lstat의 차이는?
9. Standard I/O buffering은 왜 필요한가?
10. Unix I/O가 socket/pipe에 적합한 이유는?
11. Perror/strerror는 언제 쓰는가?
12. Lab이 standard I/O를 금지하는 이유는?
13. PA1에서 whole-file loading이 금지되는 이유는?

## 13. PDF 페이지 지도

| 페이지 | 내용 | 핵심 |
| --- | --- | --- |
| 2-8 | file/directory/path | byte sequence, type, cwd, pathname |
| 9-20 | Unix I/O | fd, open/close/read/write/lseek, short count |
| 21-23 | metadata | stat, fstat, lstat |
| 24-31 | Standard I/O | stream, buffer, stdio, directory stream |
| 32-34 | comparison | Unix vs Standard tradeoff |
| 35-38 | error | return code, errno, perror, strerror |
| 39-49 | summary/example | interface 선택 기준 |
| 50-54 | Lab | line numbering, Unix I/O only, make/submit |

## 이번 주 종료 조건

- FD와 FILE pointer의 차이를 설명할 수 있다.
- Open -> read/write -> close lifecycle의 return value를 확인한다.
- Read result가 buffer size와 항상 같지 않은 이유를 안다.
- Standard I/O와 Unix I/O 선택 기준을 구분한다.
- Lab format, line ending 보존, Unix I/O-only, Makefile 제출을 확인했다.
- PA1 query type, streaming memory constraint, output ordering, pa1 build condition을 알고 있다.
