#include <gtest/gtest.h>
#include <sstream>
#include "DataMonitor/view/SampleView.h"

static Sample makeSample(const std::string& id, const std::string& name,
                         double avgProdTime, double yield, int stock) {
    Sample s;
    s.id = id; s.name = name;
    s.avgProdTime = avgProdTime; s.yield = yield; s.stock = stock;
    return s;
}

TEST(SampleViewTest, DisplaysHeaderAndRowsForNonEmptyList) {
    SampleView view;
    std::vector<Sample> samples{
        makeSample("S-001", "실리콘 웨이퍼-8인치", 0.5, 0.92, 480),
        makeSample("S-002", "GaN 에피택셀-4인치",  0.3, 0.78, 220)
    };

    std::ostringstream out;
    view.displayAll(samples, out);
    const std::string result = out.str();

    EXPECT_NE(result.find("S-001"), std::string::npos);
    EXPECT_NE(result.find("S-002"), std::string::npos);
    EXPECT_NE(result.find("0.92"),  std::string::npos);
    EXPECT_NE(result.find("480"),   std::string::npos);
    EXPECT_NE(result.find("2건"),   std::string::npos);
}

TEST(SampleViewTest, DisplaysEmptyMessageForEmptyList) {
    SampleView view;
    std::ostringstream out;
    view.displayAll({}, out);

    EXPECT_NE(out.str().find("없습니다"), std::string::npos);
}

TEST(SampleViewTest, DisplaysErrorMessage) {
    SampleView view;
    std::ostringstream out;
    view.displayError("파일을 찾을 수 없습니다", out);

    EXPECT_NE(out.str().find("파일을 찾을 수 없습니다"), std::string::npos);
}

TEST(SampleViewTest, DisplaysOneFound) {
    SampleView view;
    Sample s = makeSample("S-001", "실리콘 웨이퍼", 0.5, 0.92, 100);
    std::ostringstream out;
    view.displayOne(s, out);
    const std::string result = out.str();

    EXPECT_NE(result.find("S-001"),     std::string::npos);
    EXPECT_NE(result.find("실리콘 웨이퍼"), std::string::npos);
    EXPECT_NE(result.find("100"),       std::string::npos);
}

TEST(SampleViewTest, DisplaysOneNotFound) {
    SampleView view;
    std::ostringstream out;
    view.displayOne(std::nullopt, out);

    EXPECT_NE(out.str().find("찾을 수 없습니다"), std::string::npos);
}

TEST(SampleViewTest, DisplaysStockTable) {
    SampleView view;
    std::vector<Sample> samples{
        makeSample("S-001", "웨이퍼", 0.5, 0.92, 480),
        makeSample("S-002", "GaN",    0.3, 0.78,   0)
    };
    std::ostringstream out;
    view.displayStock(samples, out);
    const std::string result = out.str();

    EXPECT_NE(result.find("S-001"), std::string::npos);
    EXPECT_NE(result.find("480"),   std::string::npos);
    EXPECT_NE(result.find("재고"),  std::string::npos);
}
