#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <windows.h>

namespace DirectMod {
    class InputManager {
    public:
        static void Initialize(HWND hwnd);
        static void Update();
        static bool IsKeyPressed(int key);
        static void GetMousePosition(int& x, int& y);
    private:
        static HWND hwnd;
        static bool keyStates[256];
        static int mouseX, mouseY;
    };
}

#endif // INPUTMANAGER_H

