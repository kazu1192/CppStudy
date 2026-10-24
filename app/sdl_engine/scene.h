#pragma once
#include "entity.h"
#include <memory>
#include <vector>

// Entity の集まりを所有し、更新・描画・衝突判定をまとめて行う。
class Scene {
  public:
    // 所有権ごと受け取る
    void add(std::unique_ptr<Entity> e);
    void updateAll(float dt);
    void renderAll(SDL_Renderer* renderer) const;
    std::size_t count() const { return entities_.size(); }

    // 重なっている Entity の組を返す。ポインタは Scene が所有しており、借りているだけ。
    std::vector<std::pair<Entity*, Entity*>> findCollisions() const;
    // 全 Entity の衝突フラグを下ろす。毎フレーム findCollisions の前に呼ぶ。
    void clearCollisionFlags();

  private:
    std::vector<std::unique_ptr<Entity>> entities_;
};
