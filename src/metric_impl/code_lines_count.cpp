#include "metric_impl/code_lines_count.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "utils.hpp"

namespace analyser::metric::metric_impl {

    // здесь ваш код
std::string CodeLinesCountMetric::Name() const {
    return "CodeLinesCountMetric";
}

MetricResult::ValueType CodeLinesCountMetric::CalculateImpl(const function::Function &f) const {
    auto ast = f.ast | std::views::split('\n')
    | std::views::transform([](auto&& rng) {
        return std::string_view(&*rng.begin(), std::ranges::distance(rng));
    })
    | std::ranges::to<std::vector>();


    auto is_statement      = [](std::string_view s) {
        return s.contains("_statement");
    };

    auto count_parentheses = [](std::string_view s) {
        return std::ranges::count(s, '(')
             - std::ranges::count(s, ')');
    };
    bool flag = false;

    auto blocks_view = ast | std::views::all
        | std::views::drop_while([&flag , &is_statement](const auto& s)  {
          if (flag) return false;
          if (is_statement(s)) { flag = true; return false; }
          return true;
      })
      | std::views::chunk_by([bal = 0, &count_parentheses](
                               std::string_view /*prev*/,
                               std::string_view cur
                             ) mutable {
            bal += count_parentheses(cur);
            return bal > 0;
        })
      | std::views::filter([&is_statement](auto&& chunk) {
            return is_statement(*chunk.begin());
        });

    auto block_size = [&](auto&& block) {
        auto head    = block.front();
        auto open1   = head.find('[');
        auto comma1  = head.find(',', open1);
        int  start   = ToInt(head.substr(open1+1, comma1-open1-1));

        auto open2   = head.find('[', comma1);
        auto comma2  = head.find(',', open2);
        int  end     = ToInt(head.substr(open2+1, comma2-open2-1));

        return end - start + 1;  // включительно
    };

    int total = std::ranges::fold_left(
                    blocks_view
                  | std::views::transform(block_size),
                    1, std::plus<>{});

    return total;
}

} // namespace analyser::metric::metric_impl
