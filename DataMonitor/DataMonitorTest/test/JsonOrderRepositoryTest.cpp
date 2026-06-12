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

TEST_F(JsonOrderRepositoryTest, ReturnsAllOrders) {
    writeJson(R"({
      "orders": [
        {"orderNo":"ORD-20260416-0043","sampleId":"S-003","sampleName":"SiC","customerName":"삼성","quantity":200,"status":"PRODUCING"},
        {"orderNo":"ORD-20260416-0044","sampleId":"S-001","sampleName":"Si","customerName":"SK","quantity":150,"status":"RESERVED"}
      ]
    })");

    JsonOrderRepository repo(tempFile_);
    const auto orders = repo.getAll();

    ASSERT_EQ(orders.size(), 2u);

    EXPECT_EQ(orders[0].orderNo,      "ORD-20260416-0043");
    EXPECT_EQ(orders[0].sampleId,     "S-003");
    EXPECT_EQ(orders[0].customerName, "삼성");
    EXPECT_EQ(orders[0].quantity,     200);
    EXPECT_EQ(orders[0].status,       OrderStatus::PRODUCING);

    EXPECT_EQ(orders[1].status, OrderStatus::RESERVED);
}

TEST_F(JsonOrderRepositoryTest, ReturnsEmptyForEmptyArray) {
    writeJson(R"({"orders":[]})");

    JsonOrderRepository repo(tempFile_);
    EXPECT_TRUE(repo.getAll().empty());
}

TEST_F(JsonOrderRepositoryTest, ReturnsEmptyWhenNoOrdersKey) {
    writeJson(R"({})");

    JsonOrderRepository repo(tempFile_);
    EXPECT_TRUE(repo.getAll().empty());
}

TEST_F(JsonOrderRepositoryTest, ThrowsWhenFileNotFound) {
    JsonOrderRepository repo("nonexistent_orders.json");
    EXPECT_THROW(repo.getAll(), std::runtime_error);
}

TEST_F(JsonOrderRepositoryTest, ParsesAllOrderStatuses) {
    writeJson(R"({"orders":[
      {"orderNo":"A","sampleId":"S-001","sampleName":"a","customerName":"c","quantity":1,"status":"CONFIRMED"},
      {"orderNo":"B","sampleId":"S-001","sampleName":"b","customerName":"c","quantity":1,"status":"RELEASE"},
      {"orderNo":"C","sampleId":"S-001","sampleName":"c","customerName":"c","quantity":1,"status":"REJECTED"}
    ]})");

    JsonOrderRepository repo(tempFile_);
    const auto orders = repo.getAll();

    ASSERT_EQ(orders.size(), 3u);
    EXPECT_EQ(orders[0].status, OrderStatus::CONFIRMED);
    EXPECT_EQ(orders[1].status, OrderStatus::RELEASE);
    EXPECT_EQ(orders[2].status, OrderStatus::REJECTED);
}
