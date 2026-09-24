#include "input.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <algorithm>
#include <cstring>

void Input::update() {
    // 前のフレームの状態を保存してから、今の状態を取得
    if (current_) {
        const int n = std::min(numKeys_, kMaxKeys);
        std::memcpy(previous_, current_, static_cast<std::size_t>(n));
    }
    current_ = SDL_GetKeyboardState(&numKeys_);
}

bool Input::isDown(int scancode) const {
    if (!current_ || scancode < 0 || scancode >= numKeys_)
        return false;
    return current_[scancode];
}

bool Input::wasPressed(int scancode) const {
    if (!current_ || scancode < 0 || scancode >= numKeys_ || scancode >= kMaxKeys)
        return false;
    return current_[scancode] && !previous_[scancode];
}

Vec2 Input::moveAxis() const {
    Vec2 dir{};
    if (isDown(SDL_SCANCODE_LEFT) || isDown(SDL_SCANCODE_A))
        dir.x -= 1.0f;
    if (isDown(SDL_SCANCODE_RIGHT) || isDown(SDL_SCANCODE_D))
        dir.x += 1.0f;
    if (isDown(SDL_SCANCODE_UP) || isDown(SDL_SCANCODE_W))
        dir.y -= 1.0f;
    if (isDown(SDL_SCANCODE_DOWN) || isDown(SDL_SCANCODE_S))
        dir.y += 1.0f;

    return dir.normalized();
}
