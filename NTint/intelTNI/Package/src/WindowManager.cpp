#include "../include/WindowManager.h"

namespace DirectMod {
    HWND WindowManager::hwnd = nullptr;
    HINSTANCE WindowManager::hInstance = GetModuleHandle(nullptr);

    LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (msg == WM_DESTROY) {
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    bool WindowManager::CreateAppWindow(int width, int height, const wchar_t* title) {
        WNDCLASS wc = {};
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = L"DirectModWindowClass";

        RegisterClass(&wc);

        hwnd = CreateWindowEx(
            0,
            wc.lpszClassName,
            title,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            width, height,
            nullptr,
            nullptr,
            hInstance,
            nullptr
        );

        if (!hwnd)
            return false;

        ShowWindow(hwnd, SW_SHOW);
        return true;
    }

    HWND WindowManager::GetHWND() {
        return hwnd;
    }

    void WindowManager::PollEvents() {
        MSG msg = {};
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}