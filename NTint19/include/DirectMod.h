#ifndef DIRECTMOD_H
#define DIRECTMOD_H

#include <windows.h>
#include "PXIVec3.h"
#include "WindowManager.h"
#include "GraphicsManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "ambient/amntroller.h"
#include "ntccpu.h"
#include "ntchit.h"
#include "PXICollider.h"
#include "PXIPhysicsWorld.h"
#include "PXIRigidBody.h"
#include "glop.hpp"

namespace DirectMod {
    bool InitGraphics(HWND hwnd);
    void ShutdownGraphics();

    void BeginFrame();
    void EndFrame();

    void DrawMesh(int meshID, int materialID);
    void SetWorldMatrix(const float* m);
}

#endif // DIRECTMOD_H
