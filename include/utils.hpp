#pragma once

#include <charconv>
#include <stdexcept>
#include <string>
#include <string_view>

inline int ToInt(std::string_view value) {
    value.remove_prefix(value.find_first_not_of(' '));
    if (auto last_non_space = value.find_last_not_of(' '); last_non_space != std::string_view::npos) {
        value.remove_suffix(value.size() - last_non_space - 1);
    }

    int result{};
    auto [parse_end_ptr, error_code] = std::from_chars(value.begin(), value.end(), result);
    if (error_code != std::errc{} || parse_end_ptr != value.data() + value.size()) {
        throw std::invalid_argument("Cannot convert '" + std::string(value) + "' to integral");
    }
    return result;
}
