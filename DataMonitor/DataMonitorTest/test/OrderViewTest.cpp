#include <gtest/gtest.h>
#include <sstream>
#include "DataMonitor/view/OrderView.h"

static Order makeOrder(const std::string& id, const std::string& sampleId,
                       const std::string& customerName, int qty, OrderStatus status) {
    Order o;
    o.id = id; o.sampleId = sampleId;
    o.customerName = customerName; o.quantity = qty; o.status = status;
    return o;
}

TEST(OrderViewTest, DisplaysHeaderAndRowsForNonEmptyList) {
    OrderView view;
    std::vector<Order> orders{
        makeOrder("ORD-20260416-0043", "S-003", "삼성전자 파운드리", 200, OrderStatus::PRODUCING),
        makeOrder("ORD-20260416-0044", "S-001", "SK하이닉스",        150, OrderStatus::RESERVED)
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

TEST(OrderViewTest, DisplaysOneFoundWithProductionInfo) {
    OrderView view;
    Order o = makeOrder("ORD-001", "S-003", "삼성", 200, OrderStatus::PRODUCING);
    o.actualProductionQuantity = 120;
    o.productionStartTime = "2026-04-16T09:00:00";
    o.totalProductionTime = 96.0;

    std::ostringstream out;
    view.displayOne(o, out);
    const std::string result = out.str();

    EXPECT_NE(result.find("ORD-001"),  std::string::npos);
    EXPECT_NE(result.find("PRODUCING"),std::string::npos);
    EXPECT_NE(result.find("120"),      std::string::npos);
    EXPECT_NE(result.find("96"),       std::string::npos);
}

TEST(OrderViewTest, DisplaysOneNotFound) {
    OrderView view;
    std::ostringstream out;
    view.displayOne(std::nullopt, out);

    EXPECT_NE(out.str().find("찾을 수 없습니다"), std::string::npos);
}

TEST(OrderViewTest, DisplaysOneReservedWithoutProductionInfo) {
    OrderView view;
    Order o = makeOrder("ORD-002", "S-001", "SK", 100, OrderStatus::RESERVED);

    std::ostringstream out;
    view.displayOne(o, out);
    const std::string result = out.str();

    EXPECT_NE(result.find("RESERVED"), std::string::npos);
    EXPECT_EQ(result.find("생산 시작"),  std::string::npos);
}
