# 1주차 CA Lecture 01

## 자료 정보

- 과목: 컴퓨터구조개론 - 조형민
- 원본: ../1주차/CA_Lecture_01.pdf
- 주차: 1주차
- 정리일: 2026-08-31
- 방식: PDF에서 페이지별 텍스트를 자동 추출해 원문 보존 중심으로 정리

## Page 1

```text
Introduction to Computer Architecture
Chapter 1
Computer Abstractions and Technology
Hyungmin Cho
Department of Computer Science and Engineering
Sungkyunkwan University
```

## Page 2

```text
Goal of the Course
Understand how computer hardware runs software

Understand what determines the performance of a computer

Chapter 1 — Computer Abstractions and Technology — 2
```

## Page 3

```text
Class Logistics
Grading criteria

Midterm exam: 30%

Final exam: 40%

Programming assignments (3 assignments): 30%

Class attendance

Each student can miss class up to 4 times without any penalty

(No need to provide excuse)
Fail to attend more than 4 classes: 10% deduction

Fail to attend more than ¼ of the total classes: F

If you fail to attend one of the exams, you will get “F”

automatically.
```

## Page 4

```text
Lecture Videos?
All lectures will be recorded & uploaded to i-Campus

Watching the videos is optional

You can use these videos in case you missed the class.

Watching the lecture videos does not substitute for attending the missed

classes!
If you have already attended the class, you are not required to watch the video.

```

## Page 5

```text
Class Communications
Questions related to the lecture contents:

Questions are welcomed anytime during the lecture.

You may ask questions through i-Campus messages, but we recommend

using the open Q&A board rather than private messages.
Homework (programming assignment) questions:

Homework questions are allowed only in the designated “discussion” thread in

i-Campus.
```

## Page 6

```text
만라인 프로젝트 과목
1 ?
If you’re 소프트웨어학과 2022+학번, one of the conditions for your graduation is

fulfilling the “1만라인프로젝트” requirement.
To satisfy the requirement, you need to get “B” grade or higher from at least two

“1만라인프로젝트” courses.
In those courses, one or more class assignments will be based on a large (more

than 10K lines of code) open-source based project.
In this computer architecture class, the last assignment will be the 1만라인프로젝

트 assignment.
If you do not submit this last assignment, you cannot get grade B or higher.

You need to be able to understand/program C++ for the 10K line project

```

## Page 7

```text
Textbook
Computer Organization and Design RISC-V Edition: The Hardware/S

oftware Interface
2nd edition

1st edition is also okay

The “Patterson & Hennessy” book

```

## Page 8

```text
John L. Hennessy / David A. Patterson
RISC processor architecture

Turing award 2017 – “For pioneering a systematic,

quantitative approach to the design and evaluation of
computer architectures with enduring impact on the
microprocessor industry”
```

## Page 9

```text
Class Schedule (Tentative)
Week 1 8/31, 9/2 Computer Abstraction and Technology Chapter 1
Week 2 9/7, 9/9 Performance Chapter 1
Week 3 9/14, 9/16 Instructions: Language of the Computer Chapter 2
Week 4 9/21, 9/23 Instructions: Language of the Computer Chapter 2
Week 5 9/28, 9/30 Instructions: Language of the Computer Chapter 2
Week 6 10/5, 10/7 The Processor Chapter 4.1 – 4.4
Week 7 10/12, 10/14 The Processor Chapter 4.1 – 4.4
Week 8 10/19, 10/21 QnA session / Midterm Exam (4/24, Thu) Chapter 1, 2, 4.1 – 4.4
Week 9 10/26, 10/28 Pipelined Datapath and Control Chapter 4.5, 4.6
Week 10 11/2, 11/4 Pipeline Hazards Chapter 4.7
Week 11 11/9, 11/11 Pipeline Hazards Chapter 4.7, 4.8
Week 12 11/16, 11/18 Pipeline Hazards / Memory Hierarchy Chapter 4.8, 4.9, 5
Week 13 11/23, 11/25 Memory Hierarchy Chapter 5
Week 14 11/30, 12/2 Memory Hierarchy / Arithmetic for Computers Chapter 5, 3
Week 15 12/7, 12/9 Arithmetic for Computers / QnA session Chapter 3
Week 16 12/14, 12/16 Final Exam (12/14, Tue) All covered chapters
```

## Page 10

```text
RISC-V CPU
Modern x86-based computers are very complicated

Complex instruction set

This course is based on RISC-V CPUs

Open standard

Many open-source & commercial implementations exist

Simple (reduced) instruction set

```

## Page 11

```text
RISC-V CPU
I don’t have a RISC-V computer! How can we test the RISC-V software?

Simulators are available!

Why not ARM?

ARM is little more complex than RISC-V

The basic principles are similar

ARM ISA is not free

```

## Page 12

```text
x86?
Processor family from Intel and AMD

Derived from the model numbers of the first few generations:

8086, 80286, 80386, 80486  x86

x86-16: 16-bit processor

x86-32 (aka IA32): 32-bit processor

x86-64 (aka AMD64): 64-bit processor

How intel (and AMD) maintained the market dominance?

Backward compatibility with legacy software

Complexity

```

## Page 13

```text
Programming Assignment (Plan)
Building a RISC-V simulator

A software that mimics the behavior of a RISC-V CPU

Multiple-stage assignment

We will keep adding functionalities to the simulator

If you fail an assignment, it will be hard to complete the next assignment.

Details TBA

```

## Page 14

```text
Classes of Computers
Personal computers

General purpose

Human interaction

Chapter 1 — Computer Abstractions and Technology — 14
```

## Page 15

```text
Classes of Computers
Server computers

High capacity, reliability

Basic building block is not so different from personal computers

Google datacenter
Chapter 1 — Computer Abstractions and Technology — 15
```

## Page 16

```text
Classes of Computers
Supercomputers

Specialized in high computational performance

Scientific experiments, weather forecast, etc.

Rank #1 supercomputer in the world (as of Aug. 2026)

TOP 500 list (top500.org)

“LineShine” @ National Supercomputing Centre in Shenzhen

45,360 LX2 CPUs

Each LX2 CPU has 304 ARMv9 Cores (total 13,789,440 cores)

Performance

2,198,400,000,000,000,000 calculations per second (2.1984 Exa FLOPS)

Compared to a normal PC (Intel i5): 21,984,000×

Chapter 1 — Computer Abstractions and Technology — 16
```

## Page 17

```text
Classes of Computers
Embedded computers

Operates as a part of the other systems (automobile, home appliances)

Significant restrictions in size, power consumption, performance, etc…

8bit or 16bit CPU
Simple operating system
1~2KB RAM
EEPROM: Storage
USIM
[www.extremetech.com] Chapter 1 — Computer Abstractions and Technology — 17
```

## Page 18

```text
The PostPC Era
Personal Mobile Device (PMD)

Battery operated

Connects to the Internet

Hundreds of dollars

Smart phones, tablets, electronic glasses

Cloud computing

Information processing & storage is not limited in specific hardware

Processing with the many computer systems over the network

Chapter 1 — Computer Abstractions and Technology — 18
```

## Page 19

```text
Cloud Computing Examples
1. Store photos taken from mobile device in cloud storage
2. Request complex computations in powerful cloud server
Request
Result
3. Use cloud-based virtual machines instead of managing your own servers
Chapter 1 — Computer Abstractions and Technology — 19
```

## Page 20

```text
Seven Great Ideas in Computer Architecture
Use abstraction to simplify design

Make the common case fast

Performance via parallelism

Performance via pipelining

Performance via prediction

Hierarchy of memories

Dependability via redundancy

Chapter 1 — Computer Abstractions and Technology — 20
```

## Page 21

```text
1. Use Abstraction to Simplify Design
Change the eyebrow angle
Close the eye rid by 50%
Close the mouth
Bend the lips
Change to the sad face
....
...
Modern computer designs are very complex

Abstraction: Hide the lower-level details to simply the model

```

## Page 22

```text
Abstractions in Computer System
Application
Programming language
Machine code
Processor architecture
Abstractions
Microarchitecture
Circuit
Semiconductor (transistors)
Chapter 1 — Computer Abstractions and Technology — 22
```

## Page 23

```text
2. Make the Common Case Fast
Make the common case fast

vs.
Optimize the rare case
Common case is often simpler than the rare case.

```

## Page 24

```text
3. Performance via Parallelism
time
time
```

## Page 25

```text
4. Performance via Pipelining
```

## Page 26

```text
5. Performance via Prediction
Which task should I do?
A or B?
Do B Doing B Done
I think I should do A
Doing A Done
You were right.
```

## Page 27

```text
5. Performance via Prediction
Which task should I do?
A or B?
Do B Doing B Done
I think I should do A
Doing A Oops. Canceling A. Doing B Done
Nope! You should
have done B
```

## Page 28

```text
6. Hierarchy of Memories
Small & Fast Memory
large & Slow Memory
Rarely Accessed Data
Frequently Accessed Data
“Large and Fast Memory”
```

## Page 29

```text
7. Dependability via Redundancy
Result: A Result: B Result: B Result: A Result: B
≠
Something is wrong Something is wrong, but the result is probably B
```

## Page 30

```text
Hardware Abstractions
Instruction set architecture (ISA)

How software abstracts hardware

The hardware/software interface

CPU instructions

x86 (Intel), ARM, SPARC, etc…

Application binary interface (ABI)

The ISA plus system software (OS) interface

e.g., x86 Windows application vs. ARM iOS application

Chapter 1 — Computer Abstractions and Technology — 30
```

## Page 31

```text
ISAs
x86

Used in PC and servers

POWER, PowerPC

Used in Mac, but now it is replaced by Intel Processor

SPARC

Workstations & database servers (Sun microsystems is acquired by Oracle)

ARM

Most popular in embedded world

Designed by ARM, but implemented by many manufactures (Apple, Samsung, etc…)

RISC-V

Similar to ARM, MIPS

Open ISA (no license fees), Many open-source implementations

```

## Page 32

```text
Levels of Program Code
High-level language

Level of abstraction closer to problem domain

Productivity

Portability

Assembly language

Instructions is human-readable format

Machine code

How the instructions are stored in hardware

Chapter 1 — Computer Abstractions and Technology — 32
```

## Page 33

```text
Inside the Processor (CPU)
Datapath: Data moves between the components

Control: Manages how the components process the given data

Registers: Temporary data storage inside the CPU

Cache memory:

DRAM Memory: Main data storage during program execution

Frequently accessed data is brought into the CPU memory

Chapter 1 — Computer Abstractions and Technology — 33
```

## Page 34

```text
Inside the Processor (CPU)
Registers
3
+ 10
7
Control
Cache (16KB)
CPU
DRAM (4GB)
Chapter 1 — Computer Abstractions and Technology — 34
```

## Page 35

```text
Processor Architecture
Chapter 1 — Computer Abstractions and Technology — 35
```

## Page 36

```text
Inside the Processor
Intel i7 “Ivy bridge”

[anandtech.com]
Chapter 1 — Computer Abstractions and Technology — 36
```

## Page 37

```text
Inside the Processor
Apple A12X

[wikichip.org]
Chapter 1 — Computer Abstractions and Technology — 37
```
