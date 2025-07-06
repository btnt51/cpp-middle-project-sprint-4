#include "metric_accumulator_impl/sum_average_accumulator.hpp"

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

void SumAverageAccumulator::Accumulate(const metric::MetricResult &metric_result) {
    if (is_finalized)
        return;
    sum += metric_result.value;
    count++;
}

void SumAverageAccumulator::Finalize() {
    if (sum == 0 and count == 0)
        throw std::runtime_error("No accumalated metric found");
    average = sum != 0 ? static_cast<double>(sum) / static_cast<double>(count) : 0.0;
    is_finalized = true;
}

void SumAverageAccumulator::Reset() {
    average = 0.0;
    count = 0;
    sum = 0;
    is_finalized = false;
}

SumAverageAccumulator::SumAverage SumAverageAccumulator::Get() const {
    if (not is_finalized) {
        return {0, 0.0};
    }
    return SumAverageAccumulator::SumAverage(sum, average);
}
}  // namespace analyser::metric_accumulator::metric_accumulator_impl
