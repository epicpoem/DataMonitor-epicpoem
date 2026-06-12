#include <iostream>
#include <memory>
#include <windows.h>
#include "controller/MonitorController.h"
#include "repository/JsonSampleRepository.h"

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    const std::string dataPath = (argc > 1) ? argv[1] : "data/samples.json";

    try {
        auto repo = std::make_unique<JsonSampleRepository>(dataPath);
        MonitorController controller(std::move(repo));
        controller.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
