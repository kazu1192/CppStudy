#pragma once
#include <windows.h>

// Win32 のウィンドウ1枚を所有する RAII クラス。
// HWND はコールバックにしか渡されないので、this を GWLP_USERDATA に載せて橋渡ししている。
// 生成後はウィンドウが this を指し続けるため、ムーブ・コピーは禁止している。
class AppWindow {
  public:
    AppWindow() = default;
    ~AppWindow();

    AppWindow(const AppWindow&) = delete;
    AppWindow& operator=(const AppWindow&) = delete;
    AppWindow(AppWindow&&) = delete;
    AppWindow& operator=(AppWindow&&) = delete;

    // クラス登録とウィンドウ作成・表示。失敗したら false。
    bool create();
    // メッセージループ。ウィンドウが閉じられるまで戻らない。
    int run();

  private:
    // OS から呼ばれる静的関数。userdata から AppWindow を引き当てて wndProc に転送する。
    static LRESULT CALLBACK wndProcStatic(HWND, UINT, WPARAM, LPARAM);
    LRESULT wndProc(HWND, UINT, WPARAM, LPARAM); // 実処理
    HWND hwnd_ = nullptr;
};
