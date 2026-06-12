#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "DataMonitor/repository/JsonOrderRepository.h"

namespace fs = std::filesystem;

class JsonOrderRepositoryTest : public ::testing::Test {
protected:
    std::string tempFile_;

    void SetUp() override {
        tempFile_ = "test_orders_tmp.json";
    }

    void TearDown() override {
        if (fs::exists(tempFile_))
            fs::remove(tempFile_);
    }

    void writeJson(const std::string& content) {
        std::ofstream f(tempFile_);
        f << content;
    }
};

// ── Positive TCs ──────────────────────────────────────────────────────────────

// DataPersistence schema: plain JSON array, id field (not orderNo), no sampleName
TEST_F(JsonOrderRepositoryTest, ReturnsAllOrders) {
    writeJson(R"([
      {"id":"ORD-20260416-0043","sampleId":"S-003","customerName":"삼성","quantity":200,"status":"PRODUCING",
       "actualProductionQuantity":120,"productionStartTime":"2026-04-16T09:00:00","totalProductionTime":96.0},
      {"id":"ORD-20260416-0044","sampleId":"S-001","customerName":"SK","quantity":150,"status":"RESERVED",
       "actualProductionQuantity":0,"productionStartTime":"","totalProductionTime":0.0}
    ])");

    JsonOrderRepository repo(tempFile_);
    const auto orders = repo.getAll();

    ASSERT_EQ(orders.size(), 2u);

    EXPECT_EQ(orders[0].id,           "ORD-20260416-0043");
    EXPECT_EQ(orders[0].sampleId,     "S-003");
    EXPECT_EQ(orders[0].customerName, "삼성");
    EXPECT_EQ(orders[0].quantity,     200);
    EXPECT_EQ(orders[0].status,       OrderStatus::PRODUCING);
    EXPECT_EQ(orders[0].actualProductionQuantity, 120);
    EXPECT_EQ(orders[0].productionStartTime,      "2026-04-16T09:00:00");
    EXPECT_DOUBLE_EQ(orders[0].totalProductionTime, 96.0);

    EXPECT_EQ(orders[1].status, OrderStatus::RESERVED);
}

TEST_F(JsonOrderRepositoryTest, ReturnsEmptyForEmptyArray) {
    writeJson(R"([])");

    JsonOrderRepository repo(tempFile_);
    EXPECT_TRUE(repo.getAll().empty());
}

TEST_F(JsonOrderRepositoryTest, ParsesAllKnownOrderStatuses) {
    writeJson(R"([
      {"id":"A","sampleId":"S-001","customerName":"c","quantity":1,"status":"CONFIRMED",
       "actualProductionQuantity":1,"productionStartTime":"","totalProductionTime":0.0},
      {"id":"B","sampleId":"S-001","customerName":"c","quantity":1,"status":"RELEASE",
       "actualProductionQuantity":1,"productionStartTime":"","totalProductionTime":0.0},
      {"id":"C","sampleId":"S-001","customerName":"c","quantity":1,"status":"REJECTED",
       "actualProductionQuantity":0,"productionStartTime":"","totalProductionTime":0.0}
    ])");

    JsonOrderRepository repo(tempFile_);
    const auto orders = repo.getAll();

    ASSERT_EQ(orders.size(), 3u);
    EXPECT_EQ(orders[0].status, OrderStatus::CONFIRMED);
    EXPECT_EQ(orders[1].status, OrderStatus::RELEASE);
    EXPECT_EQ(orders[2].status, OrderStatus::REJECTED);
}

TEST_F(JsonOrderRepositoryTest, FindByIdReturnsCorrectOrder) {
    writeJson(R"([
      {"id":"ORD-001","sampleId":"S-001","customerName":"고객A","quantity":100,"status":"RESERVED",
       "actualProductionQuantity":0,"productionStartTime":"","totalProductionTime":0.0},
      {"id":"ORD-002","sampleId":"S-002","customerName":"고객B","quantity":50,"status":"CONFIRMED",
       "actualProductionQuantity":50,"productionStartTime":"2026-01-01T10:00:00","totalProductionTime":10.0}
    ])");

    JsonOrderRepository repo(tempFile_);
    const auto result = repo.findById("ORD-002");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->id,           "ORD-002");
    EXPECT_EQ(result->customerName, "고객B");
    EXPECT_EQ(result->status,       OrderStatus::CONFIRMED);
    EXPECT_EQ(result->actualProductionQuantity, 50);
}

// ── Negative TCs ──────────────────────────────────────────────────────────────

// 파일 없음
TEST_F(JsonOrderRepositoryTest, ThrowsWhenFileNotFound) {
    JsonOrderRepository repo("nonexistent_orders.json");
    EXPECT_THROW(repo.getAll(), std::runtime_error);
}

// 파일 없음 시 findById도 예외
TEST_F(JsonOrderRepositoryTest, FindByIdThrowsWhenFileNotFound) {
    JsonOrderRepository repo("nonexistent_orders.json");
    EXPECT_THROW(repo.findById("ORD-001"), std::runtime_error);
}

// 루트가 배열이 아닌 경우 → 빈 결과
TEST_F(JsonOrderRepositoryTest, ReturnsEmptyWhenNotAnArray) {
    writeJson(R"({})");

    JsonOrderRepository repo(tempFile_);
    EXPECT_TRUE(repo.getAll().empty());
}

// 빈 파일 → 빈 결과 반환
TEST_F(JsonOrderRepositoryTest, ReturnsEmptyForEmptyFile) {
    writeJson("");

    JsonOrderRepository repo(tempFile_);
    EXPECT_TRUE(repo.getAll().empty());
}

// 알 수 없는 status 문자열 → UNKNOWN으로 파싱
TEST_F(JsonOrderRepositoryTest, ParsesUnknownStatusAsUnknown) {
    writeJson(R"([
      {"id":"ORD-001","sampleId":"S-001","customerName":"c","quantity":1,
       "status":"INVALID_STATUS","actualProductionQuantity":0,"productionStartTime":"","totalProductionTime":0.0}
    ])");

    JsonOrderRepository repo(tempFile_);
    const auto orders = repo.getAll();

    ASSERT_EQ(orders.size(), 1u);
    EXPECT_EQ(orders[0].status, OrderStatus::UNKNOWN);
}

// 빈 status 문자열 → UNKNOWN
TEST_F(JsonOrderRepositoryTest, ParsesEmptyStatusAsUnknown) {
    writeJson(R"([
      {"id":"ORD-001","sampleId":"S-001","customerName":"c","quantity":1,
       "status":"","actualProductionQuantity":0,"productionStartTime":"","totalProductionTime":0.0}
    ])");

    JsonOrderRepository repo(tempFile_);
    const auto orders = repo.getAll();

    ASSERT_EQ(orders.size(), 1u);
    EXPECT_EQ(orders[0].status, OrderStatus::UNKNOWN);
}

// 알 수 없는 필드 포함 → 정상 파싱
TEST_F(JsonOrderRepositoryTest, IgnoresUnknownFields) {
    writeJson(R"([
      {"id":"ORD-001","sampleId":"S-001","customerName":"c","quantity":10,
       "status":"RESERVED","extraField":"ignored","anotherExtra":999,
       "actualProductionQuantity":0,"productionStartTime":"","totalProductionTime":0.0}
    ])");

    JsonOrderRepository repo(tempFile_);
    const auto orders = repo.getAll();

    ASSERT_EQ(orders.size(), 1u);
    EXPECT_EQ(orders[0].id,       "ORD-001");
    EXPECT_EQ(orders[0].quantity, 10);
    EXPECT_EQ(orders[0].status,   OrderStatus::RESERVED);
}

// 존재하지 않는 ID → nullopt
TEST_F(JsonOrderRepositoryTest, FindByIdReturnsNulloptWhenNotFound) {
    writeJson(R"([{"id":"ORD-001","sampleId":"S-001","customerName":"c","quantity":1,"status":"RESERVED",
       "actualProductionQuantity":0,"productionStartTime":"","totalProductionTime":0.0}])");

    JsonOrderRepository repo(tempFile_);
    EXPECT_FALSE(repo.findById("ORD-999").has_value());
}

// 빈 ID 검색 → nullopt
TEST_F(JsonOrderRepositoryTest, FindByIdReturnsNulloptForEmptyId) {
    writeJson(R"([{"id":"ORD-001","sampleId":"S-001","customerName":"c","quantity":1,"status":"RESERVED",
       "actualProductionQuantity":0,"productionStartTime":"","totalProductionTime":0.0}])");

    JsonOrderRepository repo(tempFile_);
    EXPECT_FALSE(repo.findById("").has_value());
}
