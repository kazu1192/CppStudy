#pragma once
#include <windows.h>

class AppWindow {
  public:
    bool create();
    int run();

  private:
    static LRESULT CALLBACK wndProcStatic(HWND, UINT, WPARAM, LPARAM);
    LRESULT wndProc(HWND, UINT, WPARAM, LPARAM); // 実処理
    HWND hwnd_ = nullptr;
};
