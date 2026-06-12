#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include "DataMonitor/controller/MonitorController.h"
#include "DataMonitor/repository/ISampleRepository.h"
#include "DataMonitor/repository/IOrderRepository.h"

using ::testing::Return;
using ::testing::Throw;

class MockSampleRepository : public ISampleRepository {
public:
    MOCK_METHOD(std::vector<Sample>, getAll, (), (override));
};

class MockOrderRepository : public IOrderRepository {
public:
    MOCK_METHOD(std::vector<Order>, getAll, (), (override));
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
    std::istringstream in("2\n0\n");
    std::ostringstream out;
    ctrl.run(in, out);

    EXPECT_NE(out.str().find("주문"), std::string::npos);
}

TEST_F(MonitorControllerTest, SelectZeroExitsImmediately) {
    EXPECT_CALL(*rawSample_, getAll()).Times(0);
    EXPECT_CALL(*rawOrder_,  getAll()).Times(0);

    auto ctrl = makeController();
    std::istringstream in("0\n");
    std::ostringstream out;
    ctrl.run(in, out);

    EXPECT_NE(out.str().find("종료"), std::string::npos);
}

TEST_F(MonitorControllerTest, InvalidInputShowsErrorMessage) {
    EXPECT_CALL(*rawSample_, getAll()).Times(0);
    EXPECT_CALL(*rawOrder_,  getAll()).Times(0);

    auto ctrl = makeController();
    std::istringstream in("9\n0\n");
    std::ostringstream out;
    ctrl.run(in, out);

    EXPECT_NE(out.str().find("잘못된 입력"), std::string::npos);
}

TEST_F(MonitorControllerTest, SampleRepoThrowDisplaysError) {
    EXPECT_CALL(*rawSample_, getAll()).Times(1)
        .WillOnce(Throw(std::runtime_error("파일 없음")));

    auto ctrl = makeController();
    std::istringstream in("1\n0\n");
    std::ostringstream out;
    ctrl.run(in, out);

    EXPECT_NE(out.str().find("파일 없음"), std::string::npos);
}

TEST_F(MonitorControllerTest, OrderRepoThrowDisplaysError) {
    EXPECT_CALL(*rawOrder_, getAll()).Times(1)
        .WillOnce(Throw(std::runtime_error("주문 파일 오류")));

    auto ctrl = makeController();
    std::istringstream in("2\n0\n");
    std::ostringstream out;
    ctrl.run(in, out);

    EXPECT_NE(out.str().find("주문 파일 오류"), std::string::npos);
}
