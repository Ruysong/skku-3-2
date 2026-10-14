# Assignment 01: 제출물과 개념 범위

근거: [안내 PDF](./2026-2-SP41-Assignment-01-1.pdf), [문제 PDF](./2026-2-SP41-Assignment-01-Problems-1.pdf), [최종 제출 PDF](./2026-2-SP41-A01-2024315329-송준혁.pdf).

최종 제출 PDF는 7페이지이고, 사용자 확인상 제출 완료다. 같은 폴더의 괄호가 붙은 다른 PDF나 DOCX를 새 최종본으로 선택하지 않았다. 아래는 원본의 문제별 **찾아보기**이며 제출 답안 전문이나 새 정답 풀이가 아니다.

## 문제별 목차

| 번호 | 최종 PDF 위치 | 무엇을 다뤘나 | 복습 개념 |
| --- | --- | --- | --- |
| 1 | p.1 | 상수 곱을 shift/add/sub 표현으로 구성 | 2의 거듭제곱, 연산 수, 정수 비트 표현 |
| 2 | p.1 | 지정 구간의 1을 가진 unsigned mask 표현 | bitwise NOT, shift, 비트 폭·shift 경계 |
| 3 | p.2~3 | 정수·unsigned 식의 항상 참 여부와 반례/이유 | 2의 보수, overflow, 산술 shift, cast, modulo 연산 |
| 4 | p.3~4 | float의 비트 패턴으로 비교 관계 표현 | 부호, 양수·음수의 순서, +0/-0 |
| 5 | p.4~5 | 일반화된 지수·소수 비트 형식의 값 표현 | bias, E/M/f/V, 정규화, 정밀도와 표현 간격 |
| 6 | p.5 | 명시적 정수 비트를 가진 확장 형식의 극값 | 정규화·비정규화, 최소 양수·음수 극값 |
| 7 | p.5~6 | Format A와 B 사이 값·비트 변환 표 | 표현 범위, directed rounding, underflow/overflow |
| 8 | p.6~7 | int에서 double로 변환한 식의 동등성 | 정수의 정확한 표현, 반올림, 연산 순서 |

## 전체 개념 연결

```text
finite bit width
      |
      +-> integer representation
      |       +-> shifts / masks
      |       +-> two's complement / unsigned
      |       +-> overflow / casts / counterexamples
      |
      +-> floating-point representation
              +-> sign / exponent / fraction / bias
              +-> normalized / denormalized
              +-> precision / range / rounding
              +-> arithmetic equality and order
```

수학적 정수, 유한 폭의 비트 연산 모델, ISO C의 연산 규칙은 무조건 같은 것이 아니다. 원본 답안이 명시한 32비트 2의 보수 가정과 실제 언어의 signed overflow·shift 규칙을 구분하며 복습한다. 이 문서를 실행 가능한 C 코드의 안전성 보증으로 사용하지 않는다.

부동소수점 문제도 비트열을 읽는 단계, 실제 값을 계산하는 단계, 다른 형식으로 반올림하는 단계가 구분된다. Format A/B 변환 표는 최종 PDF p.6에서 직접 대조한다. 숫자·비트열을 새로 옮겨 적다가 원본과 다른 답안을 만들지 않도록 표는 원본을 유지했다.

## 제출 기록

- 공지 기준 마감은 2026-10-10 토요일 17:00이었다.
- 제출 산출물은 위 최종 PDF이며, 프로그램 실행파일·Makefile을 제출하는 과제로 기록하지 않는다.
- 사용자 완료 확인은 있지만 LMS 제출 영수증 파일이나 채점 결과는 이번 점검에서 확인하지 않았다.
- 원본 답안과 문제 PDF는 그대로 보관했고 수정하지 않았다.

[과목 과제 목차](../../pa-study.md)
