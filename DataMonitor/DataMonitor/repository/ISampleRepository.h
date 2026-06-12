#pragma once
#include <optional>
#include <string>
#include <vector>
#include "../model/Sample.h"

class ISampleRepository {
public:
    virtual ~ISampleRepository() = default;
    virtual std::vector<Sample>       getAll() = 0;
    virtual std::optional<Sample>     findById(const std::string& id) = 0;
};
