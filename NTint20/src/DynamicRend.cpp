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

    DirectX::XMFLOAT2 GetDynamicUV(float time) {
        float u = std::sinf(time * 0.5f) * 0.5f + 0.5f;
        float v = std::cosf(time * 0.3f) * 0.5f + 0.5f;
        return DirectX::XMFLOAT2(u, v);
    }
    
    void WriteDynamictoColor(float time, DirectX::XMFLOAT3& outColor) {
        float r = std::sinf(time) * 0.5f + 0.5f;
        float g = std::cosf(time * 0.7f) * 0.5f + 0.5f;
        float b = std::sinf(time * 1.3f) * 0.5f + 0.5f;
        outColor = DirectX::XMFLOAT3(r, g, b);
        //experimental shuit B)
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
