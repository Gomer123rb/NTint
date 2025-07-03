#include "../include/InputManager.h"

namespace DirectMod {
    HWND InputManager::hwnd = nullptr;
    bool InputManager::keyStates[256] = { false };
    int InputManager::mouseX = 0;
    int InputManager::mouseY = 0;

    void InputManager::Initialize(HWND hwnd) {
        InputManager::hwnd = hwnd;
    }

    void InputManager::Update() {
        // Update keyboard state.
        for (int i = 0; i < 256; ++i) {
            keyStates[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
        }
        // Update mouse position.
        POINT pt;
        if (GetCursorPos(&pt)) {
            ScreenToClient(hwnd, &pt);
            mouseX = pt.x;
            mouseY = pt.y;
        }
    }

    bool InputManager::IsKeyPressed(int key) {
        return keyStates[key];
    }

    void InputManager::GetMousePosition(int& x, int& y) {
        x = mouseX;
        y = mouseY;
    }
}
