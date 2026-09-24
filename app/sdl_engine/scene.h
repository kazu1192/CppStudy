#pragma once
#include "entity.h"
#include <memory>
#include <vector>

class Scene {
  public:
    void add(std::unique_ptr<Entity> e);
    void updateAll(float dt);
    void renderAll(SDL_Renderer* renderer) const;
    std::size_t count() const { return entities_.size(); }

    std::vector<std::pair<Entity*, Entity*>> findCollisions() const;
    void clearCollisionFlags();

  private:
    std::vector<std::unique_ptr<Entity>> entities_;
};
