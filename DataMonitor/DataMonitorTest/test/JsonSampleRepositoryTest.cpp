#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "DataMonitor/repository/JsonSampleRepository.h"

namespace fs = std::filesystem;

class JsonSampleRepositoryTest : public ::testing::Test {
protected:
    std::string tempFile_;

    void SetUp() override {
        tempFile_ = "test_samples_tmp.json";
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

// DataPersistence schema: plain JSON array with avgProdTime key
TEST_F(JsonSampleRepositoryTest, ReturnsAllSamples) {
    writeJson(R"([
      {"id":"S-001","name":"Test Sample","avgProdTime":0.5,"yield":0.92,"stock":100},
      {"id":"S-002","name":"Another",    "avgProdTime":0.3,"yield":0.78,"stock":0  }
    ])");

    JsonSampleRepository repo(tempFile_);
    const auto samples = repo.getAll();

    ASSERT_EQ(samples.size(), 2u);

    EXPECT_EQ(samples[0].id,   "S-001");
    EXPECT_EQ(samples[0].name, "Test Sample");
    EXPECT_DOUBLE_EQ(samples[0].avgProdTime, 0.5);
    EXPECT_DOUBLE_EQ(samples[0].yield,       0.92);
    EXPECT_EQ(samples[0].stock, 100);

    EXPECT_EQ(samples[1].id,    "S-002");
    EXPECT_EQ(samples[1].stock, 0);
}

TEST_F(JsonSampleRepositoryTest, ReturnsEmptyForEmptyArray) {
    writeJson(R"([])");

    JsonSampleRepository repo(tempFile_);
    EXPECT_TRUE(repo.getAll().empty());
}

TEST_F(JsonSampleRepositoryTest, ParsesZeroStock) {
    writeJson(R"([{"id":"S-005","name":"Zero","avgProdTime":0.6,"yield":0.88,"stock":0}])");

    JsonSampleRepository repo(tempFile_);
    const auto samples = repo.getAll();

    ASSERT_EQ(samples.size(), 1u);
    EXPECT_EQ(samples[0].stock, 0);
}

TEST_F(JsonSampleRepositoryTest, FindByIdReturnsCorrectSample) {
    writeJson(R"([
      {"id":"S-001","name":"Test","avgProdTime":0.5,"yield":0.92,"stock":100},
      {"id":"S-002","name":"Other","avgProdTime":0.3,"yield":0.78,"stock":0}
    ])");

    JsonSampleRepository repo(tempFile_);
    const auto result = repo.findById("S-001");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->id,   "S-001");
    EXPECT_EQ(result->name, "Test");
    EXPECT_EQ(result->stock, 100);
}

// ── Negative TCs ──────────────────────────────────────────────────────────────

// 파일 없음
TEST_F(JsonSampleRepositoryTest, ThrowsWhenFileNotFound) {
    JsonSampleRepository repo("nonexistent_file.json");
    EXPECT_THROW(repo.getAll(), std::runtime_error);
}

// 파일 없음 시 findById도 예외
TEST_F(JsonSampleRepositoryTest, FindByIdThrowsWhenFileNotFound) {
    JsonSampleRepository repo("nonexistent_file.json");
    EXPECT_THROW(repo.findById("S-001"), std::runtime_error);
}

// 루트가 배열이 아닌 경우 (객체, 빈 파일 등) → 빈 결과 반환
TEST_F(JsonSampleRepositoryTest, ReturnsEmptyWhenNotAnArray) {
    writeJson(R"({})");

    JsonSampleRepository repo(tempFile_);
    EXPECT_TRUE(repo.getAll().empty());
}

// 빈 파일 → 빈 결과 반환
TEST_F(JsonSampleRepositoryTest, ReturnsEmptyForEmptyFile) {
    writeJson("");

    JsonSampleRepository repo(tempFile_);
    EXPECT_TRUE(repo.getAll().empty());
}

// 알 수 없는 필드가 있어도 파싱 정상 동작
TEST_F(JsonSampleRepositoryTest, IgnoresUnknownFields) {
    writeJson(R"([
      {"id":"S-001","name":"Test","avgProdTime":0.5,"yield":0.92,"stock":100,
       "unknownField":"someValue","anotherUnknown":42}
    ])");

    JsonSampleRepository repo(tempFile_);
    const auto samples = repo.getAll();

    ASSERT_EQ(samples.size(), 1u);
    EXPECT_EQ(samples[0].id,   "S-001");
    EXPECT_EQ(samples[0].stock, 100);
}

// 일부 필드 누락 시 기본값 사용 (id/name만 있는 최소 객체)
TEST_F(JsonSampleRepositoryTest, UseDefaultValuesForMissingFields) {
    writeJson(R"([{"id":"S-001","name":"Minimal"}])");

    JsonSampleRepository repo(tempFile_);
    const auto samples = repo.getAll();

    ASSERT_EQ(samples.size(), 1u);
    EXPECT_EQ(samples[0].id,   "S-001");
    EXPECT_DOUBLE_EQ(samples[0].avgProdTime, 0.0);
    EXPECT_DOUBLE_EQ(samples[0].yield,       0.0);
    EXPECT_EQ(samples[0].stock, 0);
}

// 존재하지 않는 ID 검색 → nullopt 반환
TEST_F(JsonSampleRepositoryTest, FindByIdReturnsNulloptWhenNotFound) {
    writeJson(R"([{"id":"S-001","name":"Test","avgProdTime":0.5,"yield":0.92,"stock":100}])");

    JsonSampleRepository repo(tempFile_);
    EXPECT_FALSE(repo.findById("S-999").has_value());
}

// 빈 ID 검색 → nullopt 반환
TEST_F(JsonSampleRepositoryTest, FindByIdReturnsNulloptForEmptyId) {
    writeJson(R"([{"id":"S-001","name":"Test","avgProdTime":0.5,"yield":0.92,"stock":100}])");

    JsonSampleRepository repo(tempFile_);
    EXPECT_FALSE(repo.findById("").has_value());
}
