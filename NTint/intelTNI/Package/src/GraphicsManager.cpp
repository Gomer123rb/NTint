#include "../include/GraphicsManager.h"

namespace DirectMod {
    ID3D11Device* GraphicsManager::device = nullptr;
    ID3D11DeviceContext* GraphicsManager::context = nullptr;
    IDXGISwapChain* GraphicsManager::swapChain = nullptr;
    ID3D11RenderTargetView* GraphicsManager::renderTargetView = nullptr;

    bool GraphicsManager::Initialize(HWND hwnd) {
        DXGI_SWAP_CHAIN_DESC scd = {};
        scd.BufferCount = 1;
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.OutputWindow = hwnd;
        scd.SampleDesc.Count = 1;
        scd.Windowed = TRUE;

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
        swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
        if (FAILED(device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView))) {
            backBuffer->Release();
            return false;
        }
        backBuffer->Release();

        context->OMSetRenderTargets(1, &renderTargetView, nullptr);
        return true;
    }

    void GraphicsManager::Clear(float r, float g, float b, float a) {
        float color[4] = { r, g, b, a };
        context->ClearRenderTargetView(renderTargetView, color);
    }

    void GraphicsManager::Present() {
        swapChain->Present(1, 0);
    }

    void GraphicsManager::Shutdown() {
        if (renderTargetView) renderTargetView->Release();
        if (swapChain) swapChain->Release();
        if (context) context->Release();
        if (device) device->Release();
    }
}
