#ifndef DIRECTMOD_H
#define DIRECTMOD_H

#include <windows.h>
#include "WindowManager.h"
#include "GraphicsManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "PhysicsManager.h"

namespace DirectMod {
    bool Initialize(int width, int height, const wchar_t* title);
    void Run();
    void Shutdown();
}

#endif // DIRECTMOD_H
