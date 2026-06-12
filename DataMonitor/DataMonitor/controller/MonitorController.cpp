#include "MonitorController.h"
#include <iostream>
#include <string>
#include <stdexcept>

MonitorController::MonitorController(std::unique_ptr<ISampleRepository> sampleRepo,
                                     std::unique_ptr<IOrderRepository>  orderRepo)
    : sampleRepo_(std::move(sampleRepo))
    , orderRepo_(std::move(orderRepo)) {}

void MonitorController::run(std::istream& in, std::ostream& out) {
    while (true) {
        showMainMenu(out);

        std::string input;
        out << "선택 > ";
        std::getline(in, input);

        if      (input == "0") break;
        else if (input == "1") showAllSamples(out);
        else if (input == "2") showAllOrders(out);
        else                   out << "\n  잘못된 입력입니다. 다시 선택해주세요.\n\n";
    }
    out << "\n프로그램을 종료합니다.\n";
}

void MonitorController::showMainMenu(std::ostream& out) {
    out << "=== 데이터 모니터링 Tool ===\n\n";
    out << "  [1] 시료 전체 조회\n";
    out << "  [2] 주문 전체 조회\n";
    out << "  [0] 종료\n\n";
}

void MonitorController::showAllSamples(std::ostream& out) {
    try {
        const auto samples = sampleRepo_->getAll();
        sampleView_.displayAll(samples, out);
    } catch (const std::exception& e) {
        sampleView_.displayError(e.what(), out);
    }
}

void MonitorController::showAllOrders(std::ostream& out) {
    try {
        const auto orders = orderRepo_->getAll();
        orderView_.displayAll(orders, out);
    } catch (const std::exception& e) {
        orderView_.displayError(e.what(), out);
    }
}
