#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(SumAverageAccumulatorTest, SingleValue) {
    SumAverageAccumulator acc;
    acc.Accumulate({.value = 10});
    acc.Finalize();
    auto result = acc.Get();
    EXPECT_EQ(result.sum, 10);
    EXPECT_DOUBLE_EQ(result.average, 10.0);
}

TEST(SumAverageAccumulatorTest, MultipleValues) {
    SumAverageAccumulator acc;
    acc.Accumulate({.value = 5});
    acc.Accumulate({.value = 15});
    acc.Accumulate({.value = 20});
    acc.Finalize();
    auto result = acc.Get();
    EXPECT_EQ(result.sum, 40);  // 5 + 15 + 20
    EXPECT_DOUBLE_EQ(result.average, 40.0 / 3.0);
}

TEST(SumAverageAccumulatorTest, ResetWorks) {
    SumAverageAccumulator acc;
    acc.Accumulate({.value = 1});
    acc.Accumulate({.value = 2});
    acc.Finalize();
    acc.Reset();
    auto result = acc.Get();
    EXPECT_EQ(result.sum, 0);
    EXPECT_DOUBLE_EQ(result.average, 0.0);
}

TEST(SumAverageAccumulatorTest, FinalizeWithNoData) {
    SumAverageAccumulator acc;

    EXPECT_THROW(acc.Finalize(), std::runtime_error);
}

TEST(SumAverageAccumulatorTest, NegativeAndZero) {
    SumAverageAccumulator acc;
    acc.Accumulate({.value = 0});
    acc.Accumulate({.value = -5});
    acc.Accumulate({.value = 5});
    acc.Finalize();
    auto result = acc.Get();
    EXPECT_EQ(result.sum, 0);  // 0 + (-5) + 5
    EXPECT_DOUBLE_EQ(result.average, 0.0);
}
}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
