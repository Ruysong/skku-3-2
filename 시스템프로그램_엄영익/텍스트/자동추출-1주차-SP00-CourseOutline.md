# 1주차 SP00 Course Outline

## 자료 정보

- 과목: 시스템프로그램 - 엄영익
- 원본: ../1주차/SP00(CourseOutline)-(2026-2)-(SWE2001-41).pdf
- 주차: 1주차
- 정리일: 2026-08-31
- 방식: PDF 내부 텍스트 레이어 자동 추출본. OCR/눈검수 완료본 아님

## 주의

이 파일은 PDF 내부 텍스트 레이어를 자동 추출한 1차본입니다. 이미지로 들어간 글자, 도식 속 텍스트, 수식, 표 일부는 누락될 수 있습니다. 완전본은 원본 PDF를 보며 OCR/눈검수 후 별도로 확정합니다.

## Page 1

```text
Systems Programming
[2026-2]
Young Ik Eom (yieom@skku.edu)
Dept. of Computer Science and Engineering
Sungkyunkwan University
```

## Page 2

```text
Introduction

Basic information
• SWE2001-41 (2026-2nd Semester)
• 16:30~17:45 (Tue), 15:00~16:15 (Thu)
• Lecture room #26310
• iCampus (https://icampus.skku.edu)

Flipped class
• Online class: about 150 minutes per week
• Offline class: about 75 minutes per week
– On Thursday 15:00-16:15

Instructor
• Young Ik Eom
Systems Programming, by Young Ik Eom, SKKU 2
```

## Page 3

```text
Course Outline (1)

Goal
Learning what is going on
under the hood of a computer system
and
Becoming the power programmer
Systems Programming, by Young Ik Eom, SKKU 3
```

## Page 4

```text
Course Outline (2)

Computer systems
Systems Programming, by Young Ik Eom, SKKU 4
```

## Page 5

```text
Course Outline (3)

Computer systems internals
Software
Applications
Operating System
Architecture
I/O Devices
Hardware CPU Mem
Systems Programming, by Young Ik Eom, SKKU 5
```

## Page 6

```text
Course Outline (4)

Levels of abstraction
Application programs
Data structures & algorithms
Programming languages & compilers
Operating system
Architecture
Microarchitecture
Hardware description languages
Digital logic
VLSI layout
Processing, Fabrication
Chemistry, Physics
Systems Programming, by Young Ik Eom, SKKU 6
System
programming
```

## Page 7

```text
Course Plan

Topics
• (C01) A tour on computer systems
• (C02) Representing and manipulating information
• (C03) Machine/Assembly-level representation of programs
• (C04) Processor architecture
• (C05) Optimizing program performance
• (C06) Memory hierarchy
• (C07) Linking
• Etc
Systems Programming, by Young Ik Eom, SKKU 7
```

## Page 8

```text
iCampus

iCampus
• 수업계획서
• 공지
• 문의게시판
• 수업자료실
• 강의콘텐츠
• 과제 및 평가
• 시험 및 설문
• [메시지함]
• Etc
Systems Programming, by Young Ik Eom, SKKU 8
```

## Page 9

```text
Prerequisites

C programming !!!

Data structures !!

Basic knowledge on UNIX/Linux systems
Systems Programming, by Young Ik Eom, SKKU 9
```

## 주의

이 파일은 PDF 내부 텍스트 레이어를 자동 추출한 1차본입니다. 이미지로 들어간 글자, 도식 속 텍스트, 수식, 표 일부는 누락될 수 있습니다. 완전본은 원본 PDF를 보며 OCR/눈검수 후 별도로 확정합니다.

## Page 10

```text
Textbook

Computer Systems:
A Programmer’s Perspective
(3-rd edition, Global edition)
• Randal E. Bryant and
David R. O’Hallaron,
Pearson, 2016.
• Authors’ homepage:
http://csapp.cs.cmu.edu
Systems Programming, by Young Ik Eom, SKKU 10
```

## 주의

이 파일은 PDF 내부 텍스트 레이어를 자동 추출한 1차본입니다. 이미지로 들어간 글자, 도식 속 텍스트, 수식, 표 일부는 누락될 수 있습니다. 완전본은 원본 PDF를 보며 OCR/눈검수 후 별도로 확정합니다.

## Page 11

```text
References

C Programming
• The C Programming Language,
(2-nd edition)
B. Kernighan and D. Ritchie,
Prentice Hall, 1988.
Systems Programming, by Young Ik Eom, SKKU 11
```

## 주의

이 파일은 PDF 내부 텍스트 레이어를 자동 추출한 1차본입니다. 이미지로 들어간 글자, 도식 속 텍스트, 수식, 표 일부는 누락될 수 있습니다. 완전본은 원본 PDF를 보며 OCR/눈검수 후 별도로 확정합니다.

## Page 12

```text
Class Policies (1)

Grading Policy (subject to change)
• Class attendance (5%)
• Projects/Assignments/Quizzes (25%)
• Mid-Exam (35%)
• Final-Exam (35%)
• If you miss one or both of exams,
you will fail this course
• You should attend offline or online classes each week
and take the quiz when it is imposed
Systems Programming, by Young Ik Eom, SKKU 12
```

## 주의

이 파일은 PDF 내부 텍스트 레이어를 자동 추출한 1차본입니다. 이미지로 들어간 글자, 도식 속 텍스트, 수식, 표 일부는 누락될 수 있습니다. 완전본은 원본 PDF를 보며 OCR/눈검수 후 별도로 확정합니다.

## Page 13

```text
Class Policies (2)

Project Policy
• You will work on each project alone or on team-basis
• Only the assignments submitted before the deadline
will receive full credit
• 20% of the credit will be deducted for every single
day delay
Systems Programming, by Young Ik Eom, SKKU 13
```

## 주의

이 파일은 PDF 내부 텍스트 레이어를 자동 추출한 1차본입니다. 이미지로 들어간 글자, 도식 속 텍스트, 수식, 표 일부는 누락될 수 있습니다. 완전본은 원본 PDF를 보며 OCR/눈검수 후 별도로 확정합니다.

## Page 14

```text
Class Policies (3)

Cheating Policy
• What is cheating?
– Sharing code or document
by copying, retyping, looking at, or supplying a file
– Coaching or helping your friend to write a code or document line by line
– Taking code or document from anywhere including WWW
– Getting help from any AI engine
• What is NOT cheating?
– Explaining how to use systems or tools
– Helping others with high-level design issues
• Penalty for cheating:
– You will fail this course
Systems Programming, by Young Ik Eom, SKKU 14
```

## 주의

이 파일은 PDF 내부 텍스트 레이어를 자동 추출한 1차본입니다. 이미지로 들어간 글자, 도식 속 텍스트, 수식, 표 일부는 누락될 수 있습니다. 완전본은 원본 PDF를 보며 OCR/눈검수 후 별도로 확정합니다.

## Page 15

```text
Any Questions?
Systems Programming, by Young Ik Eom, SKKU 15
```

