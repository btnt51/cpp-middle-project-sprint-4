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

namespace rv = rs::views;
namespace rs = rs;

auto AnalyseFunctions(const std::vector<std::string> &files,
                      const analyser::metric::MetricExtractor &metric_extractor) {
    analyser::function::FunctionExtractor funcExtractor;
    return files | rv::transform([](const auto &file_path) { return analyser::file::File(file_path); }) |
           rv::transform([&](const auto &file) { return funcExtractor.Get(file); }) | rv::join |
           rv::transform([&](const auto &function) {
               auto metrics = metric_extractor.Get(function);
               return std::make_pair(function, metrics);
           }) |
           rs::to<std::vector>();
}

auto SplitByClasses(const auto &analysis) {
    auto grouped = analysis | rv::chunk_by([](const auto &pair1, const auto &pair2) {
                       return pair1.first.class_name == pair2.first.class_name;
                   });
    return grouped;
}

auto SplitByFiles(const auto &analysis) {
    auto grouped = analysis | rv::chunk_by([](const auto &pair1, const auto &pair2) {
                       return pair1.first.filename == pair2.first.filename;
                   });
    return grouped;
}

void AccumulateFunctionAnalysis(const auto &analysis,
                                const analyser::metric_accumulator::MetricsAccumulator &accumulator) {

    rs::for_each(analysis | rv::values,
                 [&](const auto &metric_results) { accumulator.AccumulateNextFunctionResults(metric_results); });
}

}  // namespace analyser
