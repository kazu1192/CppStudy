#pragma once
#include <string>

struct SDL_Window; // 前方宣言
struct SDL_Renderer;

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
