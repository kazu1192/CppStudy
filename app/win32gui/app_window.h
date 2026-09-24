#pragma once
#include <windows.h>

class AppWindow {
  public:
    AppWindow() = default;
    ~AppWindow();

    AppWindow(const AppWindow&) = delete;
    AppWindow& operator=(const AppWindow&) = delete;
    AppWindow(AppWindow&&) = delete;
    AppWindow& operator=(AppWindow&&) = delete;

    bool create();
    int run();

  private:
    static LRESULT CALLBACK wndProcStatic(HWND, UINT, WPARAM, LPARAM);
    LRESULT wndProc(HWND, UINT, WPARAM, LPARAM); // 実処理
    HWND hwnd_ = nullptr;
};
