#pragma once

#include <cstdint>
#include <string>
#include <vector>

std::pair<std::vector<std::string>, std::int64_t> generateDecibinaries(
        std::int64_t value,
        bool only_count = false);
