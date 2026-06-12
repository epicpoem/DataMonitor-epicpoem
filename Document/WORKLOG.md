# WORKLOG - DataMonitor

> **작성 규칙**
> - Agent는 커밋 후 아래 템플릿으로 작업 내용을 기록하고 WORKLOG를 커밋한다.
> - 리뷰 요청 섹션을 작성 후 사용자에게 리뷰를 요청한다.
> - 사용자는 리뷰 섹션에 피드백/요청사항을 작성 후 커밋한다.
> - Agent는 다음 작업 시작 전 반드시 최신 WORKLOG를 확인한다.
> - **새 로그는 파일 맨 아래에 추가한다 (오래된 순 → 최신 순).**

---

## 템플릿

```
## [YYYY-MM-DD] {작업 제목}

### 작업 내용
- 

### 커밋
- `{hash}` {커밋 메시지}

### 리뷰 요청
- 

---
### 리뷰 (by User)
- 

### 다음 작업 지시
- 
```

---

<!-- 실제 로그는 아래에 오래된 순으로 작성 (최신 항목이 맨 아래) -->

## [2026-06-12] PRD 및 FEATURES 문서 작성

### 작업 내용
- `Document/PRD.md` 작성: 사용자 관점의 요구사항 12개 정의 (핵심 요구사항, 시료 조회, 주문 조회, 데이터 최신성)
- `Document/FEATURES/FEATURE-01-sample-list.md` 작성: 시료 전체 조회 기능 상세 요구사항
- `Document/FEATURES/FEATURE-02-main-menu.md` 작성: 메인 메뉴 및 프로그램 흐름
- `Document/FEATURES/FEATURE-03-order-list.md` 작성: 주문 전체 조회 기능 (선택 구현)

### 커밋
- 미커밋 (사용자 리뷰 후 커밋 예정)

### 리뷰 요청
- PRD의 요구사항 범위 확인: 주문 조회를 선택 구현으로 분류했는데 필수로 포함해야 하는지 확인 요청
- FEATURES 문서 3개 구성이 적절한지 확인 요청 (메인 메뉴를 별도 FEATURE로 분리한 것이 맞는지)
- 승인 후 코드 작업 시작

---
### 리뷰 (by User)
- Feature는 별도 분리해도 무방함
- PRD, Feature 각 리뷰 완료

### 다음 작업 지시
- 코드 작업 시작

---

## [2026-06-12] 핵심 기능 구현 및 테스트 프로젝트 구성

### 작업 내용
- `model/Sample.h`: Sample 데이터 모델 (id, name, avgProductionTime, yield, stock)
- `repository/ISampleRepository.h`: 읽기 전용 Repository 인터페이스
- `repository/JsonSampleRepository.h/.cpp`: JSON 파일 파싱 구현체 (자체 파서, 외부 의존성 없음)
- `view/SampleView.h/.cpp`: 시료 목록 테이블 출력 (ostream 주입으로 테스트 용이성 확보)
- `controller/MonitorController.h/.cpp`: 메인 메뉴 루프 및 흐름 제어
- `main.cpp`: 진입점 - JSON 경로를 인자로 받거나 기본값 `data/samples.json` 사용
- `data/samples.json`: 스펙 예시 시료 5건 샘플 데이터
- `DataMonitorTest/`: gmock 기반 테스트 프로젝트 (JsonSampleRepositoryTest 5개, SampleViewTest 3개)
- CLAUDE.md: 작업 시작 전 WORKLOG 미커밋 자동 커밋 규칙 추가

### 커밋
- `c1747a4` [AI-Docs] 워크플로우에 작업 시작 전 WORKLOG 미커밋 시 자동 커밋 규칙 추가
- `1e5b6bb` [AI-Chore] Visual Studio 솔루션/프로젝트 구성 및 샘플 데이터 추가 (소스/테스트 포함)

### 리뷰 요청
- Visual Studio에서 빌드 확인 요청 (Debug x64 기준)
  - DataMonitor 빌드 → `data/samples.json` 경로에서 실행 확인
  - DataMonitorTest 빌드 → 테스트 8개 통과 여부 확인
- `[AI-Chore]` 커밋에 Feature/Test 코드가 함께 포함된 점 양해 요청 (git add 범위 이슈)

---
### 리뷰 (by User)
- 빌드는 잘 되는 것 확인
- 콘솔창 출력 인코딩 깨짐
- 인코딩 깨짐으로 JSON 생성은 확인하지 못함

### 다음 작업 지시
- 콘솔창 출력 인코딩 깨짐 대응 디버깅
- 커밋 메시지에 양해 요청 내용 추가

---

## [2026-06-12] 콘솔 한글 인코딩 깨짐 수정

### 작업 내용
- `main.cpp`: `SetConsoleOutputCP(CP_UTF8)` / `SetConsoleCP(CP_UTF8)` 추가 — 런타임 코드 페이지를 UTF-8로 설정
- `DataMonitor.vcxproj`: 전체 빌드 구성에 `/utf-8` 컴파일러 플래그 추가 — MSVC가 소스 파일을 UTF-8로 읽도록 지정
- `DataMonitorTest.vcxproj`: 동일하게 `/utf-8` 플래그 추가

### 커밋
- `443ff08` [USER-Review] 빌드 확인 완료, 콘솔 인코딩 깨짐 수정 요청
- `cf59708` [AI-Fix] 콘솔 한글 출력 인코딩 깨짐 수정

### 리뷰 요청
- 빌드 후 재실행 시 한글 콘솔 출력 정상 여부 확인 요청
- `data/samples.json` 경로에서 시료 목록 조회(메뉴 1번) 정상 동작 확인 요청

---
### 리뷰 (by User)
- 시료 목록 조회 정상 동작 확인

### 다음 작업 지시
- 다음 Feature 개발 시작

---

## [2026-06-12] 주문 전체 조회 기능 구현 (FEATURE-03)

### 작업 내용
- `model/Order.h`, `model/OrderStatus.h`: 주문 데이터 모델 및 상태 열거형 (RESERVED/REJECTED/PRODUCING/CONFIRMED/RELEASE)
- `repository/IOrderRepository.h`, `JsonOrderRepository.h/.cpp`: JSON 주문 파싱 구현체
- `view/OrderView.h/.cpp`: 주문 목록 테이블 출력 (주문번호/시료명/고객명/수량/상태)
- `controller/MonitorController`: 메뉴 `[2] 주문 전체 조회` 추가, order repo 의존성 주입
- `main.cpp`: orders.json 경로 인자 추가 (기본값 `data/orders.json`)
- `data/orders.json`: 샘플 주문 데이터 4건 (PRODUCING/RESERVED/CONFIRMED/RELEASE 각 1건)
- 테스트: JsonOrderRepositoryTest 5개, OrderViewTest 3개

### 커밋
- `5f5e037` [USER-Review] 시료 목록 조회 정상 동작 확인 - 다음 Feature 개발 시작
- `59135a2` [AI-Feature] 주문 전체 조회 기능 구현 (FEATURE-03)
- `a724f3c` [AI-Test] 주문 조회 테스트 추가

### 리뷰 요청
- Visual Studio 리빌드 후 확인 요청
  - 메뉴 `[2] 주문 전체 조회` 동작 확인
  - `DataMonitorTest` 빌드 → 전체 테스트 16개 통과 여부 확인
- 주문 데이터 파일 경로 `data/orders.json`이 DataPersistence 출력 경로와 맞는지 확인 요청

---
### 리뷰 (by User)
- 주문 전체 조회 기능 정상 동작 확인

### 다음 작업 지시
- 테스트 빌드 및 수행
- 향후 테스트는 직접 수행 및 worklog에 내용 포함하여 보고

---

## [2026-06-12] 테스트 빌드 및 수행

### 작업 내용
- MSBuild (VS2022, Debug x64)로 DataMonitorTest 빌드 성공
- DataMonitorTest.exe 직접 실행하여 전체 테스트 검증

### 테스트 결과
```
[==========] 16 tests from 4 test suites ran. (49 ms total)
[  PASSED  ] 16 tests.

JsonOrderRepositoryTest  : 5/5 PASSED
JsonSampleRepositoryTest : 5/5 PASSED
OrderViewTest            : 3/3 PASSED
SampleViewTest           : 3/3 PASSED
```

### 커밋
- `5c1b97b` [USER-Review] 주문 전체 조회 정상 동작 확인 - 테스트 직접 수행 요청

### 리뷰 요청
- 전체 테스트 16개 PASSED 확인
- 추가 기능 개발 또는 마무리 여부 지시 요청

---
### 리뷰 (by User)
- WORKLOG 순서를 최신 항목이 맨 아래로 변경 요청
- 다음부터 새 로그를 맨 아래에 추가하도록 CLAUDE.md에 규칙 추가 요청

### 다음 작업 지시
- WORKLOG 순서 변경 및 CLAUDE.md 업데이트

---

## [2026-06-12] MonitorController 테스트 가능 리팩토링 및 단위 테스트 추가

### 작업 내용
- `controller/MonitorController.h/.cpp`: `run()` 시그니처를 `run(istream& in, ostream& out)` 으로 변경 (기본값 cin/cout 유지)
  - `showMainMenu`, `showAllSamples`, `showAllOrders` 내부 메서드도 `ostream&` 파라미터 수용
  - `main.cpp` 무변경 (기본 파라미터 사용)
- `DataMonitorTest/test/MonitorControllerTest.cpp`: gmock MockSampleRepository / MockOrderRepository 기반 단위 테스트 6개 추가
  - 메뉴 1 선택 → `sampleRepo->getAll()` 호출 검증
  - 메뉴 2 선택 → `orderRepo->getAll()` 호출 검증
  - 메뉴 0 → 즉시 종료 확인
  - 잘못된 입력 → 오류 메시지 출력 확인
  - Repository 예외 발생 시 오류 메시지 출력 확인 (Sample/Order 각각)
- `DataMonitorTest.vcxproj`: MonitorController.cpp, MonitorControllerTest.cpp 빌드 대상 추가

### 테스트 결과
```
[==========] 22 tests from 5 test suites ran. (9 ms total)
[  PASSED  ] 22 tests.

MonitorControllerTest    : 6/6 PASSED
JsonOrderRepositoryTest  : 5/5 PASSED
JsonSampleRepositoryTest : 5/5 PASSED
OrderViewTest            : 3/3 PASSED
SampleViewTest           : 3/3 PASSED
```

### 커밋
- `789000f` [AI-Refactoring] MonitorController::run()에 istream/ostream 파라미터 주입으로 테스트 가능 구조로 변경
- `60146dd` [AI-Test] MonitorController Mock 기반 단위 테스트 추가 (6개) - 전체 22개 PASSED

### 리뷰 요청
- Visual Studio 리빌드 후 기존 동작(메인 메뉴, 시료/주문 조회) 정상 유지 여부 확인 요청
- 전체 테스트 22개 PASSED 확인
- 추가 개발 아이템 또는 마무리 여부 지시 요청

---
### 리뷰 (by User)
- 정상 동작 확인
- 시료, 주문 이외에도 필요 기능에 대해서도 기능 구현 필요
- 자체적 구현이 아니라 ../DataPersistence 내부의 CRUD 기능을 활용하여 모니터링이 가능해야 함

### 다음 작업 지시
- ../DataPersistence 개발 진행 상황 파악
- ../DataPersistence 개발 문서 내용 파악
- ../DataPersistence 개발 문서 내 FEATURE에 기초하여 내용 리팩토링
- ../DataPersistence 에서 구현된 CRUD 기반 필요 모니터링 기능 추가

---

## [2026-06-12] DataPersistence 연동 리팩토링 및 모니터링 기능 확장

### 작업 내용
- **DataPersistence 구조 파악**: JSON plain array 스키마, 필드명, FEATURE 문서 9개 분석
- **JSON 스키마 연동 리팩토링** (`[AI-Refactoring]`):
  - `Sample.avgProductionTime` → `avgProdTime` (DataPersistence 필드명 일치)
  - `Order`: `orderNo` → `id`, `sampleName` 제거, 생산정보 3개 필드 추가 (`actualProductionQuantity`, `productionStartTime`, `totalProductionTime`)
  - JSON 파서: `{"samples":[...]}` 래퍼키 방식 → 평탄 배열 `[...]` 방식으로 변경
  - `ISampleRepository` / `IOrderRepository`: `findById()` 메서드 추가
  - `JsonSampleRepository` / `JsonOrderRepository`: `findById()` 구현
  - `data/samples.json` / `data/orders.json`: DataPersistence 호환 스키마로 업데이트
- **모니터링 기능 확장** (`[AI-Feature]`):
  - `SampleView`: `displayOne()` (단건 상세), `displayStock()` (재고 현황 테이블) 추가
  - `OrderView`: `displayOne()` (단건 상세, PRODUCING/CONFIRMED/RELEASE 시 생산정보 포함) 추가
  - `MonitorController`: 메뉴 5개로 확장
    - `[1]` 시료 전체 조회 / `[2]` 시료 단건 조회 / `[3]` 재고 현황 조회
    - `[4]` 주문 전체 조회 / `[5]` 주문 단건 조회 / `[0]` 종료
- **테스트 전면 업데이트**: 기존 22개 → 35개 (13개 신규 추가, 전체 PASSED)

### 테스트 결과
```
[==========] 35 tests from 5 test suites ran. (53 ms total)
[  PASSED  ] 35 tests.

JsonOrderRepositoryTest  : 7/7  PASSED  (findById 2개 신규)
JsonSampleRepositoryTest : 7/7  PASSED  (findById 2개 신규)
MonitorControllerTest    : 9/9  PASSED  (단건조회/재고 3개 신규)
OrderViewTest            : 6/6  PASSED  (displayOne 3개 신규)
SampleViewTest           : 6/6  PASSED  (displayOne/displayStock 3개 신규)
```

### 커밋
- `445e094` [AI-Refactoring] DataPersistence JSON 스키마 연동 - 모델/파서/인터페이스 전면 리팩토링
- `d946cbd` [AI-Feature] 시료/주문 단건 조회 및 재고 현황 조회 기능 추가 (메뉴 5개로 확장)

### 리뷰 요청
- Visual Studio 리빌드 후 실행 확인 요청
  - `[1]` 시료 전체 조회 → `data/samples.json` (DataPersistence 포맷) 정상 출력 확인
  - `[2]` 시료 단건 조회 → ID 입력 후 상세 출력 확인
  - `[3]` 재고 현황 → 재고 테이블 출력 확인
  - `[4]` 주문 전체 조회 → `data/orders.json` (DataPersistence 포맷) 정상 출력 확인
  - `[5]` 주문 단건 조회 → 주문번호 입력 후 생산정보 포함 상세 출력 확인
- DataPersistence 실제 JSON 파일 경로를 인자로 전달하여 연동 확인 요청
  - 예: `DataMonitor.exe <DataPersistence경로>/samples.json <DataPersistence경로>/orders.json`
- 전체 테스트 35개 PASSED 확인

---
### 리뷰 (by User)
- 추가 요구사항 수현 및 정상 동작 확인

### 다음 작업 지시
- Negative TC 추가 및 테스트

---

## [2026-06-12] Negative TC 추가 및 전체 테스트 57개 검증

### 작업 내용
- **JsonSampleRepositoryTest** (+5개 신규):
  - `FindByIdThrowsWhenFileNotFound`: findById 시 파일 없으면 예외
  - `ReturnsEmptyForEmptyFile`: 빈 파일 → 빈 결과
  - `IgnoresUnknownFields`: 알 수 없는 필드 포함 시 정상 파싱
  - `UseDefaultValuesForMissingFields`: 필드 누락 시 기본값(0/0.0) 적용
  - `FindByIdReturnsNulloptForEmptyId`: 빈 문자열 ID → nullopt
- **JsonOrderRepositoryTest** (+6개 신규):
  - `FindByIdThrowsWhenFileNotFound`: findById 시 파일 없으면 예외
  - `ReturnsEmptyForEmptyFile`: 빈 파일 → 빈 결과
  - `ParsesUnknownStatusAsUnknown`: 미정의 status → UNKNOWN
  - `ParsesEmptyStatusAsUnknown`: 빈 status 문자열 → UNKNOWN
  - `IgnoresUnknownFields`: 알 수 없는 필드 포함 시 정상 파싱
  - `FindByIdReturnsNulloptForEmptyId`: 빈 문자열 ID → nullopt
- **MonitorControllerTest** (+6개 신규):
  - `MultipleInvalidInputsThenExit`: 연속 잘못된 입력 3회 후 정상 종료
  - `SampleGetAllThrowDisplaysError` / `OrderGetAllThrowDisplaysError` / `StockGetAllThrowDisplaysError`: 각 getAll 예외 → 오류 메시지
  - `FindSampleThrowDisplaysError` / `FindOrderThrowDisplaysError`: findById 예외 → 오류 메시지
  - `FindSampleNotFoundShowsMessage` / `FindOrderNotFoundShowsMessage`: 미존재 ID → "찾을 수 없습니다"
- **OrderViewTest** (+3개 신규):
  - `DisplaysOneRejectedWithoutProductionInfo`: REJECTED 상태 → 생산정보 없음
  - `DisplaysOneConfirmedWithProductionInfo`: CONFIRMED 상태 → 생산정보 포함
  - `DisplaysOneProducingWithEmptyStartTime`: 생산 시작 시간 없음 → "-" 표시
- **SampleViewTest** (+2개 신규):
  - `DisplaysStockEmptyMessage`: 재고 조회 빈 목록 → "없습니다"
  - `DisplaysZeroStock`: 재고 0인 시료 경계값 정상 출력

### 테스트 결과
```
[==========] 57 tests from 5 test suites ran. (54 ms total)
[  PASSED  ] 57 tests.

JsonOrderRepositoryTest  : 13/13 PASSED
JsonSampleRepositoryTest : 12/12 PASSED
MonitorControllerTest    : 15/15 PASSED
OrderViewTest            :  9/9  PASSED
SampleViewTest           :  8/8  PASSED
```

### 커밋
- `1562cbf` [USER-Review] 추가 요구사항 정상 동작 확인 - Negative TC 추가 요청
- `27fc852` [AI-Test] Negative TC 추가 - 전체 57개 PASSED (35 → 57, 22개 신규)

### 리뷰 요청
- 전체 테스트 57개 PASSED 확인
- 추가 개발 아이템 또는 마무리 여부 지시 요청

---
### 리뷰 (by User)
- 테스트 구현 확인

### 다음 작업 지시
- 리포지토리 최상위에 readme.md 파일 추가, 프로젝트 개요/개발내용/사용법/실행 결과 예시 작성, 이외 필요 내용 작성
