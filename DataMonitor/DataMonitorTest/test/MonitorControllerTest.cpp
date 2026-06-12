#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include "DataMonitor/controller/MonitorController.h"
#include "DataMonitor/repository/ISampleRepository.h"
#include "DataMonitor/repository/IOrderRepository.h"

using ::testing::Return;
using ::testing::Throw;
using ::testing::_;

class MockSampleRepository : public ISampleRepository {
public:
    MOCK_METHOD(std::vector<Sample>,   getAll,   (),                       (override));
    MOCK_METHOD(std::optional<Sample>, findById, (const std::string& id),  (override));
};

class MockOrderRepository : public IOrderRepository {
public:
    MOCK_METHOD(std::vector<Order>,   getAll,   (),                       (override));
    MOCK_METHOD(std::optional<Order>, findById, (const std::string& id),  (override));
};

class MonitorControllerTest : public ::testing::Test {
protected:
    std::unique_ptr<MockSampleRepository> mockSampleRepo_;
    std::unique_ptr<MockOrderRepository>  mockOrderRepo_;
    MockSampleRepository* rawSample_{ nullptr };
    MockOrderRepository*  rawOrder_{ nullptr };

    void SetUp() override {
        auto s = std::make_unique<MockSampleRepository>();
        auto o = std::make_unique<MockOrderRepository>();
        rawSample_ = s.get();
        rawOrder_  = o.get();
        mockSampleRepo_ = std::move(s);
        mockOrderRepo_  = std::move(o);
    }

    MonitorController makeController() {
        return MonitorController(std::move(mockSampleRepo_), std::move(mockOrderRepo_));
    }
};

// ── Positive TCs ──────────────────────────────────────────────────────────────

TEST_F(MonitorControllerTest, SelectSampleMenuCallsGetAll) {
    EXPECT_CALL(*rawSample_, getAll()).Times(1).WillOnce(Return(std::vector<Sample>{}));
    EXPECT_CALL(*rawOrder_,  getAll()).Times(0);

    auto ctrl = makeController();
    std::istringstream in("1\n0\n");
    std::ostringstream out;
    ctrl.run(in, out);

    EXPECT_NE(out.str().find("시료"), std::string::npos);
}

TEST_F(MonitorControllerTest, SelectOrderMenuCallsGetAll) {
    EXPECT_CALL(*rawSample_, getAll()).Times(0);
    EXPECT_CALL(*rawOrder_,  getAll()).Times(1).WillOnce(Return(std::vector<Order>{}));

    auto ctrl = makeController();
    std::istringstream in("4\n0\n");
    std::ostringstream out;
    ctrl.run(in, out);

    EXPECT_NE(out.str().find("주문"), std::string::npos);
}

TEST_F(MonitorControllerTest, SelectZeroExitsImmediately) {
    auto ctrl = makeController();
    std::istringstream in("0\n");
    std::ostringstream out;
    ctrl.run(in, out);

    EXPECT_NE(out.str().find("종료"), std::string::npos);
}

TEST_F(MonitorControllerTest, SelectFindSampleCallsFindById) {
    Sample s; s.id = "S-001"; s.name = "웨이퍼"; s.stock = 100;
    EXPECT_CALL(*rawSample_, findById("S-001")).Times(1)
        .WillOnce(Return(std::optional<Sample>{s}));

    auto ctrl = makeController();
    std::istringstream in("2\nS-001\n0\n");
    std::ostringstream out;
    ctrl.run(in, out);

    EXPECT_NE(out.str().find("S-001"), std::string::npos);
}

TEST_F(MonitorControllerTest, SelectStockMenuCallsGetAll) {
    EXPECT_CALL(*rawSample_, getAll()).Times(1).WillOnce(Return(std::vector<Sample>{}));

    auto ctrl = makeController();
    std::istringstream in("3\n0\n");
    std::ostringstream out;
    ctrl.run(in, out);

    EXPECT_NE(out.str().find("재고"), std::string::npos);
}

TEST_F(MonitorControllerTest, SelectFindOrderCallsFindById) {
    Order o; o.id = "ORD-001"; o.customerName = "삼성"; o.status = OrderStatus::RESERVED;
    EXPECT_CALL(*rawOrder_, findById("ORD-001")).Times(1)
        .WillOnce(Return(std::optional<Order>{o}));

    auto ctrl = makeController();
    std::istringstream in("5\nORD-001\n0\n");
    std::ostringstream out;
    ctrl.run(in, out);

    EXPECT_NE(out.str().find("ORD-001"), std::string::npos);
}

// ── Negative TCs ──────────────────────────────────────────────────────────────

// 잘못된 메뉴 입력 → 오류 메시지
TEST_F(MonitorControllerTest, InvalidInputShowsErrorMessage) {
    auto ctrl = makeController();
    std::istringstream in("9\n0\n");
    std::ostringstream out;
    ctrl.run(in, out);

    EXPECT_NE(out.str().find("잘못된 입력"), std::string::npos);
}

// 여러 번 잘못된 입력 후 정상 종료
TEST_F(MonitorControllerTest, MultipleInvalidInputsThenExit) {
    auto ctrl = makeController();
    std::istringstream in("abc\n99\n-1\n0\n");
    std::ostringstream out;
    ctrl.run(in, out);

    // 잘못된 입력 메시지가 3번 출력되어야 함
    std::string result = out.str();
    size_t count = 0;
    size_t pos = 0;
    while ((pos = result.find("잘못된 입력", pos)) != std::string::npos) {
        ++count; ++pos;
    }
    EXPECT_EQ(count, 3u);
    EXPECT_NE(result.find("종료"), std::string::npos);
}

// 시료 전체조회 시 Repository 예외 → 오류 메시지 출력
TEST_F(MonitorControllerTest, SampleGetAllThrowDisplaysError) {
    EXPECT_CALL(*rawSample_, getAll()).Times(1)
        .WillOnce(Throw(std::runtime_error("파일 없음")));

    auto ctrl = makeController();
    std::istringstream in("1\n0\n");
    std::ostringstream out;
    ctrl.run(in, out);

    EXPECT_NE(out.str().find("파일 없음"), std::string::npos);
}

// 주문 전체조회 시 Repository 예외 → 오류 메시지 출력
TEST_F(MonitorControllerTest, OrderGetAllThrowDisplaysError) {
    EXPECT_CALL(*rawOrder_, getAll()).Times(1)
        .WillOnce(Throw(std::runtime_error("주문 파일 오류")));

    auto ctrl = makeController();
    std::istringstream in("4\n0\n");
    std::ostringstream out;
    ctrl.run(in, out);

    EXPECT_NE(out.str().find("주문 파일 오류"), std::string::npos);
}

// 시료 단건조회 시 findById 예외 → 오류 메시지 출력
TEST_F(MonitorControllerTest, FindSampleThrowDisplaysError) {
    EXPECT_CALL(*rawSample_, findById(_)).Times(1)
        .WillOnce(Throw(std::runtime_error("시료 파일 손상")));

    auto ctrl = makeController();
    std::istringstream in("2\nS-001\n0\n");
    std::ostringstream out;
    ctrl.run(in, out);

    EXPECT_NE(out.str().find("시료 파일 손상"), std::string::npos);
}

// 주문 단건조회 시 findById 예외 → 오류 메시지 출력
TEST_F(MonitorControllerTest, FindOrderThrowDisplaysError) {
    EXPECT_CALL(*rawOrder_, findById(_)).Times(1)
        .WillOnce(Throw(std::runtime_error("주문 파일 손상")));

    auto ctrl = makeController();
    std::istringstream in("5\nORD-001\n0\n");
    std::ostringstream out;
    ctrl.run(in, out);

    EXPECT_NE(out.str().find("주문 파일 손상"), std::string::npos);
}

// 존재하지 않는 시료 ID 검색 → "찾을 수 없습니다" 메시지
TEST_F(MonitorControllerTest, FindSampleNotFoundShowsMessage) {
    EXPECT_CALL(*rawSample_, findById("S-999")).Times(1)
        .WillOnce(Return(std::nullopt));

    auto ctrl = makeController();
    std::istringstream in("2\nS-999\n0\n");
    std::ostringstream out;
    ctrl.run(in, out);

    EXPECT_NE(out.str().find("찾을 수 없습니다"), std::string::npos);
}

// 존재하지 않는 주문번호 검색 → "찾을 수 없습니다" 메시지
TEST_F(MonitorControllerTest, FindOrderNotFoundShowsMessage) {
    EXPECT_CALL(*rawOrder_, findById("ORD-999")).Times(1)
        .WillOnce(Return(std::nullopt));

    auto ctrl = makeController();
    std::istringstream in("5\nORD-999\n0\n");
    std::ostringstream out;
    ctrl.run(in, out);

    EXPECT_NE(out.str().find("찾을 수 없습니다"), std::string::npos);
}

// 재고 조회 시 Repository 예외 → 오류 메시지
TEST_F(MonitorControllerTest, StockGetAllThrowDisplaysError) {
    EXPECT_CALL(*rawSample_, getAll()).Times(1)
        .WillOnce(Throw(std::runtime_error("시료 파일 없음")));

    auto ctrl = makeController();
    std::istringstream in("3\n0\n");
    std::ostringstream out;
    ctrl.run(in, out);

    EXPECT_NE(out.str().find("시료 파일 없음"), std::string::npos);
}
