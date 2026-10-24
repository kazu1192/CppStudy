#include "window.h"
#include <SDL3/SDL.h>
#include <cstdio>
#include <utility>

Window::Window(const std::string& title, int width, int height) {
    std::printf("[Window] コンストラクタ: %s\n", title.c_str());

    window_ = SDL_CreateWindow(title.c_str(), width, height, 0);

    if (!window_) {
        std::printf("SDL_CreateWindow 失敗: %s\n", SDL_GetError());
        return;
    }

    // 自動選択（nullptr）ではなく、opengl を優先し使えなければ software に落とす指定にしている
    // renderer_ = SDL_CreateRenderer(window_, nullptr); // 自動選択
    renderer_ = SDL_CreateRenderer(window_, "opengl,software");

    if (!renderer_) {
        std::printf("SDL_CreateRenderer 失敗: %s\n", SDL_GetError());
        return;
    }

    SDL_SetRenderVSync(renderer_, 1); // 垂直同期でフレームレートをモニタに合わせる

    std::printf("[Window] 生成完了: window_=%p renderer_=%p\n", (void*)window_, (void*)renderer_);
    std::printf("[Window] レンダラ: %s\n", SDL_GetRendererName(renderer_));
}

// renderer は window に紐づくため、生成と逆順（renderer → window）で破棄する。
// 解放後は nullptr に戻すので、二重に呼んでも安全。
void Window::release() noexcept {
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
}

Window::~Window() {
    std::printf("[Window] デストラクタ: window_=%p\n", (void*)window_);
    release();
}

Window::Window(Window&& other) noexcept : window_(other.window_), renderer_(other.renderer_) {
    std::printf("[Window] ムーブコンストラクタ\n");
    other.window_ = nullptr;
    other.renderer_ = nullptr;
}

// 自分が持っていたリソースを先に解放してから、相手の所有権を奪う
Window& Window::operator=(Window&& other) noexcept {
    std::printf("[Window] ムーブ代入\n");
    if (this != &other) {
        release();
        window_ = std::exchange(other.window_, nullptr);
        renderer_ = std::exchange(other.renderer_, nullptr);
    }
    return *this;
}
