#include "metric_impl/parameters_count.hpp"

#include <gtest/gtest.h>

#include "metric_impl/tests/base.hpp"

namespace analyser::metric::metric_impl {

class CountParametersTest : public MetricGTestBase {
public:
    static inline const analyser::metric::metric_impl::CountParametersMetric metric_instance{};

    CountParametersTest() : MetricGTestBase(metric_instance) {}
};

TEST_P(CountParametersTest, Files) { RunMetricTest(); }

INSTANTIATE_TEST_SUITE_P(
    CountParametersSuite, CountParametersTest,
    ::testing::Values(MetricTestParam{"tests/files/comments.py", 3}, MetricTestParam{"tests/files/exceptions.py", 0},
                      MetricTestParam{"tests/files/if.py", 1}, MetricTestParam{"tests/files/loops.py", 1},
                      MetricTestParam{"tests/files/many_lines.py", 0},
                      MetricTestParam{"tests/files/many_parameters.py", 5},
                      MetricTestParam{"tests/files/match_case.py", 1}, MetricTestParam{"tests/files/nested_if.py", 2},
                      MetricTestParam{"tests/files/simple.py", 0}, MetricTestParam{"tests/files/ternary.py", 1}));
}  // namespace analyser::metric::metric_impl
