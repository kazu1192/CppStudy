#pragma once
#include <cmath>

struct Vec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    constexpr Vec3() = default;
    constexpr Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    Vec3& operator+=(const Vec3& r) {
        x += r.x;
        y += r.y;
        z += r.z;
        return *this;
    }

    Vec3& operator-=(const Vec3& r) {
        x -= r.x;
        y -= r.y;
        z -= r.z;
        return *this;
    }

    Vec3& operator*=(float s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    constexpr Vec3 operator-() const { return Vec3{-x, -y, -z}; }

    float lengthSquared() const { return x * x + y * y + z * z; }
    float length() const { return std::sqrt(lengthSquared()); }

    Vec3 normalized() const {
        float len = length();
        if (len == 0.0f)
            return Vec3{};
        return Vec3{x / len, y / len, z / len};
    }
};

// --- 二項演算子 ---
inline Vec3 operator+(Vec3 l, const Vec3& r) { return l += r; }
inline Vec3 operator-(Vec3 l, const Vec3& r) { return l -= r; }
inline Vec3 operator*(Vec3 v, float s) { return v *= s; }
inline Vec3 operator*(float s, Vec3 v) { return v *= s; }

// --- 比較 ---
inline bool operator==(const Vec3& l, const Vec3& r) {
    return l.x == r.x && l.y == r.y && l.z == r.z;
}

inline bool nearlyEqual(const Vec3& l, const Vec3& r, float eps = 1e-5f) {
    return (l - r).lengthSquared() < eps * eps;
}

// --- 内積・外積 ---
inline float dot(const Vec3& l, const Vec3& r) { return l.x * r.x + l.y * r.y + l.z * r.z; }

inline Vec3 cross(const Vec3& l, const Vec3& r) {
    return Vec3{l.y * r.z - l.z * r.y, l.z * r.x - l.x * r.z, l.x * r.y - l.y * r.x};
}
