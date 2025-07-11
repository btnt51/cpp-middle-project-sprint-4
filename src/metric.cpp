#include "metric.hpp"

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

#include "function.hpp"

namespace analyser::metric {

void MetricExtractor::RegisterMetric(std::unique_ptr<IMetric> metric) { metrics.push_back(std::move(metric)); }

MetricResults MetricExtractor::Get(const function::Function &func) const {
    MetricResults result;
    result.reserve(metrics.size());
    std::ranges::for_each(metrics, [&func, &result](auto &&metric) { result.emplace_back(metric->Calculate(func)); });
    return result;
}

}  // namespace analyser::metric
