#pragma once
#include <memory>
#include "../repository/ISampleRepository.h"
#include "../view/SampleView.h"

class MonitorController {
public:
    explicit MonitorController(std::unique_ptr<ISampleRepository> sampleRepo);
    void run();

private:
    std::unique_ptr<ISampleRepository> sampleRepo_;
    SampleView sampleView_;

    void showMainMenu();
    void showAllSamples();
};
