# 알고리즘개론 공지

<!-- week12-notices-20261116:start -->
## 12주차 공지 확인 — 2026-11-16

11/9 이후 게시된 새 공지는 없다. 기존 공지는 아래에 보존하며 주차별 최신 자료·마감은 [12주차](./12주차/study.md)에서 본다.
<!-- week12-notices-20261116:end -->

## Merge Sort: Scores and Grading Criteria

- 출처: [iCampus 원문](https://canvas.skku.edu/courses/76112/discussion_topics/655085)
- 작성자: 김승호 TA. 사용자 제공 전문을 2026-10-28 저장소에 반영했으며 원문 게시일은 이 보관일과 구분한다.
- 총점 5점: hidden tests 3점, 보고서·성능 분석 1점, loop invariant 1점.
- 출력은 바이트 단위로 일치해야 하며, 시간·메모리·ANSI C 조건이 있다.
- AI check의 O 표기와 실제 추가 감점·조치는 구분한다. 공지는 정책에 따라 점수에 영향을 줄 수 있다고 안내하며 개인별 최종 조치는 제시하지 않았다.
- 채점 문의: kimsho98@naver.com, 10/30까지. 본인 점수는 채점표·LMS에서 별도 확인한다.

### 사용자 제공 원문

Hello everyone,

Scores for the Merge Sort assignment are now available. Please check the attached file for your score and test cases. The grading criteria are described below.

**Grading Criteria (Total: 5 points)**

**1. Hidden test cases (3 points)**

- Case 1 (0.3 pts): n = 1,000, stability check
- Case 2 (0.3 pts): n = 10,000, many duplicate keys
- Case 3 (0.5 pts): key is the last of 50 attributes, many empty values
- Case 4 (0.5 pts): key is the first attribute, all values at the 15-character maximum
- Case 5 (0.7 pts): n = 1,000,000
- Case 6 (0.7 pts): n = 500,000, many duplicate keys

A case is correct only if your hw1_output.txt is byte-for-byte identical to the expected output. Differences in whitespace or blank lines count as wrong. Your program must read hw1_input.txt and write hw1_output.txt in the working directory. Other file names produce no output.

Time limits: 1 second for Cases 1–2, 2 seconds for Cases 3–4, and 5 seconds for Cases 5–6. Each case was run three times. To pass, every run had to be correct and the median running time had to be within the limit.

Test environment: Ubuntu Linux with gcc, limited to 1 CPU core and 4 GB of memory.

**2. Report and performance analysis (1 point)**

Explanation of how your code works and an analysis of its time and space complexity.

- 1: both are present and adequate
- 0.5: one is missing or very brief
- 0: no report

**3. Loop invariants (1 point)**

For each principal loop, including the merge loop, state the invariant and argue initialization, maintenance, and termination.

- 1: complete
- 0.5: the invariant is stated but the arguments are incomplete, or the merge loop is missing
- 0: no invariants

**Deductions**

- ANSI C violation (−0.25): applied once if your code produces errors under `gcc -ansi -pedantic`. Warnings are not penalized. Your code was then compiled with plain gcc so the test cases could still be graded.
- Not using merge sort on a linked list: total score 0
- Compilation failure with plain gcc: 0 for the test cases
- Missing source code or report: 0 for that part
- Late submissions are marked on the score board and handled according to the course policy.

**About the “AI check” column**

The assignment included a mechanism to detect indiscriminate AI use. Submissions where such use was confirmed are marked with 'O'. **Please note that this may affect your score according to the course policy.**

If you have any questions about your score, please contact me at [kimsho98@naver.com](mailto:kimsho98@naver.com) by October 30.

Best regards,

Seungho Kim

## 2026-10-14 업데이트: 중간고사 시간·강의실·범위 확정

### 핵심 체크

- 사용자 전달일: 2026-10-15. 공지의 최종 업데이트 표시는 October 14th다.
- 시험: **2026-10-19(월) 10:30~11:45, 75분**. 10:30 시작 전 미리 도착한다.
- 장소: 산학협력센터 6층. 학번 끝자리 짝수는 **85602**, 홀수는 **85613**. 송준혁 학번 2024315329의 끝자리는 9이므로 **85613**이다.
- 범위: **7주차까지 모든 내용**, 대면 수업에서 다루지 않은 온라인 내용과 퀴즈도 포함한다.
- 제외: **Dynamic Programming 내용과 DP 온라인 퀴즈(SRTBOT, Fibonacci, bowling)**.
- **Counting/Radix/Linear Sorting은 포함**. 10/05 수업 당시 구두 안내에서 변경되었다고 공식 공지가 명시한다.
- 퀴즈 답안 온라인 녹화 업로드 예정이라고 안내했다. 실제 업로드 여부는 이 공지만으로 확인하지 않는다.
- 원문의 홀수 예시 목록에는 7 대신 6이 들어간 오기가 있다. 짝수/홀수 배정 원칙과 끝자리 9의 배정은 명확하다. 원문을 임의 수정하지 않았다.

### 준비 체크

- 10/19 10:30 전에 85613 입실.
- DP를 제외한 교수 자료·온라인 강의·퀴즈를 모두 복습.
- 퀴즈 답안 녹화가 올라왔는지 별도로 확인.
- DP 시험 제외를 수강·퀴즈 제출 면제로 해석하지 않음.

### 원문

출처: TA 장희준, `Notice for Mid-term Exam (!!!!!!!!Must check before mid-term Exam, updated on October 12th!!!!!!!!) 14th update`

원문 링크: https://canvas.skku.edu/courses/76112/discussion_topics/647728

Hello, this is Algorithm TA Heejun Jang.

**Mid-term Exam will be held on 19th October at Corporate Collaboration Center 6th floor, 85602 & 85613**
**(10/19, 산학협력센터 6층 85602호 & 85613호)**

**Students who have an even student ID(ends with 0, 2, 4, 6,8) go to 85602,**
**and the other students who have an odd student ID (ends with 1, 3, 5, 6, 9) go to 85613.**

**========================= October 12th Update =========================**

**An important notice for the mid-term exam for mid-term scope and time.**

**Midterm exam time: 19th October 10:30 ~ 11:45 (same as lecture time). The exam will start at 10:30, so you have to come to the place early.**

**Midterm exam scope (check the notice that the professor uploaded on iCampus): The midterm exam covers all material through Week 7, excluding Dynamic Programming. (It has changed from what was discussed in the previous lecture(10/05), so do not be confused; only Dynamic Programming content is excluded.)**
**All online lecture content is included, even if it was not covered in class.**
**All quizzes are also included, and an online recording of the quiz answers will be uploaded.**

**===================================================================**

**========================= October 14th Update =========================**

**Midterm exam scope (check the notice that the professor uploaded on iCampus): The midterm exam covers all material through Week 7, excluding Dynamic Programming. (It has changed from what was discussed in the previous lecture(10/05), so do not be confused; only Dynamic Programming content is excluded.) + Online quiz for Dynamic Programming, which covers SRTBOT, Fibonacci, and bowling, is also excluded (not covered in the midterm exam).**

**===================================================================**

**If you have any questions, please send an email to the TA & Professor**

**Best regards.**

**Heejun Jang.**

## 2026-09-30 중간고사 일정 (사용자 전달)

- 아래는 9/30 당시 기록이며, 현재 확정 정보는 위 10/14 업데이트를 따른다.
- 중간고사: 2026-10-19(월), 산학관 85613.
- 날짜는 8주차 월요일을 달력으로 환산했다. 시험 시각과 범위는 아직 확인되지 않았다.
- 사용자가 새 공지가 올라온 것을 확인해 전달했으며, 공지 원문은 아직 저장되지 않았다.

## 2026-08-31 평가 비율 구두 안내

### 핵심 체크

- PA: 20%
- Midterm: 30%
- Final: 30%
- Quiz: 15%
- Attendance: 5%

### 원문

출처: 수업 중 구두 안내

알고리즘 과제 PA 20퍼. 미드텀, 기말 각 30. 퀴즈 15. 출석 5.

아직 관련 파일은 올라오지 않음.


## 2026-09-22 수업 일정 및 수업계획서 공지

### 핵심 체크

| 날짜 | 수업 여부 | 비고 |
| --- | --- | --- |
| 2026-09-28 | 수업 O | 정상 강의 예정 |
| 2026-10-05 | 수업 O | 휴일이지만 강의 예정. 방식/시간은 추후 공지 확인 |
| 2026-10-12 | 수업 X | 강의 없음 |

- 일정은 교수님 일정에 따라 유동적이며 조정될 수 있다.
- 수업계획서는 1주차 `lec00_overview` 슬라이드를 기준으로 확인한다.
- 수업 문의는 iCampus 메시지가 아니라 안내된 TA/교수 이메일 절차를 따른다.

### 원문 요지

출처: TA 장희준, `Notice for Algorithm Lecture Schedule for 09/28, 10/05, 10/12 and Syllabus`

9월 28일과 10월 5일에는 강의를 진행하고, 10월 12일에는 강의가 없다. 강의 일정은 교수 일정에 따라 변경될 수 있으며, 수업계획서는 1주차 overview 슬라이드에서 확인한다.

<!-- canvas-notice-656107-20261109:start -->
## [Updated][Programming] Merge Sort: Scores and Grading Criteria (11/9 API 원문 확인)

- 게시: 2026-10-27 13:14:12
- 출처: https://canvas.skku.edu/courses/76112/discussion_topics/656107
- 본문 전체를 API HTML에서 텍스트로 옮겼다. 문단·줄바꿈을 정돈했으며 원본 HTML은 로컬 API 스냅샷에 보관한다.

### 원문

This is a re-announcement regarding the merge sort assignment grades. Scores have been updated to reflect the quiz results, so please check your updated score. The grading criteria for the assignment itself remain the same as in the previous announcement.

 

1. Policy on AI Use

As the professor mentioned in the first class and as stated in the assignment PDF, the use of AI is permitted. However, you must follow these rules:

- If you used AI, you must disclose it.

- Most importantly, you must fully understand what the AI generated. You are responsible for the code you submit.

 

2. Purpose of the October 26 Quiz

There are many ways to implement merge sort. After reviewing the submissions, we found that only 8 students implemented it the way it was taught in class, while the remaining 57 used an approach that was not covered in class. To check how well each student understands their own submitted code, we gave a quiz on October 26.

- The quiz is used only to check understanding of your own submission.

- A score of 3 or higher out of 5 is taken to mean that you understand the code you submitted.

 

3. How Final Scores Were Determined

- The 8 students who implemented merge sort as taught in class: Your originally graded score was kept as is, regardless of the quiz score.

- The other 57 students: Your originally graded score was kept only if your quiz score was 3 or higher. If your quiz score was below 3, only the document score was given.

 

If you have any questions, please contact by email(kimsho98@naver.com).

 

Best regards,

Seungho Kim

 

 

merge sort 점수 과제에 대해 다시 안내합니다. 퀴즈 결과를 반영하여 점수가 업데이트되었으니, 각자 업데이트된 점수를 확인해 주시기 바랍니다. 과제 자체의 평가 방식은 이전 공지와 동일합니다.

 

1. AI 사용에 대한 안내

첫 수업 시간에 교수님께서 말씀하셨고 과제 안내 PDF에도 명시되어 있듯이,  AI 사용은 허용됩니다. 다만 다음 사항을 반드시 지켜야 합니다.

- AI를 사용했다면 사용 사실을 명시해야 합니다.

- 무엇보다 중요한 것은 AI가 생성한 결과물을 충분히 이해하는 것입니다. 제출한 코드에 대한 책임은 제출자 본인에게 있습니다.

 

2. 10월 26일 퀴즈의 목적

merge sort는 구현 방식이 다양합니다. 제출물을 확인한 결과, 수업에서 다룬 방식대로 구현한 학생은 8명뿐이었고, 나머지 57명은 수업에서 다루지 않은 방식으로 구현했습니다. 이에 따라 각자 제출한 코드를 얼마나 이해하고 있는지 확인하기 위해 10월 26일에 퀴즈를 실시했습니다.

- 이 퀴즈는 제출물에 대한 이해도를 확인하는 용도로만 사용됩니다.

- 5점 만점에 3점 이상인 경우 제출한 코드를 이해한 것으로 판단합니다.

 

3. 과제 최종 점수 반영 방식

- 수업에서 다룬 방식대로 구현한 8명: 퀴즈 점수와 관계없이 원래 채점된 점수를 그대로 부여했습니다.

- 그 외 57명: 퀴즈 점수가 3점 이상인 경우에만 원래 채점된 점수를 그대로 부여했습니다. 퀴즈 점수가 3점 미만인 경우에는 문서 점수만 부여했습니다.

 

문의 사항이 있으면 메일로 연락바랍니다.

첨부파일: [scoreboard.xlsx](https://canvas.skku.edu/courses/76112/files/11974125)
<!-- canvas-notice-656107-20261109:end -->
