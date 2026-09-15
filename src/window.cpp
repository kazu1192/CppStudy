#include "window.h"
#include <SDL3/SDL.h>
#include <cstdio>
#include <utility>

Window::Window(const std::string& title, int width, int height) {
    std::printf("[Window] コンストラクタ: %s\n", title.c_str());

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::printf("SDL_Init 失敗: %s\n", SDL_GetError());
        return;
    }

    window_ = SDL_CreateWindow(title.c_str(), width, height, 0);

    if (!window_) {
        std::printf("SDL_CreateWindow 失敗: %s\n", SDL_GetError());
        return;
    }

    // renderer_ = SDL_CreateRenderer(window_, nullptr); // 自動選択
    renderer_ = SDL_CreateRenderer(window_, "opengl,software");

    if (!renderer_) {
        std::printf("SDL_CreateRenderer 失敗: %s\n", SDL_GetError());
        return;
    }

    SDL_SetRenderVSync(renderer_, 1);

    std::printf("[Window] 生成完了: window_=%p renderer_=%p\n", (void*)window_, (void*)renderer_);
    std::printf("[Window] レンダラ: %s\n", SDL_GetRendererName(renderer_));
}

void Window::release() noexcept {
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }

    SDL_Quit();
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

Window& Window::operator=(Window&& other) noexcept {
    std::printf("[Window] ムーブ代入\n");
    if (this != &other) {
        release();
        window_ = std::move(other.window_);
        renderer_ = std::move(other.renderer_);
    }
    return *this;
}
