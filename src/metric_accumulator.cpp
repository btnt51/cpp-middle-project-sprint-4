#include "metric_accumulator.hpp"

#include <unistd.h>

#include <algorithm>
#include <any>
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

namespace analyser::metric_accumulator {

void MetricsAccumulator::AccumulateNextFunctionResults(const std::vector<metric::MetricResult> &metric_results) const {
    std::ranges::for_each(metric_results, [&](const auto &metric_result) {
        accumulators.at(metric_result.metric_name)->Accumulate(metric_result);
    });
}

void MetricsAccumulator::ResetAccumulators() {
    std::ranges::for_each(accumulators | std::views::values, [](auto &acc) { acc->Reset(); });
}

}  // namespace analyser::metric_accumulator
