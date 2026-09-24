// 数学ユーティリティのテスト。外部フレームワークには依存しない。
// constexpr にできるものは static_assert で、コンパイル時に検証する。
#include "aabb.h"
#include "vec2.h"
#include "vec3.h"
#include <cmath>
#include <cstdio>

static int g_failures = 0;

#define CHECK(expr)                                                                                \
    do {                                                                                           \
        if (!(expr)) {                                                                             \
            std::printf("  FAIL %s:%d: %s\n", __FILE__, __LINE__, #expr);                          \
            ++g_failures;                                                                          \
        }                                                                                          \
    } while (0)

static bool nearly(float a, float b, float eps = 1e-5f) { return std::fabs(a - b) < eps; }

// --- Vec2 -------------------------------------------------------------------
// constexpr なので、これらはすべてコンパイル時に検証される
static_assert((Vec2{1, 2} + Vec2{3, 4}).x == 4.0f);
static_assert((Vec2{1, 2} + Vec2{3, 4}).y == 6.0f);
static_assert((Vec2{5, 7} - Vec2{1, 2}).x == 4.0f);
static_assert((Vec2{3, 4} * 2.0f).y == 8.0f);
static_assert((2.0f * Vec2{3, 4}).x == 6.0f); // スカラー乗算の対称性
static_assert((-Vec2{1, -2}).y == 2.0f);
static_assert(dot(Vec2{1, 2}, Vec2{3, 4}) == 11.0f);
static_assert(cross(Vec2{1, 0}, Vec2{0, 1}) == 1.0f);
static_assert(Vec2{3, 4}.lengthSquared() == 25.0f);

static void testVec2() {
    std::printf("Vec2\n");
    CHECK(nearly(Vec2(3, 4).length(), 5.0f));

    const Vec2 n = Vec2(3, 4).normalized();
    CHECK(nearly(n.length(), 1.0f));
    CHECK(nearly(n.x, 0.6f));
    CHECK(nearly(n.y, 0.8f));

    // ゼロベクトルの正規化はゼロ除算せずゼロを返す
    const Vec2 z = Vec2{}.normalized();
    CHECK(z.x == 0.0f && z.y == 0.0f);
}

// --- AABB -------------------------------------------------------------------
static_assert(AABB(Vec2{10, 20}, Vec2{30, 40}).max.x == 40.0f);
static_assert(AABB(Vec2{10, 20}, Vec2{30, 40}).size().y == 40.0f);
static_assert(AABB(Vec2{0, 0}, Vec2{10, 10}).center().x == 5.0f);

// 重なり
static_assert(intersects(AABB(Vec2{0, 0}, Vec2{10, 10}), AABB(Vec2{5, 5}, Vec2{10, 10})));
// 完全内包
static_assert(intersects(AABB(Vec2{0, 0}, Vec2{10, 10}), AABB(Vec2{2, 2}, Vec2{3, 3})));
// 辺で接している場合は重なりとみなす（境界含む判定）
static_assert(intersects(AABB(Vec2{0, 0}, Vec2{10, 10}), AABB(Vec2{10, 0}, Vec2{5, 5})));
// x 方向に離れている
static_assert(!intersects(AABB(Vec2{0, 0}, Vec2{10, 10}), AABB(Vec2{20, 0}, Vec2{5, 5})));
// y 方向に離れている（x だけ見ていると取りこぼす）
static_assert(!intersects(AABB(Vec2{0, 0}, Vec2{10, 10}), AABB(Vec2{0, 20}, Vec2{5, 5})));

static_assert(contains(AABB(Vec2{0, 0}, Vec2{10, 10}), Vec2{5, 5}));
static_assert(contains(AABB(Vec2{0, 0}, Vec2{10, 10}), Vec2{0, 0})); // 角は含む
static_assert(!contains(AABB(Vec2{0, 0}, Vec2{10, 10}), Vec2{11, 5}));

static void testAabb() {
    std::printf("AABB\n");
    // 交差判定が対称であること
    const AABB a(Vec2{0, 0}, Vec2{10, 10});
    const AABB b(Vec2{5, 5}, Vec2{10, 10});
    CHECK(intersects(a, b) == intersects(b, a));

    const AABB distant(Vec2{100, 100}, Vec2{1, 1});
    CHECK(intersects(a, distant) == intersects(distant, a));
}

// --- Vec3 -------------------------------------------------------------------
static_assert((Vec3{1, 2, 3} + Vec3{4, 5, 6}) == Vec3{5, 7, 9});
static_assert((Vec3{1, 2, 3} * 2.0f) == Vec3{2, 4, 6});
static_assert(dot(Vec3{1, 2, 3}, Vec3{4, 5, 6}) == 32.0f);
static_assert(cross(Vec3{1, 0, 0}, Vec3{0, 1, 0}) == Vec3{0, 0, 1}); // 右手系
static_assert(Vec3{3, 4, 0}.lengthSquared() == 25.0f);
static_assert(nearlyEqual(Vec3{1, 2, 3}, Vec3{1, 2, 3}));

static void testVec3() {
    std::printf("Vec3\n");
    CHECK(nearly(Vec3(3, 4, 0).length(), 5.0f));
    CHECK(nearly(Vec3(3, 4, 0).normalized().length(), 1.0f));

    const Vec3 z = Vec3{}.normalized();
    CHECK(z == Vec3{});
}

int main() {
    testVec2();
    testAabb();
    testVec3();

    if (g_failures == 0) {
        std::printf("\nすべてのテストが成功しました\n");
        return 0;
    }
    std::printf("\n%d 件のテストが失敗しました\n", g_failures);
    return 1;
}
