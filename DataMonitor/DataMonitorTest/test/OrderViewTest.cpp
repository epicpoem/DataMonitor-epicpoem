#include <gtest/gtest.h>
#include <sstream>
#include "DataMonitor/view/OrderView.h"

TEST(OrderViewTest, DisplaysHeaderAndRowsForNonEmptyList) {
    OrderView view;
    std::vector<Order> orders{
        {"ORD-20260416-0043", "S-003", "SiC 파워기판-6인치", "삼성전자 파운드리", 200, OrderStatus::PRODUCING},
        {"ORD-20260416-0044", "S-001", "실리콘 웨이퍼-8인치",  "SK하이닉스",        150, OrderStatus::RESERVED}
    };

    std::ostringstream out;
    view.displayAll(orders, out);
    const std::string result = out.str();

    EXPECT_NE(result.find("ORD-20260416-0043"), std::string::npos);
    EXPECT_NE(result.find("PRODUCING"),          std::string::npos);
    EXPECT_NE(result.find("200"),                std::string::npos);
    EXPECT_NE(result.find("2건"),                std::string::npos);
}

TEST(OrderViewTest, DisplaysEmptyMessageForEmptyList) {
    OrderView view;
    std::ostringstream out;
    view.displayAll({}, out);

    EXPECT_NE(out.str().find("없습니다"), std::string::npos);
}

TEST(OrderViewTest, DisplaysErrorMessage) {
    OrderView view;
    std::ostringstream out;
    view.displayError("주문 파일을 읽을 수 없습니다", out);

    EXPECT_NE(out.str().find("주문 파일을 읽을 수 없습니다"), std::string::npos);
}
