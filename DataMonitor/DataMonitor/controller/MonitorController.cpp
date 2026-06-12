#include "MonitorController.h"
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
        else if (input == "2") showOneSample(in, out);
        else if (input == "3") showStock(out);
        else if (input == "4") showAllOrders(out);
        else if (input == "5") showOneOrder(in, out);
        else                   out << "\n  잘못된 입력입니다. 다시 선택해주세요.\n\n";
    }
    out << "\n프로그램을 종료합니다.\n";
}

void MonitorController::showMainMenu(std::ostream& out) {
    out << "=== 데이터 모니터링 Tool ===\n\n";
    out << "  [시료]\n";
    out << "    [1] 시료 전체 조회\n";
    out << "    [2] 시료 단건 조회\n";
    out << "    [3] 재고 현황 조회\n";
    out << "  [주문]\n";
    out << "    [4] 주문 전체 조회\n";
    out << "    [5] 주문 단건 조회\n";
    out << "    [0] 종료\n\n";
}

void MonitorController::showAllSamples(std::ostream& out) {
    try {
        sampleView_.displayAll(sampleRepo_->getAll(), out);
    } catch (const std::exception& e) {
        sampleView_.displayError(e.what(), out);
    }
}

void MonitorController::showOneSample(std::istream& in, std::ostream& out) {
    out << "조회할 시료 ID: ";
    std::string id;
    std::getline(in, id);
    try {
        sampleView_.displayOne(sampleRepo_->findById(id), out);
    } catch (const std::exception& e) {
        sampleView_.displayError(e.what(), out);
    }
}

void MonitorController::showStock(std::ostream& out) {
    try {
        sampleView_.displayStock(sampleRepo_->getAll(), out);
    } catch (const std::exception& e) {
        sampleView_.displayError(e.what(), out);
    }
}

void MonitorController::showAllOrders(std::ostream& out) {
    try {
        orderView_.displayAll(orderRepo_->getAll(), out);
    } catch (const std::exception& e) {
        orderView_.displayError(e.what(), out);
    }
}

void MonitorController::showOneOrder(std::istream& in, std::ostream& out) {
    out << "조회할 주문번호: ";
    std::string id;
    std::getline(in, id);
    try {
        orderView_.displayOne(orderRepo_->findById(id), out);
    } catch (const std::exception& e) {
        orderView_.displayError(e.what(), out);
    }
}
