#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"

namespace analyser {

namespace rv = std::ranges::views;
namespace rs = std::ranges;

auto AnalyseFunctions(const std::vector<std::string> &files,
                      const analyser::metric::MetricExtractor &metric_extractor) {
    std::vector<std::pair<analyser::function::Function, analyser::metric::MetricResults>> res;
    analyser::function::FunctionExtractor funcExtractor;
    auto all_functions =
        files | std::views::transform([](const auto &file_path) { return analyser::file::File(file_path); }) |
        std::views::transform([&](const auto &file) { return funcExtractor.Get(file); }) | std::views::join;

    std::ranges::for_each(all_functions, [&](const auto &function) {
        auto metrics = metric_extractor.Get(function);
        res.push_back(std::make_pair(function, metrics));
    });

    return res;
}

auto SplitByClasses(const auto &analysis) {
    auto grouped =
        analysis |
        std::views::chunk_by([](const std::pair<analyser::function::Function, analyser::metric::MetricResults> pair1,
                                const std::pair<analyser::function::Function, analyser::metric::MetricResults> pair2) {
            return pair1.first.class_name < pair2.first.class_name;
        }) |
        std::views::transform([](auto group) { return group | std::ranges::to<std::vector>(); }) |
        std::ranges::to<std::vector>();
    return grouped;
}

auto SplitByFiles(const auto &analysis) {
    auto grouped =
        analysis |
        std::views::chunk_by([](const std::pair<analyser::function::Function, analyser::metric::MetricResults> pair1,
                                const std::pair<analyser::function::Function, analyser::metric::MetricResults> pair2) {
            return pair1.first.filename < pair2.first.filename;
        }) |
        std::views::transform([](auto group) { return group | std::ranges::to<std::vector>(); }) |
        std::ranges::to<std::vector>();
    return grouped;
}

void AccumulateFunctionAnalysis(const auto &analysis,
                                const analyser::metric_accumulator::MetricsAccumulator &accumulator) {
    std::ranges::for_each(
        analysis, [&](const std::pair<analyser::function::Function, analyser::metric::MetricResults> &metric_results) {
            accumulator.AccumulateNextFunctionResults(metric_results.second);
        });
}

}  // namespace analyser
