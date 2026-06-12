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

TEST_F(JsonSampleRepositoryTest, ReturnsAllSamples) {
    writeJson(R"({
      "samples": [
        {"id":"S-001","name":"Test Sample","avgProductionTime":0.5,"yield":0.92,"stock":100},
        {"id":"S-002","name":"Another",   "avgProductionTime":0.3,"yield":0.78,"stock":0  }
      ]
    })");

    JsonSampleRepository repo(tempFile_);
    const auto samples = repo.getAll();

    ASSERT_EQ(samples.size(), 2u);

    EXPECT_EQ(samples[0].id,   "S-001");
    EXPECT_EQ(samples[0].name, "Test Sample");
    EXPECT_DOUBLE_EQ(samples[0].avgProductionTime, 0.5);
    EXPECT_DOUBLE_EQ(samples[0].yield,             0.92);
    EXPECT_EQ(samples[0].stock, 100);

    EXPECT_EQ(samples[1].id,    "S-002");
    EXPECT_EQ(samples[1].stock, 0);
}

TEST_F(JsonSampleRepositoryTest, ReturnsEmptyForEmptyArray) {
    writeJson(R"({"samples":[]})");

    JsonSampleRepository repo(tempFile_);
    EXPECT_TRUE(repo.getAll().empty());
}

TEST_F(JsonSampleRepositoryTest, ReturnsEmptyWhenNoSamplesKey) {
    writeJson(R"({})");

    JsonSampleRepository repo(tempFile_);
    EXPECT_TRUE(repo.getAll().empty());
}

TEST_F(JsonSampleRepositoryTest, ThrowsWhenFileNotFound) {
    JsonSampleRepository repo("nonexistent_file.json");
    EXPECT_THROW(repo.getAll(), std::runtime_error);
}

TEST_F(JsonSampleRepositoryTest, ParsesZeroStock) {
    writeJson(R"({"samples":[{"id":"S-005","name":"Zero","avgProductionTime":0.6,"yield":0.88,"stock":0}]})");

    JsonSampleRepository repo(tempFile_);
    const auto samples = repo.getAll();

    ASSERT_EQ(samples.size(), 1u);
    EXPECT_EQ(samples[0].stock, 0);
}
