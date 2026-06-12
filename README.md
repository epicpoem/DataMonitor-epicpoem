# DataMonitor

반도체 시료 생산주문관리 시스템 — **데이터 모니터링 Tool (PoC)**

DataPersistence가 저장한 JSON 데이터를 읽어 시료·주문·재고 현황을 콘솔에서 실시간 조회하는 독립형 관리자 도구입니다.

---

> **[WORKLOG.md](Document/WORKLOG.md)**
>
> 교육과정 내 개인 프로젝트이기에, AI 활용이력 / Harness 도입,
> 테스트 적용, 클린 코드 수행 증빙 목적으로 WORKLOG.md를 여기에 제출합니다.

---

## 프로젝트 개요

| 항목 | 내용 |
|------|------|
| 역할 | DataPersistence PoC와 연동하여 저장 데이터를 조회 |
| 언어 | C++20 (MSVC v145, Visual Studio 2022) |
| 빌드 | MSBuild / Visual Studio 솔루션 (`DataMonitor.slnx`) |
| 테스트 | Google Mock / Google Test (gmock 1.11.0 NuGet) |
| 데이터 | DataPersistence가 생성하는 JSON 파일 직접 참조 |
| 저장소 | 독립 Repository (DataPersistence 코드 미수정) |

---

## 주요 기능

| 메뉴 | 기능 |
|------|------|
| `[1]` 시료 전체 조회 | 등록된 모든 시료 목록 테이블 출력 |
| `[2]` 시료 단건 조회 | ID 입력 → 해당 시료 상세 정보 출력 |
| `[3]` 재고 현황 조회 | 시료별 현재 재고 수량 테이블 출력 |
| `[4]` 주문 전체 조회 | 등록된 모든 주문 목록 테이블 출력 |
| `[5]` 주문 단건 조회 | 주문번호 입력 → 상세 정보 출력 (생산 중/완료/출고 시 생산정보 포함) |
| `[0]` 종료 | 프로그램 종료 |

---

## 아키텍처

```
main.cpp
 └── MonitorController          ← 메뉴 흐름 제어, istream/ostream 주입
      ├── ISampleRepository     ← 시료 저장소 인터페이스
      │    └── JsonSampleRepository  ← JSON plain array 파싱 (외부 의존성 없음)
      ├── IOrderRepository      ← 주문 저장소 인터페이스
      │    └── JsonOrderRepository   ← JSON plain array 파싱
      ├── SampleView            ← displayAll / displayOne / displayStock
      └── OrderView             ← displayAll / displayOne
```

**설계 원칙**
- Repository 패턴으로 저장소 추상화 → 테스트 시 Mock으로 교체 가능
- View 메서드에 `ostream&` 주입 → `ostringstream`으로 단위 테스트 가능
- `MonitorController::run(istream&, ostream&)` → 입출력 교체 가능

---

## 빌드 방법

### Visual Studio 2022 (GUI)

1. `DataMonitor\DataMonitor.slnx` 열기
2. 솔루션 탐색기에서 `DataMonitor` 프로젝트 우클릭 → **빌드**
3. 구성: **Debug | x64** 권장

### MSBuild (CLI)

```powershell
$msbuild = "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe"

# 메인 프로그램 빌드
& $msbuild DataMonitor\DataMonitor\DataMonitor.vcxproj /p:Configuration=Debug /p:Platform=x64

# 테스트 프로젝트 빌드
& $msbuild DataMonitor\DataMonitorTest\DataMonitorTest.vcxproj /p:Configuration=Debug /p:Platform=x64
```

---

## 실행 방법

### 기본 실행 (샘플 데이터 사용)

```powershell
# DataMonitor\DataMonitor\x64\Debug\ 에서 실행
.\DataMonitor.exe
# 기본 경로: data\samples.json, data\orders.json
```

### DataPersistence 연동 실행

DataPersistence가 생성한 실제 JSON 파일을 인자로 지정합니다.

```powershell
.\DataMonitor.exe <samples.json 경로> <orders.json 경로>

# 예시
.\DataMonitor.exe "..\..\..\DataPersistence\DataPersistence\DataPersistence\samples.json" `
                  "..\..\..\DataPersistence\DataPersistence\DataPersistence\orders.json"
```

---

## 실행 결과 예시

### 메인 메뉴

```
=== 데이터 모니터링 Tool ===

  [시료]
    [1] 시료 전체 조회
    [2] 시료 단건 조회
    [3] 재고 현황 조회
  [주문]
    [4] 주문 전체 조회
    [5] 주문 단건 조회
    [0] 종료

선택 >
```

### [1] 시료 전체 조회

```
ID        이름                      평균생산시간  수율    재고
--------------------------------------------------------------------------
S-001     실리콘 웨이퍼-8인치       0.5 min/ea    0.92    480 ea
S-002     GaN 에피택셀-4인치        0.3 min/ea    0.78    220 ea
S-003     SiC 파워기판-6인치        0.8 min/ea    0.92    30 ea
S-004     포토레지스트-PR7          0.2 min/ea    0.95    910 ea
S-005     산화막 웨이퍼-SiO2        0.6 min/ea    0.88    0 ea

총 5건
```

### [2] 시료 단건 조회

```
조회할 시료 ID: S-001

  ID          : S-001
  이름        : 실리콘 웨이퍼-8인치
  평균생산시간: 0.50 min/ea
  수율        : 0.92
  재고        : 480 ea
```

### [3] 재고 현황 조회

```
ID        이름                      재고
----------------------------------------------
S-001     실리콘 웨이퍼-8인치       480 ea
S-002     GaN 에피택셀-4인치        220 ea
S-003     SiC 파워기판-6인치        30 ea
S-004     포토레지스트-PR7          910 ea
S-005     산화막 웨이퍼-SiO2        0 ea

총 5건
```

### [4] 주문 전체 조회

```
주문번호               시료ID      고객명              수량    상태
------------------------------------------------------------------------
ORD-20260416-0043      S-003       삼성전자 파운드리    200 ea  PRODUCING
ORD-20260416-0044      S-001       SK하이닉스           150 ea  RESERVED
ORD-20260416-0045      S-004       인텔코리아           300 ea  CONFIRMED
ORD-20260415-0021      S-002       TSMC 연구소          50 ea   RELEASE

총 4건
```

### [5] 주문 단건 조회 (생산 중)

```
조회할 주문번호: ORD-20260416-0043

  주문번호        : ORD-20260416-0043
  시료 ID         : S-003
  고객명          : 삼성전자 파운드리
  주문 수량       : 200 ea
  상태            : PRODUCING
  생산 시작 시간  : 2026-04-16T09:00:00
  실생산 수량     : 120 ea
  총 생산 시간    : 96.0 min
```

---

## 테스트

### 테스트 실행

```powershell
# 빌드 후 실행
.\DataMonitor\DataMonitorTest\x64\Debug\DataMonitorTest.exe
```

### 테스트 현황 (최신)

```
[==========] 57 tests from 5 test suites ran.
[  PASSED  ] 57 tests.

JsonOrderRepositoryTest  : 13/13  (파싱, findById, Negative: 파일없음/빈파일/미지정status/필드무시)
JsonSampleRepositoryTest : 12/12  (파싱, findById, Negative: 파일없음/빈파일/기본값/필드무시)
MonitorControllerTest    : 15/15  (메뉴흐름, Mock 주입, Negative: 예외처리/미존재ID/다중잘못입력)
OrderViewTest            :  9/9   (목록/단건 출력, Negative: 빈목록/미존재/상태별 생산정보)
SampleViewTest           :  8/8   (목록/단건/재고 출력, Negative: 빈목록/미존재/재고0)
```

### 테스트 구성 전략

| 레이어 | 방식 | 목적 |
|--------|------|------|
| Repository | 임시 파일 생성/삭제 (`SetUp`/`TearDown`) | 실제 JSON 파싱 검증 |
| View | `ostringstream` 캡처 | 출력 문자열 단위 검증 |
| Controller | gmock `MockSampleRepository` / `MockOrderRepository` | 메뉴 흐름 및 예외 처리 검증 |

---

## JSON 데이터 스키마

DataPersistence와 동일한 스키마를 사용합니다.

### samples.json

```json
[
  {
    "id": "S-001",
    "name": "실리콘 웨이퍼-8인치",
    "avgProdTime": 0.5,
    "yield": 0.92,
    "stock": 480
  }
]
```

### orders.json

```json
[
  {
    "id": "ORD-20260416-0043",
    "sampleId": "S-003",
    "customerName": "삼성전자 파운드리",
    "quantity": 200,
    "status": "PRODUCING",
    "actualProductionQuantity": 120,
    "productionStartTime": "2026-04-16T09:00:00",
    "totalProductionTime": 96.0
  }
]
```

**주문 상태값**: `RESERVED` | `REJECTED` | `PRODUCING` | `CONFIRMED` | `RELEASE`

---

## 디렉토리 구조

```
DataMonitor/
├── README.md
├── CLAUDE.md
├── Document/
│   ├── PRD.md
│   ├── WORKLOG.md
│   └── FEATURES/
│       ├── FEATURE-01-sample-list.md
│       ├── FEATURE-02-main-menu.md
│       └── FEATURE-03-order-list.md
└── DataMonitor/                        ← Visual Studio 솔루션
    ├── DataMonitor.slnx
    ├── DataMonitor/                    ← 메인 프로젝트
    │   ├── main.cpp
    │   ├── controller/
    │   │   └── MonitorController.h/.cpp
    │   ├── model/
    │   │   ├── Sample.h
    │   │   ├── Order.h
    │   │   └── OrderStatus.h
    │   ├── repository/
    │   │   ├── ISampleRepository.h
    │   │   ├── IOrderRepository.h
    │   │   ├── JsonSampleRepository.h/.cpp
    │   │   └── JsonOrderRepository.h/.cpp
    │   ├── view/
    │   │   ├── SampleView.h/.cpp
    │   │   └── OrderView.h/.cpp
    │   └── data/
    │       ├── samples.json            ← 샘플 데이터 (DataPersistence 포맷)
    │       └── orders.json
    └── DataMonitorTest/                ← 테스트 프로젝트
        └── test/
            ├── JsonSampleRepositoryTest.cpp
            ├── JsonOrderRepositoryTest.cpp
            ├── SampleViewTest.cpp
            ├── OrderViewTest.cpp
            └── MonitorControllerTest.cpp
```
