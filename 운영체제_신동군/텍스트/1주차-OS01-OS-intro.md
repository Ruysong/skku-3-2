# 1주차 OS01 OS intro

## 자료 정보

- 과목: 운영체제 - 신동군
- 원본: ../1주차/OS01-OS intro.pdf
- 주차: 1주차
- 정리일: 2026-08-31
- 방식: PDF에서 페이지별 텍스트를 자동 추출해 원문 보존 중심으로 정리

## Page 1

```text
Operating Systems
1
Dongkun Shin, SKKU
```

## Page 2

```text
Syllabus
• Instructors:
– Dongkun Shin
– Office : Room 400310 (Semiconductor Building)
– E-mail : dongkun@skku.edu
– Office Hours: Mon. 14:00-16:00 or by appointment
• Lecture notes
– iCampus
– Lecture notes and talks will be given in English.
• Flipped Learning
– 1.5h online pre-production video + 1.5h offline class (330110)
– Students must complete the pre-production video study before
the offline class each week.
– Please bring your own laptop, as we may conduct hands-on
exercises in the classroom.
2
Dongkun Shin, SKKU
```

## Page 3

```text
About Me
• Research Topics
– Operating Systems and File Systems
– Intelligent Flash Storage (Smart SSD)
– On-Device Machine Learning for Intelligent Embedded Systems
– Neural Network Processor (NPU) and its Compiler
– Intelligent Memory (Processing-in-Memory)
– Multicore & GPU
3
Dongkun Shin, SKKU
```

## Page 4

```text
Syllabus (cont’d)
• Main text
– Operating Systems: Three Easy Pieces
– http://pages.cs.wisc.edu/~remzi/OSTEP/
– free online book!!
– The book is very easy and funny.
– You have to read the scheduled chapter
before the class time.
• Grading policy (subject to change)
– Midterm exam: 35%
– Final exam: 35%
– Assignment (Report, Project): 30%
– If you miss one or both of exams, you will fail this course.
– Cheating on tests and other assignments will not be tolerated and you
will take no (or a negative) point for the test!
4
Dongkun Shin, SKKU
```

## Page 5

```text
Syllabus (cont’d)
• Course Outline
– Part 1. CPU Virtualization
• Process
• CPU Scheduling
– Part 2. Memory Virtualization
• Address Space, Allocation
• Address Translation
• Paging, TLB, Swapping
– Part 3. Concurrency
• Thread
• Lock, Semaphore
– Part 4. Persistence
• I/O Systems
• Storage
• File System
5
Dongkun Shin, SKKU
```

## Page 6

```text
Syllabus (cont’d)
• Assignments
– Homework in the textbook
– xv6 projects or others
• Prerequisites
– C programming
• If you are not familiar with C programming, I strongly
recommend against taking this class.
– System Programming
• must be familiar with the Linux Shell and Linux APIs
– Computer Architecture
• Cache, TLB, Virtual Memory, Exception, Interrupt, …
6
Dongkun Shin, SKKU
```

## Page 7

```text
Introduction to Operating Systems
7
Dongkun Shin, SKKU
```

## Page 8

```text
What happens when a program runs?
• Von Neumann model of Computing (Instruction Execution)
– The processor fetches an instruction from memory,
– Decodes it (i.e., figures out which instruction this is), and
– Executes it (i.e., it does the thing that it is supposed to do, like
add two numbers together, access memory, check a condition,
jump to a function, and so forth).
– After it is done with this instruction, the processor moves on to
the next instruction, and so on, and so on
8
Dongkun Shin, SKKU
```

## Page 9

```text
Operating System (OS)
• Makes it easy to run programs
– even many programs at the same time
• Allows programs to share memory
• Enables programs to interact with devices
➔ in charge of making sure the system operates correctly and
efficiently in an easy-to-use manner
9
Dongkun Shin, SKKU
```

## Page 10

```text
Operating System (OS)
• The role of OS
– Virtualization
• OS takes a physical resource (such as the processor, or
memory, or a disk)
• transforms it into a more general, powerful, and easy-to-use
virtual form of itself.
• System calls allow users to tell the OS what to do and thus
make use of the features of the OS
• Virtualization allows many programs to run concurrently
– Resource Manager
• Resource: CPU, memory, and disk
• OS manages those resources efficiently or fairly or indeed
with many other possible goals in mind.
10
Dongkun Shin, SKKU
```

## Page 11

```text
Computer System Organization
Application Application Application Application
Process Process Process Process
System Lib.
System Call Interface
OS
Mem Mgr Process Mgr File System IO Mgr
Device Drivers
ROM
Hardware
bootstrap
11
Dongkun Shin, SKKU
```

## Page 12

```text
Virtualizing the CPU
#include <stdio.h> prompt> gcc -o cpu cpu.c -Wall
#include <stdlib.h> prompt> ./cpu A
#include "common.h" A
A
int main(int argc, char *argv[]) A
{ A
if (argc != 2) { ˆC
fprintf(stderr, "usage: cpu <string>\n"); prompt>
exit(1);
}
prompt> ./cpu A & ; ./cpu B & ; ./cpu C & ; ./cpu D &
char *str = argv[1];
[1] 7353
[2] 7354
while (1) {
[3] 7355
printf("%s\n", str);
[4] 7356
Spin(1);
A
}
B Even though we have
return 0;
D
} only one processor,
C
A somehow all four of these
Spin(1): repeatedly checks the time and B
programs seem to be
returns once it has run for a second D
running at the same time!
C
A
C
B
Running Many Programs At Once
D
... 12
Dongkun Shin, SKKU
```

## Page 13

```text
Virtualizing the CPU
• Illusion that the system has a very large number of virtual CPUs.
• Turning a single CPU (or small set of them) into a seemingly
infinite number of CPUs and thus allowing many programs to
seemingly run at once
• OS is Resource Manager
– Policy
• If more than one program want to run at a particular time,
which should run?
13
Dongkun Shin, SKKU
```

## Page 14

```text
Virtualizing Memory
int prompt> ./mem 1
main(int argc, char *argv[]) (2134) address pointed to by p: 0x200000
{ (2134) p: 1
int *p; (2134) p: 2
p = malloc(sizeof(int)); (2134) p: 3
assert(p != NULL); (2134) p: 4
printf("(%d) address pointed to by p: %pn", getpid(), p); (2134) p: 5
*p = 0; ˆC
while (1) {
Spin(1);
prompt> ./mem 1 &; ./mem 1 &
*p = *p + 1;
[1] 24113
printf("(%d) p: %d\n", getpid(), *p);
[2] 24114
}
(24113) address pointed to by p: 0x200000
return 0;
(24114) address pointed to by p: 0x200000
}
(24113) p: 1
Each running program has allocated memory (24114) p: 1
(24114) p: 2
at the same address (0x200000), and yet
(24113) p: 2
each seems to be updating the value at
(24113) p: 3
0x200000 independently! (24114) p: 3
It is as if each running program has its own (24113) p: 4
(24114) p: 4
private memory, instead of sharing the same
...
physical memory with other running programs
14
Dongkun Shin, SKKU
```

## Page 15

```text
Virtualizing Memory
• Each process accesses its own private virtual
address space
• OS maps the virtual address onto the physical
memory of the machine.
• A memory reference within one running
program does not affect the address space of
other processes (or the OS itself); as far as the
running program is concerned, it has physical
memory all to itself.
• The reality, however, is that physical memory is
a shared resource, managed by the operating
system.
15
Dongkun Shin, SKKU
```

## Page 16

```text
Concurrency
#include <stdio.h> multi-threaded programs
prompt> gcc -o thread thread.c -Wall -pthread
#include <stdlib.h>
#include "common.h" prompt> ./thread 1000
Initial value : 0
volatile int counter = 0; Final value : 2000
int loops;
void *worker(void *arg) {
prompt> ./thread 100000
int i;
Initial value : 0
for (i = 0; i < loops; i++) {
Final value : 143012 // huh??
counter = counter + 1;
} prompt> ./thread 100000
pthread_exit(NULL); Initial value : 0
} Final value : 137298 // what the??
int main(int argc, char *argv[]) { counter = counter + 1; ➔ no atomic
if (argc != 2) {
3 instructions
fprintf(stderr, "usage: threads <loops>\n");
(1) load the value of the counter from
exit(1);
} memory into a register
loops = atoi(argv[1]);
(2) increment it
pthread_t p1, p2;
(3) store it back into memory.
printf("Initial value : %d\n", counter);
Pthread_create(&p1, NULL, worker, NULL);
Pthread_create(&p2, NULL, worker, NULL); How can we build a correctly
Pthread_join(p1, NULL);
working multi-threaded program?
Pthread_join(p2, NULL);
printf("Final value : %d\n", counter); What primitives are needed from
return 0;
the OS?
}
16
Dongkun Shin, SKKU
```

## Page 17

```text
Persistence
#include <stdio.h>
• DRAM is volatile
#include <unistd.h>
#include <assert.h>
• We need hardware and
#include <fcntl.h>
software to store data #include <sys/types.h>
persistently
int
main(int argc, char *argv[])
– H/W: HDD, SSD
{
– S/W: file system manages int fd = open("/tmp/file", O_WRONLY | O_CREAT |
O_TRUNC, S_IRWXU);
the disk, responsible for
assert(fd > -1);
storing any files the user int rc = write(fd, "hello world\n", 13);
assert(rc == 13);
creates in a reliable and
close(fd);
efficient manner on the disks return 0;
}
of the system
• OS does not create a private, virtualized disk for each application.
• Rather, users will want to share information that is in files.
• System calls: open, read, write, close
• Device driver issues I/O requests to the underlying storage device
17
Dongkun Shin, SKKU
```

## Page 18

```text
Persistence
• I/O management
user program
kernel
I/O buf
18
Dongkun Shin, SKKU
```

## Page 19

```text
I/O Mechanism
inst
c
a
CPU c Memory
h
e
data
I/O request transfers blocks of data from I/O
interrupt data (DMA)
device directly to main memory
command
without CPU intervention
I/O controller
I/O device
OS includes interrupt handlers
19
Dongkun Shin, SKKU
```

## Page 20

```text
Design Goals
• build up some abstractions in order to make the system
convenient and easy to use
• provide high performance
• minimize the overheads of the OS
– Extra time and space
• provide protection between applications, as well as between the
OS and applications
– isolating processes from one another is the key to protection
• Reliability
– The operating system must also run non-stop
– when it fails, all applications running on the system fail as well
• energy-efficiency, security, mobility
• Depending on how the system is used, the OS will have different
goals and thus likely be implemented in at least slightly different
ways.
20
Dongkun Shin, SKKU
```

## Page 21

```text
History
• Early Operating Systems: Just Libraries
• Beyond Libraries: Protection
– The idea of a system call was invented
– System call transfers control (i.e., jumps) into the OS while
simultaneously raising the hardware privilege level.
– User applications run in what is referred to as user mode which
means the hardware restricts what applications can do
– Trap raises the privilege level to kernel mode, the OS has full
access to the hardware of the system
• The Era of Multiprogramming, Minicomputer
– OS loads a number of jobs into memory and switch rapidly
between them, thus improving CPU utilization
– Protection mechanisms are necessary to control access to system
resources (including files)
– concurrency issues
– The introduction of the UNIX operating system
• Ken Thompson (and Dennis Ritchie) at Bell Labs
21
Dongkun Shin, SKKU
```

## Page 22

```text
History
• The Modern Era
– personal computer: Apple II, IBM PC
• Unfortunately, for operating systems, the PC at first
represented a great leap backwards
• forgot (or never knew of) the lessons learned in the era of
minicomputers
• DOS: no memory protection
• Mac OS (v9 and earlier): cooperative job scheduling
– Now
• Mac OS X
– Steve Jobs took his UNIX-based NeXTStep operating
environment with him to Apple
• Windows NT
• Linux
– Linus Torvalds wrote his own version of UNIX which
borrowed heavily on the principles and ideas behind the
original system, but not from the code base, thus avoiding
issues of legality
22
Dongkun Shin, SKKU
```

## Page 23

```text
History of UNIX
NeXTStep ➔ MacOS
1989 2001
1960’ 1973 1975~1983 1991~1994
ITS vi, Virtual Memory, File System, TCP/IP, …
MIT
UNIX Source
Multics UNIX, C BSD ver. 1, 2, 3, 4, 4.1, 4.2 FreeBSD
Bell Labs
UC Berkeley
Kenneth Thompson
Dennis Ritchie Bill Joy, Marshall Kirk McKusick Lawsuit
GE
AT&T System V
1991
Linux 0.1
Comics about Free and Open Source Software
AIX (IBM)
SunOS, Solaris
HPUX (HP)
(Sun Microsystems)
IRIX (SGI)
23
Dongkun Shin, SKKU
```

## Page 24

```text
Ask to ChatGPT
• What is the hardware privilege level?
• How to Schedule Multiple Programs in an Operating
System?
• Why is Virtual Memory Useful in an Operating System?
• How to Implement a Multithreaded Program?
• How to Handle Files and Directories in an Operating
System?
24
Dongkun Shin, SKKU
```
