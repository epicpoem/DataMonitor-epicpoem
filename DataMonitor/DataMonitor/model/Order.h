#pragma once
#include <string>
#include "OrderStatus.h"

struct Order {
    std::string id;
    std::string sampleId;
    std::string customerName;
    int         quantity{ 0 };
    OrderStatus status{ OrderStatus::UNKNOWN };
    int         actualProductionQuantity{ 0 };
    std::string productionStartTime;
    double      totalProductionTime{ 0.0 };
};
