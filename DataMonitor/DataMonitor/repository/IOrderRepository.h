#pragma once
#include <vector>
#include "../model/Order.h"

class IOrderRepository {
public:
    virtual ~IOrderRepository() = default;
    virtual std::vector<Order> getAll() = 0;
};
