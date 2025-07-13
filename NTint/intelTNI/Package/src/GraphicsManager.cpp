#include "../include/GraphicsManager.h"
#include <DirectXMath.h>

namespace DirectMod {
    ID3D11Device* GraphicsManager::device = nullptr;
    ID3D11DeviceContext* GraphicsManager::context = nullptr;
    IDXGISwapChain* GraphicsManager::swapChain = nullptr;
    ID3D11RenderTargetView* GraphicsManager::renderTargetView = nullptr;

    bool GraphicsManager::Initialize(HWND hwnd) {
        DXGI_SWAP_CHAIN_DESC scd = {};
        scd.BufferCount = 2;  // Double buffering for smoother frames
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.BufferDesc.RefreshRate.Numerator = 60;
        scd.BufferDesc.RefreshRate.Denominator = 1;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.OutputWindow = hwnd;
        scd.SampleDesc.Count = 1;
        scd.SampleDesc.Quality = 0;
        scd.Windowed = TRUE;
        scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        if (FAILED(D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &scd,
            &swapChain,
            &device,
            nullptr,
            &context)))
        {
            return false;
        }

        ID3D11Texture2D* backBuffer = nullptr;
        if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer))) {
            return false;
        }

        if (FAILED(device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView))) {
            backBuffer->Release();
            return false;
        }
        backBuffer->Release();
        context->OMSetRenderTargets(1, &renderTargetView, nullptr);

        // Set up viewport based on buffer size
        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = static_cast<float>(scd.BufferDesc.Width ? scd.BufferDesc.Width : 800);
        viewport.Height = static_cast<float>(scd.BufferDesc.Height ? scd.BufferDesc.Height : 600);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        context->RSSetViewports(1, &viewport);

        return true;
    }

    DirectX::XMFLOAT4 GraphicsManager::GetSimulatedRayClearColor() {
        return DirectX::XMFLOAT4(0.13f, 0.16f, 0.20f, 1.0f);
    }

    void GraphicsManager::Clear(float r, float g, float b, float a, bool useNTINT) {
        float color[4];

        if (useNTINT) {
            DirectX::XMFLOAT4 simulatedRayColor = GetSimulatedRayClearColor();
            color[0] = simulatedRayColor.x;
            color[1] = simulatedRayColor.y;
            color[2] = simulatedRayColor.z;
            color[3] = simulatedRayColor.w;
        } else {
            color[0] = r;
            color[1] = g;
            color[2] = b;
            color[3] = a;
        }

        context->ClearRenderTargetView(renderTargetView, color);
    }

    void GraphicsManager::Present() {
        swapChain->Present(0, 0);  // Immediate frame display, minimal lag
    }

    void GraphicsManager::Shutdown() {
        if (renderTargetView) renderTargetView->Release();
        if (swapChain) swapChain->Release();
        if (context) context->Release();
        if (device) device->Release();
    }
}
