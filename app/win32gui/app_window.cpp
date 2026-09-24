#include "app_window.h"
#include <cstdio>

AppWindow::~AppWindow() {
    std::printf("[AppWindow] デストラクタ: hwnd_=%p\n", (void*)hwnd_);
    if (hwnd_) {
        DestroyWindow(hwnd_);
    }
}

LRESULT CALLBACK AppWindow::wndProcStatic(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    AppWindow* self = nullptr;

    if (msg == WM_NCCREATE) {
        auto* cs = reinterpret_cast<CREATESTRUCT*>(lp);
        self = static_cast<AppWindow*>(cs->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
        if (self) {
            self->hwnd_ = hwnd;
        }
    } else {
        self = reinterpret_cast<AppWindow*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    }

    if (self) {
        return self->wndProc(hwnd, msg, wp, lp);
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

LRESULT AppWindow::wndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rc;
        GetClientRect(hwnd, &rc);
        DrawTextW(hdc, L"Hello", -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY: // 破棄の途中
        PostQuitMessage(0);
        return 0;
    case WM_NCDESTROY: // 最後のメッセージ
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, 0);
        hwnd_ = nullptr;
        break; // DefWindowProcに流す
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

bool AppWindow::create() {
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = wndProcStatic;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"MyWindowClass";

    static const ATOM atom = RegisterClassExW(&wc); // 関数内static
    if (!atom) {
        return false;
    }

    HWND hwnd =
        CreateWindowExW(0, L"MyWindowClass", L"Sample", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                        CW_USEDEFAULT, 800, 600, nullptr, nullptr, GetModuleHandleW(nullptr), this);
    if (!hwnd) {
        return false;
    }

    ShowWindow(hwnd, SW_SHOW);
    return true;
}

int AppWindow::run() {
    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}
