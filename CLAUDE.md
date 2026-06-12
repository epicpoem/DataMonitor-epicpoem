# DataMonitor - 데이터 모니터링 Tool PoC Agent

## 역할
저장된 데이터 전체를 콘솔에서 조회할 수 있는 **관리자용 모니터링 Tool**을 구현하는 PoC.
DataPersistence PoC의 저장소를 참조하여 전체 데이터를 읽고 표시한다.

## 상위 Orchestrator
`../CLAUDE.md` 참조. 스펙 문서: `../Document/spec/spec_reference.txt`

---

## 구현 목표

DataPersistence에서 저장된 JSON 데이터를 읽어,
**전체 데이터를 콘솔에서 조회**할 수 있는 독립적인 Tool을 완성한다.

### 정책 결정 사항
- 구현 범위: **전체 데이터 조회** 위주 (Questions.txt Q8 구두설명)
- DataPersistence 의존 허용, Repository는 분리 유지

---

## 구현할 기능

### 조회 화면 구성 (예시)
```
=== 데이터 모니터링 Tool ===

[1] 시료 전체 조회
[2] 주문 전체 조회      (구현 여부 자율 결정)
[0] 종료

선택 > 1

ID       이름                  평균생산시간  수율   재고
S-001    실리콘 웨이퍼-8인치   0.5 min/ea   0.92   480 ea
S-002    GaN 에피택셀-4인치    0.3 min/ea   0.78   220 ea
...
총 N건
```

### 조회 기능 요구사항
- 전체 목록 출력 (페이징 선택 사항)
- 필터/검색은 선택 사항 (기본 전체 조회가 핵심)
- 콘솔 실행 시 최신 데이터를 읽어 표시

---

## DataPersistence와의 연계

- DataPersistence의 JSON 파일 경로 또는 Repository 인터페이스를 참조
- DataPersistence의 코드를 **직접 수정하지 않음**
- 가능하면 DataPersistence의 Repository 인터페이스를 재사용하여 읽기 전용으로 활용

---

## 제출 기준

- [ ] DataPersistence가 생성한 JSON을 읽어 콘솔 출력
- [ ] 전체 목록 조회 동작 확인
- [ ] DataPersistence 레포와 분리된 독립 Repository
- [ ] CLAUDE.md 존재
- [ ] 의미있는 커밋 이력

---

## 개발 환경 및 테스트 정책

### 개발 언어
- **C++** (C++17 이상 권장)
- 빌드 시스템: CMake 사용 권장

### 테스트 프레임워크
- **Google Mock (gmock) / Google Test (gtest)**
- CMakeLists.txt에 테스트 타겟을 별도 구성
- 테스트 파일 위치: `test/` 디렉토리

### 테스트 정책
- TDD(Test-Driven Development)는 강제하지 않는다.
- 단, 구현한 **각 기능에 대해 반드시 테스트 코드를 작성**한다.
- 기능 구현 커밋(`[AI-Feature]`) 이후 테스트 커밋(`[AI-Test]`)을 함께 남긴다.
- 최소 기준: 정상 동작 케이스 + 경계값/예외 케이스 각 1개 이상

---

## 개발 워크플로우 (WORKLOG 기반)

모든 작업은 아래 사이클을 반드시 따른다.
WORKLOG 파일 위치: `Document/WORKLOG.md`

```
1. 작업 시작 전     → Document/WORKLOG.md 열어 최신 [USER-Review] 내용 확인
2. 작업 및 커밋     → 기능 단위로 [AI-xxx] 헤더로 커밋
3. WORKLOG 업데이트 → 작업 내용 요약, 커밋 해시, 리뷰 요청사항 기록 (커밋 안 함)
4. 사용자 리뷰      → 사용자가 WORKLOG에 피드백/다음 지시 작성
5. 커밋 요청        → 사용자가 커밋 요청 시, Agent가 [USER-Review] 헤더로 커밋 및 푸시
6. 다음 작업        → [USER-Review] 내용 기반으로 작업 후 [AI-xxx] 커밋
7. 3번으로 반복
```

### 커밋 메시지 컨벤션

| 헤더 | 사용 주체 | 사용 시점 |
|------|-----------|-----------|
| `[AI-Feature]` | Agent | 새 기능 구현 |
| `[AI-Fix]` | Agent | 버그 수정 (리뷰 피드백 반영 포함) |
| `[AI-Test]` | Agent | 테스트 코드 작성/수정 |
| `[AI-Refactoring]` | Agent | 리팩토링 (기능 변경 없음) |
| `[AI-Docs]` | Agent | 문서 작성/수정 (PRD, README 등) |
| `[AI-Chore]` | Agent | 빌드 설정, 의존성 추가, 프로젝트 초기 구성 |
| `[USER-Review]` | Agent | 사용자가 WORKLOG 업데이트 후 커밋 요청 시 |

예시:
```
[AI-Feature] 시료 등록 및 조회 기능 구현
[AI-Fix] 중복 ID 허용되던 버그 수정
[AI-Test] 시료 등록 중복 ID 검증 테스트 추가
[AI-Refactoring] SampleController 메서드 분리
[AI-Docs] PRD.md 작성
[AI-Chore] Gradle 프로젝트 초기 구성
[USER-Review] 사용자 리뷰 반영 - 시료 검색 기능 수정 요청
```

### WORKLOG 작성 형식
```markdown
## [YYYY-MM-DD] {작업 제목}

### 작업 내용
- 구현한 내용 요약

### 커밋
- `{hash}` {커밋 메시지}

### 리뷰 요청
- 확인 또는 판단이 필요한 사항

---
### 리뷰 (by User)
- (사용자 작성)

### 다음 작업 지시
- (사용자 작성)
```

---

## 공통 제약
- 모델: **Sonnet / Effort: Medium** 만 허용 (Opus 사용 금지)
- Repository: Public 유지
