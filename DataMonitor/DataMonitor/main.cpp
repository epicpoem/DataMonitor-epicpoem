#include <iostream>
#include <memory>
#include <windows.h>
#include "controller/MonitorController.h"
#include "repository/JsonSampleRepository.h"
#include "repository/JsonOrderRepository.h"

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    const std::string samplesPath = (argc > 1) ? argv[1] : "data/samples.json";
    const std::string ordersPath  = (argc > 2) ? argv[2] : "data/orders.json";

    try {
        auto sampleRepo = std::make_unique<JsonSampleRepository>(samplesPath);
        auto orderRepo  = std::make_unique<JsonOrderRepository>(ordersPath);
        MonitorController controller(std::move(sampleRepo), std::move(orderRepo));
        controller.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
