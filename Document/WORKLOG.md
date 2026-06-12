# WORKLOG - DataMonitor

> **작성 규칙**
> - Agent는 커밋 후 아래 템플릿으로 작업 내용을 기록하고 WORKLOG를 커밋한다.
> - 리뷰 요청 섹션을 작성 후 사용자에게 리뷰를 요청한다.
> - 사용자는 리뷰 섹션에 피드백/요청사항을 작성 후 커밋한다.
> - Agent는 다음 작업 시작 전 반드시 최신 WORKLOG를 확인한다.

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

<!-- 실제 로그는 아래에 최신순으로 작성 -->

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
