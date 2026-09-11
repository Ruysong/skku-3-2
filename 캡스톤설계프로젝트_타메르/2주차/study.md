# 캡스톤설계프로젝트 - 타메르 2주차 학습지도

## 한줄 결론

캡스톤은 단순히 기능을 많이 만드는 과목이 아니라, **실제 문제를 정의하고, 기존 해결책을 조사하고, 제약 안에서 해결책을 설계해 프로토타입으로 검증한 뒤 발표와 보고서로 설득하는 팀 프로젝트**다. 이번 1강과 Proposal submission 안내를 기준으로 보면 가장 가까운 목표는 2026-09-20 23:59까지 팀당 한 명이 제안서를 제출하는 것이다. 따라서 지금은 완벽한 아이디어를 혼자 확정하는 시점이 아니라, 3~4명의 팀을 만들고 문제·사용자·해결 방법·역할·학기 일정이 서로 연결된 제안서를 만드는 시점이다.

## 상태

- 2026-09-11 기준 `CapstoneDesign-Lecture1.pdf`와 `Capstone Design-Proposal-Template2026.docx`를 반영했다.
- Proposal submission은 2026-09-20 23:59 마감의 10점 팀 과제이며, 팀당 한 명만 파일 업로드한다.
- 템플릿 본문은 팀 인원을 최소 3명, 최대 4명으로 적고 있다. 첫 공지의 4인 팀 안내와 함께 보면 4인을 목표로 하되 3~4명 범위에서 구성하면 된다.
- 1강 자료의 프로젝트 일정, 평가, 좋은 프로젝트 기준, 제안서에 필요한 설계 흐름을 이 문서에 정리했다.

## 원본 자료

| 자료 | 역할 | 핵심 |
| --- | --- | --- |
| `CapstoneDesign-Lecture1.pdf` | 1강 course overview | 수업 목표, 프로젝트 timeline, 평가, 분야, engineering design process |
| `../Capstone Design-Proposal-Template2026.docx` | 팀 제안서 양식 | 제안서 항목, 팀원 역할표, W3 이후 일정표 |
| iCampus `Proposal submission` | 실제 제출 과제 안내 | 9/20 23:59 마감, 팀당 1회 업로드, 문제/해결책/역할/일정/결과 요구 |

---

# 1. 이 과목에서 만드는 것

강의 첫 장의 제목은 `Capstone Design Overview`이고, 수업을 관통하는 단어는 Innovation과 Collaboration이다. 이 과목의 산출물은 아이디어 메모나 발표 슬라이드 하나가 아니다. 팀이 현실의 문제를 정하고, 해결을 위한 시스템 또는 프로세스를 설계하고, 실제로 동작하는 prototype을 구현·테스트하며, 그 과정과 결과를 문서와 발표로 보여주는 것이다.

강의는 이 과목을 전통적인 이론 강의와 구분한다. 이론 강의가 passive learning이라면, laboratory work와 homework를 통해 스스로 만들며 배우는 방식은 active learning이다. 캡스톤의 핵심은 후자다. 따라서 결과물을 만들지 못하면 좋은 기획 문장만으로 충분하지 않고, 반대로 구현만 하고 문제·사용자·검증을 설명하지 못해도 완성된 캡스톤으로 보이지 않는다. 문제 정의, 설계, 구현, 테스트, 문서화, 의사소통이 한 덩어리다.

강의에서 제시한 수업 목표는 세 갈래다. 첫째, senior student가 engineering design experience를 얻는 것. 둘째, 주어진 engineering problem을 이해하고 필요하면 다시 정의한 뒤 conceptual design을 만드는 것. 셋째, 결과와 판단 근거를 효과적으로 전달하는 능력을 기르는 것이다. 이때 physical project work와 process documentation도 분명히 언급된다. 즉 개발 이력, 조사한 근거, 역할 분담, 설계 변경 이유를 남기는 일이 평가와 직접 연결된다.

## 1.1 이 과목에서 기르는 역량

강의는 공학 훈련의 구성 요소로 creative thinking, project management skills, teamwork ability, research ability, lifelong learning habit and skills, effective communication skills, analytical skills, system approach, technical perfection을 열거한다. 이것은 평가 항목을 해석하는 힌트이기도 하다.

- Creative thinking: 이미 있는 서비스를 얕게 복제하는 대신, 실제 불편을 새롭게 정의하고 해결의 차별점을 만드는 능력이다.
- Project management: 한 학기 안에 무엇을 언제 만들지 정하고, 기능을 나누고, 위험을 관리하는 능력이다.
- Teamwork: 각자 할 일을 적는 것에서 끝나지 않고, 서로의 작업이 하나의 prototype과 발표로 합쳐지게 만드는 능력이다.
- Research: 기존 제품·논문·서비스·시장 상황을 조사해 왜 이 문제가 아직 충분히 해결되지 않았는지 설명하는 능력이다.
- Communication: 사용자, 팀원, 교수님에게 문제와 해결책을 설득력 있게 말하고 문서화하는 능력이다.
- System approach: 기능 한 조각이 아니라 입력, 처리, 저장, 사용자 경험, 제약, 테스트를 연결해 시스템 전체를 보는 능력이다.

---

# 2. 학기 흐름과 당장 맞닥뜨린 마감

## 2.1 강의자료의 큰 timeline

1강 slide는 프로젝트를 다음 흐름으로 제시한다.

```text
Weeks 1-3: 팀 선택 -> 주제 선택 -> 해결할 문제 파악
Week 4: 1차 proposal presentation (5-7분) + instructor approval
Weeks 5-8: 구현과 진행 보고를 중심으로 프로젝트 전개
Week 9: midterm / 2차 presentation (5-10분)
Weeks 9-12: 중간 피드백을 반영한 구현·검증 지속
Weeks 15-16: final report + final presentation
```

슬라이드의 주차 표기는 큰 진행 구간을 보여준다. 실제 iCampus 과제는 이보다 더 직접적인 행동 마감을 제공한다. `Proposal submission`은 **2026-09-20 일요일 23:59**까지다. 이 과제를 낸 뒤 교수님이 다음 주에 각 팀의 form 내용을 확인한다고 공지했다. 즉 주제는 제출 뒤에도 구현 중 조정될 수 있지만, 지금 제안서에는 적어도 팀·문제·대략의 architecture·역할·주차별 계획이 있어야 한다.

## 2.2 Proposal submission의 제출 규칙

- 과제는 10점이다.
- 파일 업로드형이다.
- 팀 과제이므로 **한 팀에서 한 명만** 업로드한다.
- 모든 팀원이 중복 제출할 필요는 없으며, 대표 제출자를 미리 정해야 한다.
- 제출 후 다음 주에 교수님이 form에 적힌 세부사항을 각 팀과 확인한다.

여기서 대표 제출자는 단순 업로더가 아니다. 팀명, 문서 버전, 최종 파일명, 제출 완료 여부를 책임지고 확인하는 역할이 된다. 제안서 본문에도 팀장 또는 대표학생과 같은 실무 역할이 나중에 지원금·정산 문서와 연결될 수 있으므로, 초기에 역할을 너무 애매하게 두지 않는 편이 좋다.

## 2.3 평가 구조

강의자료의 grading policy는 다음과 같다.

| 항목 | 비율 | 수업이 보는 것 |
| --- | ---: | --- |
| Project Management and Teamwork | 15% | 격주 보고, 역할 이행, 진행 관리 |
| Presentations | 30% | 짧은 발표 2회와 최종 발표 |
| Final Report | 30% | 문제, 조사, 설계, 구현, 결과를 문서로 설명하는 능력 |
| Project Testing and Demonstrations | 25% | 실제 prototype의 동작, 검증, 데모 |
| Intermediate reports | 10% bonus 표기 | 중간 보고서 관련 보너스 여부는 추후 운영 확인 필요 |

이 구조에서 특히 중요한 점은 구현과 데모가 25%이지만, 발표와 최종 보고서가 합쳐 60%라는 것이다. 그러므로 "코드가 동작한다"만으로는 부족하다. 문제의 필요성, 기존 해결책과의 차이, 설계 선택, 제약, 테스트 결과를 남겨야 한다. 반대로 문서가 아무리 길어도 테스트 가능한 prototype이 없으면 project testing과 demonstration에서 약해진다.

---

# 3. 팀 구성: 지금 결정해야 하는 기준

첫 공지는 4인 팀을 말했고, 현재 Proposal Template의 `Number of Students`는 최소 3명·최대 4명이라고 명시한다. 과목 설명의 4~5명 표현보다, 실제 제출 양식의 3~4명 제한이 현재 기준으로 더 직접적이다. 따라서 현실적인 목표는 **역할이 분명한 4인 팀**이다. 3명이 된다면 한 사람이 개발·문서·발표를 겸해야 할 가능성이 커진다.

팀원을 구할 때 주제 취향만 볼 필요는 없다. proposal에서 architecture, activities, technical details, roles, workflow까지 적어야 하므로 다음 질문에 답할 수 있는 사람들이면 좋다.

1. 한 학기 동안 꾸준히 만날 수 있는가?
2. 구현을 맡을 사람, 화면·사용자 흐름을 정리할 사람, 조사·문서·발표를 책임질 사람이 균형 있게 있는가?
3. 의견 충돌이 생겼을 때 범위를 줄이고 기능을 우선순위화할 수 있는가?
4. 금요일 수업과 중간·최종 발표에 함께 대응할 수 있는가?
5. 어려운 기술이 필요할 때 누가 학습을 맡고, 그 위험을 일정에 반영할 수 있는가?

좋은 팀은 모두 같은 기술을 아는 팀이 아니라, 필요한 일을 나누되 서로의 작업 결과를 연결할 수 있는 팀이다. 예를 들어 software 중심 프로젝트라면 기획·사용자 시나리오, frontend 또는 client, backend·data·AI/algorithm, testing·deployment·documentation이라는 축을 나눌 수 있다. hardware가 섞이면 회로·센서·embedded 구현과 application 또는 dashboard를 연결하는 역할이 필요하다. 다만 역할표는 처음부터 고정 계약이 아니라 `Initial Project Role`이다. 구현이 진행되면 역할은 바뀔 수 있지만, proposal 시점에는 책임의 출발점을 보여줘야 한다.

---

# 4. 좋은 주제를 고르는 기준

## 4.1 project field는 넓다

강의는 hardware, embedded systems, Raspberry Pi, Arduino, integrated circuits, software, service, security problem, artificial intelligence, health, education, multidisciplinary project를 예시로 든다. 하지만 분야 이름 자체가 주제가 되지는 않는다. "AI 프로젝트", "보안 프로젝트", "앱 만들기"는 아직 문제 정의가 아니다. 반드시 현실의 누가 어떤 상황에서 무엇을 어려워하는지로 내려와야 한다.

강의가 명시한 공통 조건은 `real life problem related to the industry`다. 즉 사용자 또는 현장 맥락이 있고, 현재 방식의 불편·비용·오류·안전 문제를 설명할 수 있어야 한다. 산업과 연결된다는 말은 꼭 기업과 계약해야 한다는 뜻이 아니라, 실제 세계에서 일어나는 문제이며 누군가가 쓸 이유가 있어야 한다는 의미로 이해하면 된다.

## 4.2 좋은 student project의 조건

강의는 좋은 프로젝트가 다음을 만족한다고 말한다.

- Creative/Innovative: 시판 부품이나 기존 서비스를 단순 조립한 결과가 아니어야 한다.
- Prototype: 실제로 만들고, 사용하거나 테스트하며 배워야 한다.
- Revisions: prototype 결과에 따라 design change와 revision이 일어나야 한다.
- Sufficient scope: senior design에 맞는 충분한 범위가 있어야 한다.
- Capability: 팀이 필요한 기술을 이미 갖추었거나, 학기 중 배울 준비가 되어 있어야 한다.
- Progress reporting: 프로젝트 각 단계에서 진행을 보고해야 한다.

여기서 "혁신"을 거대한 발명으로 오해하면 범위가 망가진다. 기존 해결책이 있음에도 특정 사용자·환경·제약에서는 불편하다는 점을 발견하고, 그 차이를 해결하는 작은 시스템을 만드는 것도 충분히 창의적인 방향이 될 수 있다. 반대로 기존 API, 오픈소스, 센서를 쓴다는 사실 자체가 문제가 아니라, 그것들을 연결한 이유와 사용자가 얻는 개선이 설명되지 않는 것이 문제다.

## 4.3 주제 검증 질문

후보를 2~3개 떠올렸다면, 각 후보에 대해 다음 질문을 먼저 통과시키면 된다.

```text
누가 사용자/고객인가?
그 사람이 실제로 겪는 문제는 무엇인가?
기존에는 어떻게 해결하고 있으며 왜 부족한가?
우리의 해결책은 무엇을 입력받아 어떤 판단/처리를 하고 무엇을 보여주거나 제어하는가?
한 학기 안에 만들 수 있는 최소 데모는 무엇인가?
테스트로 성공 여부를 어떻게 확인할 것인가?
필요한 기술·데이터·장비·비용은 무엇인가?
```

이 질문에 한두 문장이라도 답할 수 있어야 proposal의 Problem Statement와 Proposed Solution이 공허해지지 않는다. 아직 답하기 어렵다면 구현 아이디어를 늘리기보다 문제 조사부터 하는 편이 맞다.

---

# 5. Engineering Design Process와 proposal의 관계

강의는 engineering design을 creative, iterative, often open-ended process라고 정의한다. 부품, 시스템, 프로세스를 구상하고 개발하는 과정이며 engineering, basic skills, mathematical sciences를 통합해야 한다. 또한 설계자는 경제성, 건강·안전, 사회·환경 영향, 관행과 법적 제약을 고려해야 한다.

slide는 design steps를 다음 순서로 제시한다.

```text
1. Identifying a need
2. Defining the problem
3. Conducting research
4. Narrowing the research
5. Analyzing set criteria
6. Develop solution
7. Plan alternative solutions
8. Analyzing possible solutions
9. Making a decision and create prototype
10. Test and improve
11. Presenting the product
12. Communicating and selling the product
```

슬라이드에는 ten-step process라고 적혀 있지만 실제 열거는 presenting과 communicating까지 포함해 더 세분화되어 있다. 중요한 것은 숫자를 외우는 게 아니라, 한 번 정한 아이디어를 바로 구현으로 밀어 넣지 않는 흐름이다. 필요를 찾고, 문제를 정의하고, 기존 해결을 조사하고, 제약과 평가 기준을 세우고, 대안을 비교한 뒤 prototype으로 검증한다. 테스트 결과는 다시 설계 변경으로 돌아간다.

## 5.1 Identifying a Need

need는 단순히 "이런 앱이 있으면 좋겠다"보다 구체적인 불편 또는 결핍이다. 강의는 아래와 같은 문장을 문제 출발점의 예로 든다.

```text
Dealing with ________ is almost impossible. I wish there was a ________ to help.
I need a ________ to get ________ done.
We cannot handle ________ and we wish there is ________ to solve this issue.
```

이 단계에서는 해결책의 기술 이름을 먼저 넣지 않는 것이 좋다. 예를 들어 "LLM을 쓰는 앱"이 아니라 "특정 사용자가 반복적으로 어떤 정보를 찾거나 판단할 때 시간이 너무 오래 걸린다"가 need다. 기술은 그 need를 해결하기 위해 나중에 선택된다.

## 5.2 Defining the Problem

문제 정의의 첫 단계는 client 또는 customer를 파악하는 것이다. 강의는 고객과 충분히 대화하고, 그 사람의 business·process·people을 이해하며, 고객의 관점에 서 보라고 한다. 캡스톤에서 실제 외부 고객이 없다면, 최소한 구체적인 사용자 persona와 사용 상황을 정해야 한다.

좋은 Problem Statement에는 다음이 들어간다.

- 문제를 겪는 사람이 누구인지
- 현재 어떤 과정에서 문제가 생기는지
- 그 문제가 시간, 비용, 오류, 접근성, 안전, 학습, 보안 등에 어떤 영향을 주는지
- 현재 방식이나 기존 솔루션이 왜 충분하지 않은지
- 프로젝트가 풀 문제의 경계가 어디까지인지

문제를 충분히 이해했다고 생각될 때 문장으로 적고, 가능하면 실제 사용자 또는 주변의 해당 상황 경험자에게 읽혀 보는 것이 좋다. 그 과정에서 문제 범위가 너무 넓거나, 사용자가 불명확하거나, 불편이 실제로 크지 않다는 점이 드러날 수 있다.

## 5.3 Conducting Research

강의는 생산적인 engineer가 많은 시간을 정보를 찾고 적용하고 전달하는 데 쓴다고 설명한다. 연구 단계에서 물어야 할 질문은 다음과 같다.

- 이 문제에 대해 이미 무엇이 쓰였는가?
- 시장에 이미 해결책이 있는가?
- 기존 방식에서 무엇이 잘되고 무엇이 잘되지 않는가?
- 누가 현재 솔루션을 만들고 있으며 비용은 얼마인가?
- 사용자가 더 나은 해결책에 비용이나 시간을 들 이유가 있는가?

정보원으로 Internet, local libraries, government documents, organizations, trade journals, vendor catalogs, individual experts 등을 제시한다. 조사 결과는 bibliography로 남겨야 나중에 다시 찾고 보고서에 인용할 수 있다. proposal의 Related Works는 단순 링크 모음이 아니라, "기존 해결책 A와 B가 있는데 각각 이 제약이 있어 우리 팀은 C에 집중한다"라는 설계 근거를 만드는 파트다.

## 5.4 Constraints와 기준

연구를 넓게 한 뒤에는 범위를 좁혀야 한다. 강의는 제약이 비효율적·비용 과다·물리적으로 불가능한 극단적 해결을 제거한다고 말한다. 비교 기준은 appearance, durability, cost 같은 질적 기준도 될 수 있다. software 프로젝트라면 개발 기간, 데이터 접근 가능성, 개인정보 위험, 사용자 수, 배포 환경, 유지보수 부담, 테스트 가능성이 대표적 제약이 된다.

따라서 팀은 제안서 전에 간단한 기준표를 만들면 좋다. 예를 들면 각 후보에 대해 문제의 명확성, 사용자 접근성, 구현 난이도, 데모 가능성, 차별점, 비용, 테스트 방법을 1~5점으로 비교한다. 이것은 교수님에게 보여주기 위한 장식이 아니라, 팀이 무리한 아이디어를 스스로 걸러내는 장치다.

---

# 6. Proposal Template을 실제로 채우는 법

템플릿은 다음 구조를 요구한다.

1. Project Title
2. Project summary - 최대 500단어
3. Problem Statement - 최대 200단어
4. Related works - 분량 제한 없음
5. Proposed Solution - 분량 제한 없음
6. Main functions of the project
7. Time Plan and Roles of Team Members
8. Number of Students - 최소 3명, 최대 4명
9. List of students - 학번, 이름, 연락처 또는 이메일, Initial Project Role
10. 주차별 task/assignment 일정표

## 6.1 Project Summary

요약에는 main objectives, realistic constraints, broadly final results, 그리고 한 학기 내 attainable함을 명확히 쓰라고 되어 있다. 즉 "무엇을 만들겠다"만 쓰지 말고, 왜 가능한 범위인지와 학기 말에 남을 결과물을 함께 말해야 한다.

좋은 요약은 보통 `대상 사용자와 문제 -> 해결 시스템 -> 핵심 기능 -> 제약 -> 학기 말 데모` 순서가 자연스럽다. 여기서 final result는 "완벽한 상용 서비스"가 아니라, 어떤 입력에서 어떤 동작을 보여주는 prototype인지로 구체화해야 한다.

## 6.2 Problem Statement

과제 안내는 다음을 요구한다.

- 어떤 issue를 다루며 왜 중요한가?
- 제안하는 것이 왜 필요한가?
- 과거에 왜 충분히 해결되지 않았는가?
- 누가 이 문제에 이미 작업했는가?

이 부분은 200단어 제한이 있으므로 기술 설명으로 채우기보다, 사용자와 문제의 원인·영향·기존 해결의 공백을 압축해서 써야 한다. Related Works가 뒤에 따로 있으니 여기서는 문제의 필요성을 먼저 설득하는 편이 맞다.

## 6.3 Related Works

여기에는 현재 가능한 대안, 제품, 서비스, 논문, 오픈소스 등을 조사한다. Figures나 Tables도 넣을 수 있다. 단, 기존 사례를 많이 나열하는 것보다 비교 항목이 중요하다. 각 사례가 대상 사용자, 제공 기능, 장점, 한계, 우리 프로젝트와의 관계에서 어떻게 다른지 정리하면 Proposed Solution의 근거가 된다.

## 6.4 Proposed Solution과 architecture

과제 공지는 overall architecture 또는 project의 overall scenario를 설명하라고 한다. 따라서 이 파트에는 최소한 다음이 있어야 한다.

```text
사용자/센서/데이터 입력
        -> 처리 또는 판단 로직
        -> 저장·외부 서비스 연동이 있다면 그 연결
        -> 사용자에게 보이는 결과 또는 제어 결과
        -> 테스트할 성공 기준
```

software라면 client, server, database, external API, model, deployment의 연결일 수 있다. hardware/embedded라면 sensor, controller, actuator, communication, application의 연결일 수 있다. 아직 구현을 시작하기 전이라 세부 기술은 바뀔 수 있으며, 템플릿도 이 section의 described solution이 이후 implementation 중 변경될 수 있다고 명시한다. 다만 지금도 "누가 무엇을 쓰며 무엇을 만든다"는 수준의 구체성은 필요하다.

## 6.5 Main Functions

Main functions에는 최종 제품의 주요 동작을 사용자의 관점에서 적는다. 예를 들어 로그인, 데이터 입력, 분석, 알림, 검색, 시각화처럼 관찰 가능한 기능 단위가 좋다. 템플릿은 기능이 구현 단계에서 수정·증가할 수 있다고 밝히므로, 초기에 기능을 과도하게 많이 약속하기보다 데모 필수 기능과 있으면 좋은 확장 기능을 나눠두는 편이 안전하다.

## 6.6 역할과 time plan

과제 공지는 specific activities/parts와 담당자를 적고, week 5 task 1, week 6 task 2처럼 workflow를 제시하라고 한다. 템플릿 일정표는 W3부터 W15까지 주차별로 task와 assignment를 배치하게 되어 있다.

일정표는 단순히 "개발"을 여러 주 반복해서 쓰는 대신, 조사·요구사항 확정·설계·개별 기능 구현·통합·테스트·수정·발표자료·보고서로 나누는 것이 좋다. 특히 테스트와 수정 시간을 final 바로 앞에만 몰아두면 위험하다. prototype을 일찍 만들고 중간 발표 전에 최소 시연을 해 본 뒤, 피드백을 반영하는 구조가 강의의 iterative design과 맞다.

---

# 7. 지금부터 제출까지의 실행 순서

## 오늘부터 팀 확정 전

1. 토론 게시판에 팀원 모집 글을 남긴다. 한국어 또는 영어 모두 가능하다고 공지됐다.
2. 관심 분야만 나열하기보다 software/hardware 선호, 구현까지 가능한 주제를 함께 논의하고 싶다는 점, 연락 방법을 적는다.
3. 지원자가 있으면 주제 하나를 고정하기 전에 금요일 수업 참여 가능성, 기술·역할, 협업 방식부터 확인한다.
4. 팀이 3~4명으로 정해지면 대표 제출자와 연락 채널을 먼저 정한다.

## 팀이 정해진 뒤

1. 각자 현실 문제 후보를 2~3개씩 가져온다.
2. 사용자, 기존 해결책, 데모 가능성, 필요한 기술과 비용으로 후보를 비교한다.
3. 한 문제를 고르고 200단어 Problem Statement의 골격을 쓴다.
4. Related Works를 조사하고, 기존 해결의 부족한 지점을 한 문장으로 정리한다.
5. architecture와 핵심 기능을 그린다.
6. 역할과 W5 이후 workflow를 나눈다.
7. 템플릿을 한 파일로 합치고, 대표가 9/20 23:59 전에 업로드한다.

## 제출 전 최종 점검

- [ ] 팀원 수가 3~4명인지 확인
- [ ] 팀당 한 명만 업로드할지 합의
- [ ] Project summary에 목표, 제약, 학기 내 가능성, 최종 결과가 있는지 확인
- [ ] Problem Statement에 사용자, 중요성, 기존 해결의 한계가 있는지 확인
- [ ] Related Works에서 실제 기존 대안을 조사했는지 확인
- [ ] Proposed Solution에 전체 scenario 또는 architecture가 있는지 확인
- [ ] 핵심 기능이 구체적이고 학기 안에 가능한지 확인
- [ ] 활동별 담당자와 주차별 workflow가 있는지 확인
- [ ] 필요한 hardware/software/skills와 예상 지원 필요 자원이 기록됐는지 확인
- [ ] 파일을 대표 한 명이 2026-09-20 23:59 전에 제출했는지 확인

## 아직 확인되지 않은 항목

- 첫 proposal presentation의 실제 날짜·발표 순서·슬라이드 제출 여부
- 격주 보고서의 양식과 제출 주기
- bonus로 표기된 intermediate reports의 실제 적용 방식
- 팀 자동 매칭 여부와 팀 확정 마감
- 지원금 신청 한도 및 프로젝트별 승인 금액

이 항목들은 1강의 일반 안내만으로 확정할 수 없다. 오늘 또는 다음 공지에서 운영 세부사항이 나오면 별도로 기록한다.
## 수업 메모

- 2026-09-11 수업 중 전달: **다음 주 금요일 2026-09-18 캡스톤 수업은 13:30에 시작**한다.
- 이 메모에는 종료 시각이나 이후 매주 시간표 변경 여부가 언급되지 않았으므로, 다음 주 수업 한정 일정으로 기록한다.