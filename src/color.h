#pragma once
#include <cstdint>

struct Color {
    std::uint8_t r = 0;
    std::uint8_t g = 0;
    std::uint8_t b = 0;
    std::uint8_t a = 255;
};

namespace colors {
inline constexpr Color white{255, 255, 255};
inline constexpr Color black{0, 0, 0};
inline constexpr Color red{220, 80, 80};
inline constexpr Color blue{80, 140, 220};
inline constexpr Color yellow{230, 200, 90};
inline constexpr Color bg{30, 30, 40};
} // namespace colors
