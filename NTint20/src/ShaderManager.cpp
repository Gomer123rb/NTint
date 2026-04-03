#include "../include/DirectMod.h"
#include <d3dcompiler.h>
#include <iostream>

namespace DirectMod {

    bool ShaderManager::CompileVertexShader(const std::wstring& filename,
        const std::string& entryPoint,
        ID3D11VertexShader** vertexShader,
        ID3DBlob** shaderBlob)
    {
        ID3DBlob* errorBlob = nullptr;

        HRESULT hr = D3DCompileFromFile(
            filename.c_str(),
            nullptr,
            nullptr,
            entryPoint.c_str(),
            "vs_5_0",
            0,
            0,
            shaderBlob,
            &errorBlob
        );

        if (FAILED(hr)) {
            if (errorBlob) {
                std::cerr << "Vertex shader error: "
                    << static_cast<char*>(errorBlob->GetBufferPointer()) << "\n";
                errorBlob->Release();
            }
            return false;
        }

        // FIX: use GetDevice() instead of accessing private member
        hr = GraphicsManager::GetDevice()->CreateVertexShader(
            (*shaderBlob)->GetBufferPointer(),
            (*shaderBlob)->GetBufferSize(),
            nullptr,
            vertexShader
        );

        if (FAILED(hr)) {
            std::cerr << "Failed to create vertex shader.\n";
            return false;
        }

        return true;
    }

    bool ShaderManager::CompilePixelShader(const std::wstring& filename,
        const std::string& entryPoint,
        ID3D11PixelShader** pixelShader)
    {
        ID3DBlob* shaderBlob = nullptr;
        ID3DBlob* errorBlob = nullptr;

        HRESULT hr = D3DCompileFromFile(
            filename.c_str(),
            nullptr,
            nullptr,
            entryPoint.c_str(),
            "ps_5_0",
            0,
            0,
            &shaderBlob,
            &errorBlob
        );

        if (FAILED(hr)) {
            if (errorBlob) {
                std::cerr << "Pixel shader error: "
                    << static_cast<char*>(errorBlob->GetBufferPointer()) << "\n";
                errorBlob->Release();
            }
            return false;
        }

        // FIX: use GetDevice()
        hr = GraphicsManager::GetDevice()->CreatePixelShader(
            shaderBlob->GetBufferPointer(),
            shaderBlob->GetBufferSize(),
            nullptr,
            pixelShader
        );

        if (FAILED(hr)) {
            std::cerr << "Failed to create pixel shader.\n";
            shaderBlob->Release();
            return false;
        }

        shaderBlob->Release();
        return true;
    }

}
