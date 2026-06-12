#pragma once
#include <vector>
#include <ostream>
#include "../model/Order.h"

class OrderView {
public:
    void displayAll(const std::vector<Order>& orders, std::ostream& out) const;
    void displayError(const std::string& message, std::ostream& out) const;
};
