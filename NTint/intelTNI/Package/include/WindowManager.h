#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <windows.h>

namespace DirectMod {
    class WindowManager {
    public:
        static bool CreateAppWindow(int width, int height, const wchar_t* title);
        static HWND GetHWND();
        static void PollEvents();
    private:
        static HWND hwnd;
        static HINSTANCE hInstance;
    };
}

#endif // WINDOWMANAGER_H
