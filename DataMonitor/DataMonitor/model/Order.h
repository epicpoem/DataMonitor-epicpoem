#pragma once
#include <string>
#include "OrderStatus.h"

struct Order {
    std::string orderNo;
    std::string sampleId;
    std::string sampleName;
    std::string customerName;
    int         quantity{ 0 };
    OrderStatus status{ OrderStatus::UNKNOWN };
};
