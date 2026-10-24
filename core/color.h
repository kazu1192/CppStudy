#pragma once
#include <cstdint>

// 8bit x RGBA の色。a は不透明度で、既定は不透明（255）。
struct Color {
    std::uint8_t r = 0;
    std::uint8_t g = 0;
    std::uint8_t b = 0;
    std::uint8_t a = 255;
};

// よく使う色の定義。bg は画面クリア用の背景色。
namespace colors {
inline constexpr Color white{255, 255, 255};
inline constexpr Color black{0, 0, 0};
inline constexpr Color red{220, 80, 80};
inline constexpr Color blue{80, 140, 220};
inline constexpr Color yellow{230, 200, 90};
inline constexpr Color bg{30, 30, 40};
} // namespace colors
