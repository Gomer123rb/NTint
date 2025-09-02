
#include "../include/GraphicsManager.h"
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <iostream>
#include <cmath>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

namespace DirectMod {

    ID3D11Device* GraphicsManager::device = nullptr;
    ID3D11DeviceContext* GraphicsManager::context = nullptr;
    IDXGISwapChain* GraphicsManager::swapChain = nullptr;
    ID3D11RenderTargetView* GraphicsManager::renderTargetView = nullptr;
    ID3D11SamplerState* GraphicsManager::samplerState = nullptr;

    bool GraphicsManager::Initialize(HWND hwnd) {
        if (!hwnd) {
            std::cerr << "Initialize: invalid HWND\n";
            return false;
        }

        DXGI_SWAP_CHAIN_DESC scd = {};
        scd.BufferCount = 2;
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
        scd.BufferDesc.Width = 0;  // let DXGI infer from window
        scd.BufferDesc.Height = 0;

        D3D_FEATURE_LEVEL featureLevel;
        HRESULT hr = D3D11CreateDeviceAndSwapChain(
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
            &featureLevel,
            &context
        );

        if (FAILED(hr) || !device || !context || !swapChain) {
            std::cerr << "D3D11CreateDeviceAndSwapChain failed (HRESULT = 0x" << std::hex << hr << ")\n";
            // Ensure partial objects are released
            if (swapChain) { swapChain->Release(); swapChain = nullptr; }
            if (device) { device->Release(); device = nullptr; }
            if (context) { context->Release(); context = nullptr; }
            return false;
        }

        // Create render target view from back buffer
        ID3D11Texture2D* backBuffer = nullptr;
        hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
        if (FAILED(hr) || !backBuffer) {
            std::cerr << "GetBuffer failed (HRESULT = 0x" << std::hex << hr << ")\n";
            goto fail_cleanup;
        }

        hr = device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
        backBuffer->Release();
        backBuffer = nullptr;
        if (FAILED(hr) || !renderTargetView) {
            std::cerr << "CreateRenderTargetView failed (HRESULT = 0x" << std::hex << hr << ")\n";
            goto fail_cleanup;
        }

        // Create a simple linear sampler state
        D3D11_SAMPLER_DESC sampDesc = {};
        sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        sampDesc.MinLOD = 0;
        sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

        hr = device->CreateSamplerState(&sampDesc, &samplerState);
        if (FAILED(hr) || !samplerState) {
            std::cerr << "CreateSamplerState failed (HRESULT = 0x" << std::hex << hr << ")\n";
            goto fail_cleanup;
        }

        // Set RTV as render target and set a viewport sized to the client area
        context->OMSetRenderTargets(1, &renderTargetView, nullptr);

        RECT rc;
        GetClientRect(hwnd, &rc);
        FLOAT width = static_cast<FLOAT>(rc.right - rc.left);
        FLOAT height = static_cast<FLOAT>(rc.bottom - rc.top);

        D3D11_VIEWPORT vp = {};
        vp.TopLeftX = 0.0f;
        vp.TopLeftY = 0.0f;
        vp.Width = (width > 0.0f) ? width : 800.0f;
        vp.Height = (height > 0.0f) ? height : 600.0f;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        context->RSSetViewports(1, &vp);

        return true;

    fail_cleanup:
        if (renderTargetView) { renderTargetView->Release(); renderTargetView = nullptr; }
        if (samplerState) { samplerState->Release(); samplerState = nullptr; }
        if (swapChain) { swapChain->Release(); swapChain = nullptr; }
        if (context) { context->Release(); context = nullptr; }
        if (device) { device->Release(); device = nullptr; }
        return false;
    }

    DirectX::XMFLOAT3 GetLightToDynamic(float nearbyviewoflight) {
        using namespace DirectX;
        float t = nearbyviewoflight;

        if (t == 0.0f) {
            return XMFLOAT3(0.0f, 0.0f, 0.0f);
        }

        float x = std::sinf(t) * 5.0f;
        float y = t;
        float z = std::cosf(t) * 3.0f;

        XMVECTOR v = XMVectorSet(x, y, z, 0.0f);
        v = XMVector3Normalize(v);
        XMFLOAT3 out;
        XMStoreFloat3(&out, v);
        return out;
    }

    struct Dynt {
        float range = 0.0f; // How far the light or influence reaches

        DirectX::XMFLOAT3 xdi = {1.0f, 0.0f, 0.0f};
        DirectX::XMFLOAT3 ydi = {0.0f, 1.0f, 0.0f};
        DirectX::XMFLOAT3 zdi = {0.0f, 0.0f, 1.0f};

        // Composite vectors
        DirectX::XMFLOAT3 xz() const {
            return DirectX::XMFLOAT3(xdi.x + zdi.x, xdi.y + zdi.y, xdi.z + zdi.z);
        }

        DirectX::XMFLOAT3 yz() const {
            return DirectX::XMFLOAT3(ydi.x + zdi.x, ydi.y + zdi.y, ydi.z + zdi.z);
        }

        DirectX::XMFLOAT3 xy() const {
            return DirectX::XMFLOAT3(xdi.x + ydi.x, xdi.y + ydi.y, xdi.z + ydi.z);
        }
    };
}
