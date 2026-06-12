#include "OrderView.h"
#include <iomanip>
#include <sstream>

namespace {
const int COL_NO   = 22;
const int COL_NAME = 22;
const int COL_CUST = 20;
const int COL_QTY  = 8;
const int COL_STAT = 10;
} // namespace

void OrderView::displayAll(const std::vector<Order>& orders, std::ostream& out) const {
    if (orders.empty()) {
        out << "\n  등록된 주문이 없습니다.\n\n";
        return;
    }

    out << "\n";
    out << std::left
        << std::setw(COL_NO)   << "주문번호"
        << std::setw(COL_NAME) << "시료명"
        << std::setw(COL_CUST) << "고객명"
        << std::setw(COL_QTY)  << "수량"
        << std::setw(COL_STAT) << "상태"
        << "\n";
    out << std::string(COL_NO + COL_NAME + COL_CUST + COL_QTY + COL_STAT, '-') << "\n";

    for (const auto& o : orders) {
        std::ostringstream qtyStr;
        qtyStr << o.quantity << " ea";

        out << std::left
            << std::setw(COL_NO)   << o.orderNo
            << std::setw(COL_NAME) << o.sampleName
            << std::setw(COL_CUST) << o.customerName
            << std::setw(COL_QTY)  << qtyStr.str()
            << std::setw(COL_STAT) << orderStatusToString(o.status)
            << "\n";
    }

    out << "\n총 " << orders.size() << "건\n\n";
}

void OrderView::displayError(const std::string& message, std::ostream& out) const {
    out << "\n  [오류] " << message << "\n\n";
}
