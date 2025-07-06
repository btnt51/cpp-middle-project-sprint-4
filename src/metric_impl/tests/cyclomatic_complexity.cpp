#include "metric_impl/cyclomatic_complexity.hpp"

#include <metric_impl/tests/base.hpp>

namespace analyser::metric::metric_impl {

class CyclomaticComplexityTest : public MetricGTestBase {
public:
    static inline const analyser::metric::metric_impl::CyclomaticComplexityMetric metric_instance{};

    CyclomaticComplexityTest() : MetricGTestBase(metric_instance) {}
};

TEST_P(CyclomaticComplexityTest, Files) {
    RunMetricTest();
}

INSTANTIATE_TEST_SUITE_P(
    CyclomaticComplexitySuite,
    CyclomaticComplexityTest,
    ::testing::Values(
        MetricTestParam{"tests/files/comments.py", 1},
        MetricTestParam{"tests/files/exceptions.py", 2},
        MetricTestParam{"tests/files/if.py", 2},
        MetricTestParam{"tests/files/loops.py", 4},
        MetricTestParam{"tests/files/many_lines.py", 1},
        MetricTestParam{"tests/files/many_parameters.py", 1},
        MetricTestParam{"tests/files/match_case.py", 4},
        MetricTestParam{"tests/files/nested_if.py", 4},
        MetricTestParam{"tests/files/simple.py", 1},
        MetricTestParam{"tests/files/ternary.py", 3}
    )
);
}  // namespace analyser::metric::metric_impl
