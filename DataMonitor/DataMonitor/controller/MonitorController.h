#pragma once
#include <memory>
#include "../repository/ISampleRepository.h"
#include "../repository/IOrderRepository.h"
#include "../view/SampleView.h"
#include "../view/OrderView.h"

class MonitorController {
public:
    MonitorController(std::unique_ptr<ISampleRepository> sampleRepo,
                      std::unique_ptr<IOrderRepository>  orderRepo);
    void run();

private:
    std::unique_ptr<ISampleRepository> sampleRepo_;
    std::unique_ptr<IOrderRepository>  orderRepo_;
    SampleView sampleView_;
    OrderView  orderView_;

    void showMainMenu();
    void showAllSamples();
    void showAllOrders();
};
