#include "SampleView.h"
#include <iomanip>
#include <sstream>

namespace {
const int COL_ID   = 8;
const int COL_NAME = 26;
const int COL_TIME = 14;
const int COL_YILD = 6;
const int COL_STCK = 8;
} // namespace

void SampleView::displayAll(const std::vector<Sample>& samples, std::ostream& out) const {
    if (samples.empty()) {
        out << "\n  등록된 시료가 없습니다.\n\n";
        return;
    }

    out << "\n";
    out << std::left
        << std::setw(COL_ID)   << "ID"
        << std::setw(COL_NAME) << "이름"
        << std::setw(COL_TIME) << "평균생산시간"
        << std::setw(COL_YILD) << "수율"
        << std::setw(COL_STCK) << "재고"
        << "\n";
    out << std::string(COL_ID + COL_NAME + COL_TIME + COL_YILD + COL_STCK, '-') << "\n";

    for (const auto& s : samples) {
        std::ostringstream timeStr;
        timeStr << std::fixed << std::setprecision(1) << s.avgProductionTime << " min/ea";

        std::ostringstream yieldStr;
        yieldStr << std::fixed << std::setprecision(2) << s.yield;

        std::ostringstream stockStr;
        stockStr << s.stock << " ea";

        out << std::left
            << std::setw(COL_ID)   << s.id
            << std::setw(COL_NAME) << s.name
            << std::setw(COL_TIME) << timeStr.str()
            << std::setw(COL_YILD) << yieldStr.str()
            << std::setw(COL_STCK) << stockStr.str()
            << "\n";
    }

    out << "\n총 " << samples.size() << "건\n\n";
}

void SampleView::displayError(const std::string& message, std::ostream& out) const {
    out << "\n  [오류] " << message << "\n\n";
}
