#pragma once

#include <optional>
#include <string_view>
#include <vector>

namespace builtin {
    std::optional<std::pair<std::string_view, std::string_view>> splitFunction(const std::string_view& key);
}
