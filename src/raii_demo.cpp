// RAII とムーブセマンティクスの練習用デモ。
// fake_gl.h の偽 OpenGL がテクスチャIDの生存を追跡するので、
// ムーブが所有権を正しく移せていればリーク・二重解放ともに報告されない。
#include "fake_gl.h"
#include "texture.h"
#include "vec3.h"
#include <cstdio>
#include <utility>

int main() {
    std::printf("=== Texture (RAII + ムーブ) ===\n");
    {
        Texture a{"move_ctor"};
        Texture b = std::move(a);
        std::printf("  ムーブ構築後: a.id=%u, b.id=%u\n", a.id(), b.id());

        Texture c{"move_assign"};
        c = std::move(b);
        std::printf("  ムーブ代入後: b.id=%u, c.id=%u\n", b.id(), c.id());
    }
    reportLeaks();

    std::printf("\n=== Vec3 ===\n");
    std::printf("  sizeof(Vec3) = %zu バイト\n", sizeof(Vec3));

    constexpr Vec3 a{1.0f, 2.0f, 3.0f};
    constexpr Vec3 b{4.0f, 5.0f, 6.0f};

    constexpr Vec3 c = a + b;
    std::printf("  a + b = (%.1f, %.1f, %.1f)\n", c.x, c.y, c.z);

    constexpr Vec3 d = a * 2.0f;
    std::printf("  a * 2 = (%.1f, %.1f, %.1f)\n", d.x, d.y, d.z);

    constexpr Vec3 e = 2.0f * a; // 対称性の確認
    std::printf("  2 * a = (%.1f, %.1f, %.1f)\n", e.x, e.y, e.z);

    std::printf("  dot(a,b) = %.1f\n", dot(a, b));

    constexpr Vec3 f = cross(Vec3{1, 0, 0}, Vec3{0, 1, 0});
    std::printf("  cross(x,y) = (%.1f, %.1f, %.1f)\n", f.x, f.y, f.z);

    Vec3 g{3.0f, 4.0f, 0.0f};
    std::printf("  length = %.1f\n", g.length());

    Vec3 h = g.normalized();
    std::printf("  normalized = (%.2f, %.2f, %.2f), 長さ %.2f\n", h.x, h.y, h.z, h.length());

    // 数式らしく書けることの確認（すべてコンパイル時に畳まれる）
    constexpr Vec3 result = (a + b) * 0.5f - c;
    std::printf("  (a+b)*0.5-c = (%.1f, %.1f, %.1f)\n", result.x, result.y, result.z);

    return 0;
}
