#pragma once
#include "IOrderRepository.h"
#include <string>

class JsonOrderRepository : public IOrderRepository {
public:
    explicit JsonOrderRepository(std::string filePath);
    std::vector<Order> getAll() override;

private:
    std::string filePath_;
};
