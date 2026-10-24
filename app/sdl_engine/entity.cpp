#include "entity.h"
#include "aabb.h"
#include "input.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

Box::Box(std::string name, Vec2 pos, Vec2 vel, float size, Color color, Vec2 bounds)
    : Entity(std::move(name), pos, vel), size_(size), color_(color), bounds_(bounds) {}

void Box::update(float dt) {
    position_ += velocity_ * dt;

    // 画面端に当たったら、はみ出し分を戻したうえで速度を反転して跳ね返す

    if (position_.x < 0.0f) {
        position_.x = 0.0f;
        velocity_.x = -velocity_.x;
    }
    if (position_.x + size_ > bounds_.x) {
        position_.x = bounds_.x - size_;
        velocity_.x = -velocity_.x;
    }

    if (position_.y < 0.0f) {
        position_.y = 0.0f;
        velocity_.y = -velocity_.y;
    }
    if (position_.y + size_ > bounds_.y) {
        position_.y = bounds_.y - size_;
        velocity_.y = -velocity_.y;
    }
}

void Box::render(SDL_Renderer* renderer) const {
    // 衝突中は白で光らせる
    Color c = isColliding() ? colors::white : color_;
    SDL_FRect rect{position_.x, position_.y, size_, size_};
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
    SDL_RenderFillRect(renderer, &rect);
}

AABB Box::bounds() const { return AABB{position_, Vec2{size_, size_}}; }

Player::Player(std::string name, Vec2 pos, float size, Color color, Vec2 bounds)
    : Entity(std::move(name), pos, Vec2{}), size_(size), color_(color), bounds_(bounds) {}

// 入力は速度として保持し、実際の移動は update() で dt を掛けて行う
void Player::handleInput(const Input& input) { velocity_ = input.moveAxis() * speed_; }

void Player::update(float dt) {
    position_ += velocity_ * dt;

    // 画面外に出ないよう位置だけ丸める（Box と違って速度は反転しない）

    if (position_.x < 0.0f)
        position_.x = 0.0f;
    if (position_.x + size_ > bounds_.x)
        position_.x = bounds_.x - size_;

    if (position_.y < 0.0f)
        position_.y = 0.0f;
    if (position_.y + size_ > bounds_.y)
        position_.y = bounds_.y - size_;
}

void Player::render(SDL_Renderer* renderer) const {
    SDL_FRect rect{position_.x, position_.y, size_, size_};
    SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);
    SDL_RenderFillRect(renderer, &rect);
}

AABB Player::bounds() const { return AABB{position_, Vec2{size_, size_}}; }
