#pragma once
#include <cstdio>
#include <set>

inline std::set<unsigned int> &liveTextures() {
  static std::set<unsigned int> s;
  return s;
}

// テクスチャIDを発行する(本物の glGenTextures と同じシグネチャ)
inline void glGenTextures(int n, unsigned int *out) {
  static unsigned int next = 1;
  for (int i = 0; i < n; ++i) {
    out[i] = next++;
    liveTextures().insert(out[i]);
    std::printf("  [GL] texture %u 生成\n", out[i]);
  }
}

// テクスチャIDを解放する
inline void glDeleteTextures(int n, const unsigned int *ids) {
  for (int i = 0; i < n; ++i) {
    if (ids[i] == 0)
      continue; // 0 は「無効なID」の慣習
    if (liveTextures().erase(ids[i]) == 0) {
      std::printf("  [GL] !!! texture %u を二重解放 !!!\n", ids[i]);
    } else {
      std::printf("  [GL] texture %u 解放\n", ids[i]);
    }
  }
}

// プログラム終了時にリークを報告する
inline void reportLeaks() {
  if (liveTextures().empty()) {
    std::printf("\n[GL] リークなし ✓\n");
  } else {
    for (auto id : liveTextures()) {
      std::printf("\n[GL] !!! texture %u がリークしています !!!\n", id);
    }
  }
}

// --- エンティティのリソース追跡用 ---
inline std::set<unsigned int> &liveResources() {
  static std::set<unsigned int> s;
  return s;
}

// 派生クラスが持つ「重いリソース」の代わり
class HeavyResource {
public:
  explicit HeavyResource(const char *owner) : owner_(owner) {
    static unsigned int next = 100;
    id_ = next++;
    liveResources().insert(id_);
    std::printf("  [RES] resource %u 確保 (%s)\n", id_, owner_);
  }
  ~HeavyResource() {
    liveResources().erase(id_);
    std::printf("  [RES] resource %u 解放 (%s)\n", id_, owner_);
  }
  HeavyResource(const HeavyResource &) = delete;
  HeavyResource &operator=(const HeavyResource &) = delete;

private:
  unsigned int id_ = 0;
  const char *owner_;
};

inline void reportResourceLeaks() {
  if (liveResources().empty()) {
    std::printf("[RES] リークなし ✓\n");
  } else {
    for (auto id : liveResources()) {
      std::printf("[RES] !!! resource %u がリーク !!!\n", id);
    }
  }
}
