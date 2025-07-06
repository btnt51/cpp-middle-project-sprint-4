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

#include "file.hpp"

namespace fs = std::filesystem;
namespace rv = std::ranges::views;
namespace rs = std::ranges;

namespace analyser::function {

struct Function {
    std::string filename;
    std::optional<std::string> class_name;
    std::string name;
    std::string ast;
};

struct FunctionExtractor {
    std::vector<Function> Get(const analyser::file::File& file);

private:
    struct Position {
        size_t line;
        size_t col;
    };

    struct FunctionNameLocation {
        Position start;
        Position end;
        std::string name;
    };

    struct ClassInfo {
        std::string name;
        Position start;
        Position end;
    };

    FunctionNameLocation GetNameLocation(const std::string& function_ast);
    std::string GetNameFromSource(const std::string& function_ast,
                                  const std::vector<std::string>& lines);
    std::optional<ClassInfo> FindEnclosingClass(const std::string& ast,
                                                const FunctionNameLocation& func_loc);
    std::string GetClassNameFromSource(const ClassInfo& class_info,
                                       const std::vector<std::string>& lines);
};

} // namespace analyser::function

namespace std {
    template <>
    struct formatter<analyser::function::Function, char> {
        template <typename FormatContext>
        auto format(const analyser::function::Function func, FormatContext &fc) const {
            auto className = func.class_name.value_or("unknown");

            return format_to(fc.out(), "Filename: {}, classname: {}, name: {}, ast:\n{}", func.filename, className, func.name, func.ast);
        }

        constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
    };
}