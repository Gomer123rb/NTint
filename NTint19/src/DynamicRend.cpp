#include "../include/DirectMod.h"
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <iostream>
#include <cmath>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

namespace DirectMod {

    // This file NO LONGER defines GraphicsManager::Initialize
    // That function belongs ONLY in GraphicsManager.cpp

    // Dynamic lighting helper
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

    // Dynamic composite vector struct
    struct Dynt {
        float range = 0.0f;

        DirectX::XMFLOAT3 xdi = { 1.0f, 0.0f, 0.0f };
        DirectX::XMFLOAT3 ydi = { 0.0f, 1.0f, 0.0f };
        DirectX::XMFLOAT3 zdi = { 0.0f, 0.0f, 1.0f };

        DirectX::XMFLOAT3 xz() const {
            return DirectX::XMFLOAT3(
                xdi.x + zdi.x,
                xdi.y + zdi.y,
                xdi.z + zdi.z - 2
            );
        }

        DirectX::XMFLOAT3 yz() const {
            return DirectX::XMFLOAT3(
                ydi.x + zdi.x,
                ydi.y + zdi.y,
                ydi.z + zdi.z - 2
            );
        }

        DirectX::XMFLOAT3 xy() const {
            return DirectX::XMFLOAT3(
                xdi.x + ydi.x,
                xdi.y + ydi.y,
                xdi.z + ydi.z - 2
            );
        }
    };

}
