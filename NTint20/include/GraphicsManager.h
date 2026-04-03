#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include <d3d11.h>
#include <DirectXMath.h>  // For DirectX::XMFLOAT4
#include <Windows.h>

namespace DirectMod {

    class GraphicsManager {
    public:
        static bool Initialize(HWND hwnd);
        static void Clear(float r, float g, float b, float a, bool useNTINTAmbient = false);
        static void Present();
        static void Shutdown();

        // Getter so ShaderManager can access the device safely
        static ID3D11Device* GetDevice() { return device; }
        static ID3D11DeviceContext* GetContext() { return context; }

    private:
        // FIXED: Added DirectX:: namespace
        static DirectX::XMFLOAT4 GetSimulatedRayClearColor();

        static ID3D11Device* device;
        static ID3D11DeviceContext* context;
        static IDXGISwapChain* swapChain;
        static ID3D11RenderTargetView* renderTargetView;
        static ID3D11SamplerState* samplerState;
    };

}

#endif // GRAPHICSMANAGER_H
