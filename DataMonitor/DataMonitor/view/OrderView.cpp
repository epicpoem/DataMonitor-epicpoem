#include "OrderView.h"
#include <iomanip>
#include <sstream>

namespace {
const int COL_NO   = 22;
const int COL_SID  = 12;
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
        << std::setw(COL_SID)  << "시료ID"
        << std::setw(COL_CUST) << "고객명"
        << std::setw(COL_QTY)  << "수량"
        << std::setw(COL_STAT) << "상태"
        << "\n";
    out << std::string(COL_NO + COL_SID + COL_CUST + COL_QTY + COL_STAT, '-') << "\n";

    for (const auto& o : orders) {
        std::ostringstream qtyStr;
        qtyStr << o.quantity << " ea";

        out << std::left
            << std::setw(COL_NO)   << o.id
            << std::setw(COL_SID)  << o.sampleId
            << std::setw(COL_CUST) << o.customerName
            << std::setw(COL_QTY)  << qtyStr.str()
            << std::setw(COL_STAT) << orderStatusToString(o.status)
            << "\n";
    }

    out << "\n총 " << orders.size() << "건\n\n";
}

void OrderView::displayOne(const std::optional<Order>& order, std::ostream& out) const {
    if (!order) {
        out << "\n  해당 주문번호를 찾을 수 없습니다.\n\n";
        return;
    }
    out << "\n";
    out << "  주문번호        : " << order->id           << "\n";
    out << "  시료 ID         : " << order->sampleId     << "\n";
    out << "  고객명          : " << order->customerName << "\n";
    out << "  주문 수량       : " << order->quantity     << " ea\n";
    out << "  상태            : " << orderStatusToString(order->status) << "\n";
    if (order->status == OrderStatus::PRODUCING ||
        order->status == OrderStatus::CONFIRMED  ||
        order->status == OrderStatus::RELEASE) {
        const std::string startTime = order->productionStartTime.empty()
                                    ? "-" : order->productionStartTime;
        out << "  생산 시작 시간  : " << startTime                         << "\n";
        out << "  실생산 수량     : " << order->actualProductionQuantity   << " ea\n";
        out << "  총 생산 시간    : " << std::fixed << std::setprecision(1)
            << order->totalProductionTime << " min\n";
    }
    out << "\n";
}

void OrderView::displayError(const std::string& message, std::ostream& out) const {
    out << "\n  [오류] " << message << "\n\n";
}
