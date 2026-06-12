#pragma once
#include "ISampleRepository.h"
#include <string>

class JsonSampleRepository : public ISampleRepository {
public:
    explicit JsonSampleRepository(std::string filePath);
    std::vector<Sample> getAll() override;

private:
    std::string filePath_;
};
