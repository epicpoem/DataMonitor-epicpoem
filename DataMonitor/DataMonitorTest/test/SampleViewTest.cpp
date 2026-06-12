#include <gtest/gtest.h>
#include <sstream>
#include "DataMonitor/view/SampleView.h"

TEST(SampleViewTest, DisplaysHeaderAndRowsForNonEmptyList) {
    SampleView view;
    std::vector<Sample> samples{
        {"S-001", "실리콘 웨이퍼-8인치", 0.5, 0.92, 480},
        {"S-002", "GaN 에피택셀-4인치",  0.3, 0.78, 220}
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
