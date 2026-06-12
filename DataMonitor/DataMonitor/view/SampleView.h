#pragma once
#include <optional>
#include <vector>
#include <ostream>
#include "../model/Sample.h"

class SampleView {
public:
    void displayAll(const std::vector<Sample>& samples, std::ostream& out) const;
    void displayOne(const std::optional<Sample>& sample, std::ostream& out) const;
    void displayStock(const std::vector<Sample>& samples, std::ostream& out) const;
    void displayError(const std::string& message, std::ostream& out) const;
};
