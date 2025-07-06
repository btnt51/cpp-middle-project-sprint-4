#pragma once
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

#include "metric_accumulator.hpp"

namespace analyser::metric_accumulator::metric_accumulator_impl {

struct SumAverageAccumulator: public IAccumulator {
    struct SumAverage {
        int sum;
        double average;
        auto operator<=>(const SumAverage&) const = default;
    };
    void Accumulate(const metric::MetricResult& metric_result) override;

    virtual void Finalize() override;

    virtual void Reset() override;

    SumAverage Get() const;

private:
    int sum = 0;
    int count = 0;
    double average = 0;
};

} // namespace analyser::metric_accumulator::metric_accumulator_impl

namespace std {
    template <>
    struct formatter<analyser::metric_accumulator::metric_accumulator_impl::SumAverageAccumulator::SumAverage, char> {
        template <typename FormatContext>
        auto format(const analyser::metric_accumulator::metric_accumulator_impl::SumAverageAccumulator::SumAverage& sum, FormatContext &fc) const {
            return format_to(fc.out(), "Sum: {}, Average: {}", sum.sum, sum.average);
        }

        constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
    };
}
