#pragma once
#include <optional>
#include <string>
#include <vector>
#include "../model/Order.h"

class IOrderRepository {
public:
    virtual ~IOrderRepository() = default;
    virtual std::vector<Order>     getAll() = 0;
    virtual std::optional<Order>   findById(const std::string& id) = 0;
};
