#include "metric_accumulator_impl/average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(AverageAccumulatorTest, AverageOfOneValue) {
    AverageAccumulator acc;
    acc.Accumulate({.value = 42});
    acc.Finalize();
    EXPECT_DOUBLE_EQ(acc.Get(), 42.0);
}

TEST(AverageAccumulatorTest, AverageOfSeveralValues) {
    AverageAccumulator acc;
    acc.Accumulate({.value = 10});
    acc.Accumulate({.value = 20});
    acc.Accumulate({.value = 30});
    acc.Finalize();
    EXPECT_DOUBLE_EQ(acc.Get(), 20.0);
}

TEST(AverageAccumulatorTest, ResetClearsState) {
    AverageAccumulator acc;
    acc.Accumulate({.value = 10});
    acc.Finalize();
    acc.Reset();
    EXPECT_DOUBLE_EQ(acc.Get(), 0.0);
}

TEST(AverageAccumulatorTest, FinalizeWithNoData) {
    AverageAccumulator acc;

    EXPECT_THROW(acc.Finalize(), std::runtime_error);
}

TEST(AverageAccumulatorTest, HandlesNegativeAndZero) {
    AverageAccumulator acc;
    acc.Accumulate({.value = 0});
    acc.Accumulate({.value = -10});
    acc.Accumulate({.value = 10});
    acc.Finalize();
    EXPECT_DOUBLE_EQ(acc.Get(), 0.0);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
