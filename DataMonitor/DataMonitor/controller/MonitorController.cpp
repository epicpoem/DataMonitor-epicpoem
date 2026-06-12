#include "MonitorController.h"
#include <iostream>
#include <string>
#include <stdexcept>

MonitorController::MonitorController(std::unique_ptr<ISampleRepository> sampleRepo,
                                     std::unique_ptr<IOrderRepository>  orderRepo)
    : sampleRepo_(std::move(sampleRepo))
    , orderRepo_(std::move(orderRepo)) {}

void MonitorController::run() {
    while (true) {
        showMainMenu();

        std::string input;
        std::cout << "선택 > ";
        std::getline(std::cin, input);

        if      (input == "0") break;
        else if (input == "1") showAllSamples();
        else if (input == "2") showAllOrders();
        else                   std::cout << "\n  잘못된 입력입니다. 다시 선택해주세요.\n\n";
    }
    std::cout << "\n프로그램을 종료합니다.\n";
}

void MonitorController::showMainMenu() {
    std::cout << "=== 데이터 모니터링 Tool ===\n\n";
    std::cout << "  [1] 시료 전체 조회\n";
    std::cout << "  [2] 주문 전체 조회\n";
    std::cout << "  [0] 종료\n\n";
}

void MonitorController::showAllSamples() {
    try {
        const auto samples = sampleRepo_->getAll();
        sampleView_.displayAll(samples, std::cout);
    } catch (const std::exception& e) {
        sampleView_.displayError(e.what(), std::cout);
    }
}

void MonitorController::showAllOrders() {
    try {
        const auto orders = orderRepo_->getAll();
        orderView_.displayAll(orders, std::cout);
    } catch (const std::exception& e) {
        orderView_.displayError(e.what(), std::cout);
    }
}
