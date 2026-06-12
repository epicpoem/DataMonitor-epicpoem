#pragma once
#include <vector>
#include "../model/Sample.h"

class ISampleRepository {
public:
    virtual ~ISampleRepository() = default;
    virtual std::vector<Sample> getAll() = 0;
};
