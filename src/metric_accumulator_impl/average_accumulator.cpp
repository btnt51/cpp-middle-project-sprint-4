#include "metric_accumulator_impl/average_accumulator.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyser::metric_accumulator::metric_accumulator_impl {
void AverageAccumulator::Accumulate(const metric::MetricResult &metric_result) {
    if (is_finalized)
        return;
    sum += metric_result.value;
    count++;
}

void AverageAccumulator::Finalize() {
    if (sum == 0 and count == 0)
        throw std::runtime_error("No accumalated metric found");
    average = sum != 0 ? static_cast<double>(sum) / static_cast<double>(count) : 0.0;
    is_finalized = true;
}

void AverageAccumulator::Reset() {
    average = 0.0;
    count = 0;
    sum = 0;
    is_finalized = false;
}

double AverageAccumulator::Get() const {
    if (not is_finalized)
        return 0;
    return average;
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
