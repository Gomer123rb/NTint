#include "../include/DirectMod.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>

namespace DirectMod {
    ID3D11Device* GraphicsManager::device = nullptr;
    ID3D11DeviceContext* GraphicsManager::context = nullptr;
    IDXGISwapChain* GraphicsManager::swapChain = nullptr;
    ID3D11RenderTargetView* GraphicsManager::renderTargetView = nullptr;
    ID3D11SamplerState* GraphicsManager::samplerState = nullptr;

    bool GraphicsManager::Initialize(HWND hwnd) {
        DXGI_SWAP_CHAIN_DESC scd = {};
        scd.BufferCount = 2;
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.BufferDesc.RefreshRate.Numerator = 300;
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
            &context))) {
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

        // Viewport Setup
        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = static_cast<float>(scd.BufferDesc.Width ? scd.BufferDesc.Width : 800);
        viewport.Height = static_cast<float>(scd.BufferDesc.Height ? scd.BufferDesc.Height : 600);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        context->RSSetViewports(1, &viewport);

        // Anisotropic Sampler Setup
        D3D11_SAMPLER_DESC sampDesc = {};
        sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
        sampDesc.MaxAnisotropy = 8;
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        sampDesc.MinLOD = 0;
        sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

        if (FAILED(device->CreateSamplerState(&sampDesc, &samplerState))) {
            return false;
        }
        context->PSSetSamplers(0, 1, &samplerState);

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
            color[0] = r + 0.5f;
            color[1] = g + 0.5f;
            color[2] = b + 0.5f;
            color[3] = a + 0.5f;
        }

        context->ClearRenderTargetView(renderTargetView, color);
    }

    void GraphicsManager::Present() {
        swapChain->Present(0, 0); // Fast present, zero sync delay
    }

    void GraphicsManager::Shutdown() {
        if (samplerState) samplerState->Release();
        if (renderTargetView) renderTargetView->Release();
        if (swapChain) swapChain->Release();
        if (context) context->Release();
        if (device) device->Release();
    }
}
