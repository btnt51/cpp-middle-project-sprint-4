#include "metric_impl/parameters_count.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyser::metric::metric_impl {

// здесь ваш код

MetricResult::ValueType CountParametersMetric::CalculateImpl(const function::Function &f) const {
    auto ast = f.ast | std::views::split('\n') | std::views::transform([](auto &&rng) {
                   return std::string_view(&*rng.begin(), std::ranges::distance(rng));
               });

    auto start_parametrs =
        std::ranges::find_if(ast, [](const auto &line) { return line.contains("parameters: (parameters"); });

    auto end_parametrs = std::ranges::find_if(ast, [](const auto &line) { return line.contains("body: (block"); });

    if (start_parametrs != ast.end() && end_parametrs != ast.end() && start_parametrs != end_parametrs) {
        auto range = std::ranges::subrange(start_parametrs, end_parametrs);
        auto param_count =
            std::ranges::count_if(range, [](const auto &line) { return line.contains("(identifier ["); });
        return param_count;
    }

    return 0;
}

std::string CountParametersMetric::Name() const { return "CountParametersMetric"; }
}  // namespace analyser::metric::metric_impl
