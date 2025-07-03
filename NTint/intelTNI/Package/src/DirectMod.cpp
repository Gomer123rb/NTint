#include "../include/DirectMod.h"

namespace DirectMod {
    bool Initialize(int width, int height, const wchar_t* title) {
        if (!WindowManager::CreateAppWindow(width, height, title))
            return false;
        if (!GraphicsManager::Initialize(WindowManager::GetHWND()))
            return false;
        InputManager::Initialize(WindowManager::GetHWND());
        // Additional subsystem initialization can be added here.
        return true;
    }

    void Run() {
        MSG msg = {0};
        while (msg.message != WM_QUIT) {
            WindowManager::PollEvents();
            InputManager::Update();
            // For now, physics update is stubbed with a fixed delta time.
            PhysicsManager::Update(0.016f);
            GraphicsManager::Clear(0.0f, 0.2f, 0.4f, 1.0f);
            GraphicsManager::Present();
        }
    }

    void Shutdown() {
        GraphicsManager::Shutdown();
        // Additional shutdown for other systems if necessary.
    }
}
