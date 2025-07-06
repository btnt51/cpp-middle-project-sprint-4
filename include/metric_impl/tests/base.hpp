#include "metric_impl/code_lines_count.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

struct MetricTestParam {
    std::string file_path;
    int expected_value;
};

class MetricGTestBase : public ::testing::TestWithParam<MetricTestParam> {
protected:
    const IMetric& metric;

    explicit MetricGTestBase(const IMetric& m) : metric(m) {}

    void RunMetricTest() {
        auto param = GetParam();

        analyser::file::File file(param.file_path); // предположим ты можешь так парсить
        analyser::function::FunctionExtractor extractor;
        const auto functions = extractor.Get(file);
        const MetricResult result = metric.Calculate(functions.front());
        EXPECT_EQ(result.value, param.expected_value) << "File: " << param.file_path;
    }
};
}