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

#include "analyse.hpp"
#include "cmd_options.hpp"
#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"
#include "metric_accumulator_impl/accumulators.hpp"
#include "metric_impl/metrics.hpp"

namespace metric_acm = analyser::metric_accumulator::metric_accumulator_impl;
namespace metric_impl = analyser::metric::metric_impl;

int main(int argc, char *argv[]) {
    try {
        analyser::cmd::ProgramOptions options;
        options.Parse(argc, argv);
        auto files = options.GetFiles();
        if (files.empty()) {
            throw std::runtime_error("No files specified");
        }

        analyser::metric::MetricExtractor metric_extractor;
        metric_extractor.RegisterMetric(std::make_unique<metric_impl::CodeLinesCountMetric>());
        metric_extractor.RegisterMetric(std::make_unique<metric_impl::CyclomaticComplexityMetric>());
        metric_extractor.RegisterMetric(std::make_unique<metric_impl::CountParametersMetric>());

        // запустите analyser::AnalyseFunctions
        auto analysis_results = analyser::AnalyseFunctions(files, metric_extractor);

        std::ranges::for_each(analysis_results, [&](auto &pair) {
            std::println("Function: {} ", pair.first);
            std::ranges::for_each(pair.second, [&](auto &metric) {
                std::println("Metrics Results: (metric_name: {}, metric value: {}) ", metric.metric_name, metric.value);
            });
        });


        // зарегистрируйте аккумуляторы метрик в accumulator
        analyser::metric_accumulator::MetricsAccumulator accumulator;
        accumulator.RegisterAccumulator("CodeLinesCountMetric", std::make_unique<metric_acm::AverageAccumulator>());
        accumulator.RegisterAccumulator("CyclomaticComplexityMetric", std::make_unique<metric_acm::SumAverageAccumulator>());
        accumulator.RegisterAccumulator("CountParametersMetric", std::make_unique<metric_acm::AverageAccumulator>());
        accumulator.ResetAccumulators();

        auto print_accumalted_data = [&]() {
            auto result_for_accumulator = accumulator.GetFinalizedAccumulator<metric_acm::AverageAccumulator>("CodeLinesCountMetric").Get();
            std::println("Accumulator Results: (accumulator_name: {}, value: {}) ", "CodeLinesCountMetric", result_for_accumulator);
            auto sum_result_for_accumulator = accumulator.GetFinalizedAccumulator<metric_acm::SumAverageAccumulator>("CyclomaticComplexityMetric").Get();
            std::println("Accumulator Results: (accumulator_name: {}, value: {}) ", "CyclomaticComplexityMetric", sum_result_for_accumulator);
            result_for_accumulator = accumulator.GetFinalizedAccumulator<metric_acm::AverageAccumulator>("CountParametersMetric").Get();
            std::println("Accumulator Results: (accumulator_name: {}, value: {}) ", "CountParametersMetric", result_for_accumulator);
        };

        std::println();
        std::println();
        std::println();
        std::println();
        std::println("Split by files");
        auto splitted_by_files = analyser::SplitByFiles(analysis_results);
        std::ranges::for_each(splitted_by_files, [&](const auto &group) {
            analyser::AccumulateFunctionAnalysis(group, accumulator);
            const auto& filename = group[0].first.filename;
            std::println("Accumulated Analysis for file: {}", filename);
            print_accumalted_data();
        });

        accumulator.ResetAccumulators();

        std::println();
        std::println();
        std::println();
        std::println();
        std::println("Split by class");

        auto splitted_by_classes = analyser::SplitByClasses(analysis_results);
        std::ranges::for_each(splitted_by_classes, [&](const auto & group) {
            analyser::AccumulateFunctionAnalysis(group, accumulator);
            const auto& class_name = group[0].first.class_name.value_or("unknown");
            std::println("Accumulated Analysis for class: {}", class_name);

            print_accumalted_data();
        });
        accumulator.ResetAccumulators();

        analyser::AccumulateFunctionAnalysis(analysis_results, accumulator);
        std::println();
        std::println();
        std::println();
        std::println();
        std::println("Overal statistic");
        std::ranges::for_each(analysis_results, [&](const auto &pair) {
            print_accumalted_data();
        });

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
