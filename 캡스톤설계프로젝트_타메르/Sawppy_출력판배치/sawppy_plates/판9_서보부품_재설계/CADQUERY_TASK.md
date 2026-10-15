# Sawppy 서보/모터 마운트 부품 재설계 작업 지시

## 1. 배경

Sawppy Rover(https://github.com/Roger-random/Sawppy_Rover, MIT)를 기반으로 자율주행 로버를 제작한다.
원본은 LewanSoul LX-16A 시리얼 버스 서보 10개로 구동 6 + 조향 4를 처리하지만,
본 프로젝트는 구동을 DC 기어모터로, 조향을 MG996R 서보로 대체한다.

따라서 원본 STL 23종 중 서보에 직접 물리는 2종만 재설계가 필요하다.
나머지 21종은 원본 그대로 출력한다.

| 원본 파일 | 처리 |
|---|---|
| `LX-16A - Bracket.stl` x10 | 재설계. 구동용 모터 마운트 6개 + 조향용 서보 브래킷 4개, 2종으로 분리 |
| `LX-16A - Coupler.stl` x10 | 재설계. 조향용 4개만 필요 (구동축은 시판 리지드 커플러 사용) |
| 나머지 21종 | 수정 없음 |

## 2. 산출물

CadQuery(Python) 파라메트릭 스크립트로 아래 3종을 생성하고 STL로 내보낸다.

1. `motor_mount.py` — DC 기어모터 마운트 (6개 필요)
2. `servo_bracket.py` — MG996R 조향 서보 브래킷 (4개 필요)
3. `steer_coupler.py` — 서보 출력축 ↔ 8mm 조향축 커플러 (4개 필요)

## 3. 불변 제약 — 너클 인터페이스

가장 중요한 제약이다. 원본 설계자가 서보 교체 시 지켜야 할 조건을 문서에 명시했다.

> "The conversion process involves making a shaft coupler and a bracket that fits the specific servo
> and mounts to the same places (Two M3 screws + 8mm shaft) as the LewanSoul LX-16A actuator module."
> — `docs/Print Servo Parts.md`

즉 **너클에 닿는 면의 M3 볼트 2개 위치와 8mm 축 구멍 위치**를 원본과 동일하게 유지하면
조향 너클(`Steering Knuckle.stl`), 고정 너클(`Fixed Knuckle.stl`)을 포함한 나머지 부품은 수정할 필요가 없다.

이 치수는 원본 `LX-16A - Bracket.stl`에서 직접 측정해야 한다. (부록의 측정 스크립트 사용)

원본 바운딩 박스 (참고):
- `LX-16A - Bracket.stl` : 54.6 x 15.0 x 34.6 mm
- `LX-16A - Coupler.stl` : 24.0 x 10.0 x 24.0 mm, 서보 혼 체결용 Ø4.2 구멍 4개가 9.9mm 간격 정사각 배치

## 4. 부품별 요구사항

### 4.1 motor_mount.py — DC 기어모터 마운트

모터: JGB37-520B 계열 (원통 지름 37mm, 출력축 6mm D컷)

- 모터 원통을 잡는 클램프 또는 모터 앞면 나사 구멍에 체결되는 판 구조
- 너클 인터페이스(M3 x2 + 8mm 축 구멍)는 원본과 동일 위치 유지
- 모터축(6mm)과 8mm 바퀴축이 동축이 되도록 배치. 두 축은 시판 리지드 커플러(6→8mm)로 연결되므로
  커플러 길이만큼 축 방향 여유를 확보할 것
- 조향 바퀴 4개는 이 마운트가 조향축을 중심으로 회전한다. 모터 뒤쪽이 서스펜션 암과 간섭하지 않는지
  회전 반경을 확인할 것 (모터 길이 약 65mm)

실측이 필요한 값:
- 모터 원통 실제 지름과 길이
- 모터 앞면 나사 구멍 규격(보통 M3)과 피치원 지름
- 출력축 길이와 D컷 위치

### 4.2 servo_bracket.py — MG996R 조향 서보 브래킷

서보: MG996R 표준 사이즈 (40.7 x 19.7 x 42.9 mm, 양쪽 이어에 고정 구멍 4개)

- 서보 몸통이 들어가는 포켓 + 이어 고정 구멍 4개
- 너클 인터페이스(M3 x2 + 8mm 축 구멍)는 원본과 동일 위치 유지
- 서보 출력축과 8mm 조향축이 동축이 되도록 배치

실측이 필요한 값:
- 서보 이어 구멍 간격(장변/단변)과 구멍 지름
- 서보 몸통 실제 치수, 출력축 중심에서 몸통 끝까지 거리

### 4.3 steer_coupler.py — 조향 커플러

- 한쪽: MG996R 혼(horn) 체결. 혼 나사 구멍에 맞는 구멍 배치
- 반대쪽: 8mm 축이 들어가는 구멍 + 측면 M3 세트스크류 구멍(열간 인서트용)
- 원본과 동일하게 세트스크류는 축을 밀어 고정하는 구조. 인서트는 8mm 구멍 안쪽에서 삽입하므로
  인서트 자리 설계 시 방향에 주의 (`docs/Print Servo Parts.md` 참고)

실측이 필요한 값:
- MG996R 혼 나사 구멍의 피치원 지름과 구멍 개수
- 혼 스플라인 규격(25T) 또는 혼을 통째로 끼울 경우 혼 외형 치수

## 5. 개발 환경

```bash
pip install cadquery
```

파일 구조:

```
cad/
├── params.py          # 공통 치수 상수 (너클 인터페이스, 서보/모터 치수)
├── motor_mount.py
├── servo_bracket.py
├── steer_coupler.py
├── export.py          # 3종 STL 일괄 내보내기
└── out/               # 생성된 STL
```

모든 치수는 `params.py`에 상수로 두고 각 스크립트가 import 한다.
실측값이 바뀌면 상수 한 곳만 고쳐서 전체를 다시 생성할 수 있어야 한다.

## 6. 작업 순서

1. 부록 스크립트로 원본 `LX-16A - Bracket.stl`의 너클 인터페이스 치수 측정 → `params.py`에 기록
2. `steer_coupler.py` 부터 작성 (형상이 가장 단순함)
3. `servo_bracket.py`
4. `motor_mount.py`
5. `export.py`로 STL 생성 후 검증

## 7. 검증

- 각 스크립트는 실행 시 형상을 PNG로 렌더링해 육안 확인이 가능해야 한다
- 생성된 솔리드의 바운딩 박스와 주요 구멍 좌표를 assert로 검사한다
  (특히 너클 인터페이스 M3 구멍 2개 위치는 원본 측정값과 ±0.1mm 이내)
- STL 내보내기 후 슬라이서(Bambu Studio)에서 열어 오류 메시지가 없는지 확인
- 3D 프린팅 특성상 구멍은 설계 치수보다 작게 나온다. M3 관통 구멍은 3.2~3.4mm,
  8mm 축 구멍은 8.2mm 정도로 여유를 줄 것

## 8. 참고

- 원본 저장소: https://github.com/Roger-random/Sawppy_Rover
- 서보 부품 문서: `docs/Print Servo Parts.md`
- 드라이브 모듈 조립: `docs/AssembleDriveModules.md`
- 원본 CAD(Onshape, 참고용): `docs/Print Servo Parts.md` 내 링크

## 부록. STL 단면 측정 스크립트

원본 STL에서 구멍 위치와 지름을 추출한다. 지정한 축의 중앙 단면을 잘라 닫힌 원형 루프를 찾는다.

```python
import struct, numpy as np

def load(path):
    with open(path, 'rb') as fh:
        head = fh.read(84)
        n = struct.unpack('<I', head[80:84])[0]
        dt = np.dtype([('n', '<3f4'), ('v', '(3,3)<f4'), ('a', '<u2')])
        d = np.frombuffer(fh.read(n * 50), dtype=dt, count=n)
    return d['v'].astype(float)

def section(tri, axis, val):
    """axis(0=X,1=Y,2=Z) = val 평면으로 자른 단면 선분들"""
    segs = []
    for t in tri:
        d = t[:, axis] - val
        if abs(np.sign(d).sum()) == 3:
            continue
        pts = []
        for i in range(3):
            a, b = t[i], t[(i + 1) % 3]
            da, db = d[i], d[(i + 1) % 3]
            if da == 0:
                pts.append(a)
            elif da * db < 0:
                pts.append(a + (b - a) * (da / (da - db)))
        if len(pts) >= 2:
            keep = [k for k in range(3) if k != axis]
            segs.append((np.array(pts[0])[keep], np.array(pts[1])[keep]))
    return segs

def loops(segs, tol=1e-3):
    """선분들을 이어붙여 닫힌 루프로 만든다"""
    used = [False] * len(segs)
    out = []
    for i, (a, b) in enumerate(segs):
        if used[i]:
            continue
        used[i] = True
        loop, cur = [a, b], b
        changed = True
        while changed:
            changed = False
            for j, (c, d) in enumerate(segs):
                if used[j]:
                    continue
                if np.linalg.norm(c - cur) < tol:
                    loop.append(d); cur = d; used[j] = True; changed = True
                elif np.linalg.norm(d - cur) < tol:
                    loop.append(c); cur = c; used[j] = True; changed = True
        out.append(np.array(loop))
    return out

tri = load('STL/LX-16A - Bracket.stl')
v = tri.reshape(-1, 3)
mn, mx = v.min(0), v.max(0)
print('bbox', mx - mn)

for axis in range(3):
    for frac in (0.25, 0.5, 0.75):
        val = mn[axis] + (mx[axis] - mn[axis]) * frac
        for L in loops(section(tri, axis, val)):
            c = L.mean(0)
            r = np.linalg.norm(L - c, axis=1)
            if r.std() < 0.35 and len(L) > 6 and r.mean() < 12:
                print(f'축{"XYZ"[axis]} {frac:.0%} 단면: 중심 {np.round(c - np.delete(mn, axis), 2)} 반지름 {r.mean():.2f}')
```
