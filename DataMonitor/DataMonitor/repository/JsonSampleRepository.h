#pragma once
#include "ISampleRepository.h"
#include <string>

class JsonSampleRepository : public ISampleRepository {
public:
    explicit JsonSampleRepository(std::string filePath);
    std::vector<Sample>    getAll() override;
    std::optional<Sample>  findById(const std::string& id) override;

private:
    std::string filePath_;
};
