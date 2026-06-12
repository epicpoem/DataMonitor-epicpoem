#pragma once
#include <iostream>
#include <memory>
#include "../repository/ISampleRepository.h"
#include "../repository/IOrderRepository.h"
#include "../view/SampleView.h"
#include "../view/OrderView.h"

class MonitorController {
public:
    MonitorController(std::unique_ptr<ISampleRepository> sampleRepo,
                      std::unique_ptr<IOrderRepository>  orderRepo);
    void run(std::istream& in = std::cin, std::ostream& out = std::cout);

private:
    std::unique_ptr<ISampleRepository> sampleRepo_;
    std::unique_ptr<IOrderRepository>  orderRepo_;
    SampleView sampleView_;
    OrderView  orderView_;

    void showMainMenu(std::ostream& out);
    void showAllSamples(std::ostream& out);
    void showOneSample(std::istream& in, std::ostream& out);
    void showStock(std::ostream& out);
    void showAllOrders(std::ostream& out);
    void showOneOrder(std::istream& in, std::ostream& out);
};
