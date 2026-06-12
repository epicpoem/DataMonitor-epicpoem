#pragma once
#include "IOrderRepository.h"
#include <string>

class JsonOrderRepository : public IOrderRepository {
public:
    explicit JsonOrderRepository(std::string filePath);
    std::vector<Order>    getAll() override;
    std::optional<Order>  findById(const std::string& id) override;

private:
    std::string filePath_;
};
