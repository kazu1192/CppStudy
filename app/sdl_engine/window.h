#pragma once
#include <string>

struct SDL_Window; // 前方宣言
struct SDL_Renderer;

// SDL_Window と SDL_Renderer をまとめて所有する RAII クラス。
// コピー不可・ムーブ可で、所有権は常に一つだけ。
// 生成に失敗しても例外は投げないので、使う前に isValid() を確認すること。
class Window {
  public:
    Window(const std::string& title, int width, int height);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;

    bool isValid() const { return window_ != nullptr && renderer_ != nullptr; }

    SDL_Window* handle() const { return window_; }
    SDL_Renderer* renderer() const { return renderer_; }

  private:
    void release() noexcept; // 解放処理

    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
};
