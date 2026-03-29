#include "../include/DirectMod.h"

namespace DirectMod {

    bool InitWindow(int width, int height, const wchar_t* title) {
        return WindowManager::CreateAppWindow(width, height, title);
    }

    bool InitGraphics(HWND hwnd) {
        if (!GraphicsManager::Initialize(hwnd))
            return false;

        InputManager::Initialize(hwnd);
        return true;
    }

    void BeginFrame() {
        GraphicsManager::Clear(0.0f, 0.2f, 0.4f, 1.0f);
    }

    void EndFrame() {
        GraphicsManager::Present();
    }

    void ShutdownGraphics() {
        GraphicsManager::Shutdown();
        InputManager::Shutdown();
    }

    void ShutdownWindow() {
        WindowManager::Shutdown();
    }
}
