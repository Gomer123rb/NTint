#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include <d3d11.h>

namespace DirectMod {
    class GraphicsManager {
    public:
        static bool Initialize(HWND hwnd);
        static void Clear(float r, float g, float b, float a);
        static void Present();
        static void Shutdown();
    private:
        static ID3D11Device* device;
        static ID3D11DeviceContext* context;
        static IDXGISwapChain* swapChain;
        static ID3D11RenderTargetView* renderTargetView;
    };
}

#endif // GRAPHICSMANAGER_H
