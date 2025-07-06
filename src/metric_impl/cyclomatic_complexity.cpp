#include "metric_impl/cyclomatic_complexity.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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

std::string CyclomaticComplexityMetric::Name() const { return "CyclomaticComplexityMetric"; }

MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function &f) const {
    constexpr auto baseCyclomaticComplexity = 1;
    auto ast = f.ast | std::views::split('\n') | std::views::transform([](auto &&rng) {
                   return std::string_view(&*rng.begin(), std::ranges::distance(rng));
               });

    static constexpr auto keywords =
        std::to_array<std::string_view>({"if_statement", "for_statement", "while_statement", "case_clause",
                                         "elif_clause", "except_clause", "conditional_expression"});

    return baseCyclomaticComplexity + std::ranges::count_if(ast, [](const auto &x) {
               return std::ranges::any_of(keywords, [&](auto kw) { return x.contains(kw); });
           });
    ;
}

}  // namespace analyser::metric::metric_impl
