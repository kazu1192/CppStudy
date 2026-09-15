#pragma once
#include "vec2.h"

struct AABB {
    Vec2 min; // 左上
    Vec2 max; // 右下

    constexpr AABB() = default;
    constexpr AABB(Vec2 pos, Vec2 size) : min(pos), max(pos + size) {}

    constexpr Vec2 size() const { return max - min; }
    constexpr Vec2 center() const { return (max + min) * 0.5f; }
};

// 重なっているか
inline bool intersects(const AABB& a, const AABB& b) {
    if (a.max.x < b.min.x || a.min.x > b.max.x)
        return false;
    if (a.max.y < b.min.y || a.min.y > b.max.y)
        return false;
    return true;
}

// 点が含まれるか
inline bool contains(const AABB& box, const Vec2& p) {
    return p.x >= box.min.x && p.x <= box.max.x && p.y >= box.min.y && p.y <= box.max.y;
}
