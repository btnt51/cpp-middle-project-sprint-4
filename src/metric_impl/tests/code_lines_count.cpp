#include <metric_impl/tests/base.hpp>

namespace analyser::metric::metric_impl {

class CodeLinesCountTest : public MetricGTestBase {
public:
    static inline const analyser::metric::metric_impl::CodeLinesCountMetric metric_instance{};

    CodeLinesCountTest() : MetricGTestBase(metric_instance) {}
};

TEST_P(CodeLinesCountTest, Files) { RunMetricTest(); }

INSTANTIATE_TEST_SUITE_P(
    CodeLinesCountSuite, CodeLinesCountTest,
    ::testing::Values(MetricTestParam{"tests/files/comments.py", 4}, MetricTestParam{"tests/files/exceptions.py", 8},
                      MetricTestParam{"tests/files/if.py", 4}, MetricTestParam{"tests/files/loops.py", 7},
                      MetricTestParam{"tests/files/many_lines.py", 12},
                      MetricTestParam{"tests/files/many_parameters.py", 2},
                      MetricTestParam{"tests/files/match_case.py", 8}, MetricTestParam{"tests/files/nested_if.py", 9},
                      MetricTestParam{"tests/files/simple.py", 6}, MetricTestParam{"tests/files/ternary.py", 2}));
}  // namespace analyser::metric::metric_impl
