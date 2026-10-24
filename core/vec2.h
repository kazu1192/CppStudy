#pragma once
#include <cmath>

// 2次元ベクトル。位置・速度・サイズの表現に使う。
// 演算子は constexpr にしてあり、コンパイル時評価（static_assert）でテストできる。
struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;

    constexpr Vec2() = default;
    constexpr Vec2(float x_, float y_) : x(x_), y(y_) {}

    constexpr Vec2& operator+=(const Vec2& r) {
        x += r.x;
        y += r.y;
        return *this;
    }

    constexpr Vec2& operator-=(const Vec2& r) {
        x -= r.x;
        y -= r.y;
        return *this;
    }

    constexpr Vec2& operator*=(float s) {
        x *= s;
        y *= s;
        return *this;
    }

    constexpr Vec2 operator-() const { return Vec2{-x, -y}; }

    constexpr float lengthSquared() const { return x * x + y * y; }
    float length() const { return std::sqrt(lengthSquared()); }

    // 長さ 1 に揃えたベクトル。ゼロベクトルはゼロ除算を避けてゼロのまま返す。
    Vec2 normalized() const {
        float len = length();
        if (len == 0.0f)
            return Vec2{};
        return Vec2{x / len, y / len};
    }
};

// --- 二項演算子 ---
constexpr Vec2 operator+(Vec2 l, const Vec2& r) { return l += r; }
constexpr Vec2 operator-(Vec2 l, const Vec2& r) { return l -= r; }
constexpr Vec2 operator*(Vec2 v, float s) { return v *= s; }
constexpr Vec2 operator*(float s, Vec2 v) { return v *= s; }

// --- 内積・外積 ---
constexpr float dot(const Vec2& l, const Vec2& r) { return l.x * r.x + l.y * r.y; }

// 2D の外積はスカラー（z 成分）になる。符号で l から r への回転方向が分かる。
constexpr float cross(const Vec2& l, const Vec2& r) { return l.x * r.y - l.y * r.x; }
