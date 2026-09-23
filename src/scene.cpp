#include "scene.h"
#include "aabb.h"
#include <SDL3/SDL_render.h>
#include <cstdio>

void Scene::add(std::unique_ptr<Entity> e) {
    std::printf(" Scene: %s を追加\n", e->name().c_str());
    entities_.push_back(std::move(e));
}

void Scene::updateAll(float dt) {
    for (const auto& e : entities_) {
        e->update(dt);
    }
}

void Scene::renderAll(SDL_Renderer* renderer) const {
    for (const auto& e : entities_) {
        e->render(renderer);
    }
}

std::vector<std::pair<Entity*, Entity*>> Scene::findCollisions() const {
    std::vector<std::pair<Entity*, Entity*>> result;

    for (std::size_t i = 0; i < entities_.size(); ++i) {
        for (std::size_t j = i + 1; j < entities_.size(); ++j) {
            if (intersects(entities_[i]->bounds(), entities_[j]->bounds())) {
                result.emplace_back(entities_[i].get(), entities_[j].get());
            }
        }
    }

    return result;
}

void Scene::clearCollisionFlags() {
    for (const auto& e : entities_) {
        e->setColliding(false);
    }
}
