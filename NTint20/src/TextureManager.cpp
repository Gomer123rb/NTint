#include "../include/DirectMod.h"
#include <wrl/client.h>
#include <wincodec.h>
#include <vector>
#include <iostream>
#include <Windows.h>

using Microsoft::WRL::ComPtr;

namespace DirectMod {

    bool TextureManager::LoadTexture(const std::wstring& filename,
                                     ID3D11ShaderResourceView** textureView)
    {
        std::wcerr << L"[TextureManager] Loading texture: " << filename << std::endl;

        *textureView = nullptr;

        // Create WIC factory
        ComPtr<IWICImagingFactory> wicFactory;
        HRESULT hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&wicFactory)
        );

        if (FAILED(hr)) {
            std::wcerr << L"[TextureManager] Failed to create WIC factory\n";
            return false;
        }

        // Decode the image
        ComPtr<IWICBitmapDecoder> decoder;
        hr = wicFactory->CreateDecoderFromFilename(
            filename.c_str(),
            nullptr,
            GENERIC_READ,
            WICDecodeMetadataCacheOnLoad,
            &decoder
        );

        if (FAILED(hr)) {
            std::wcerr << L"[TextureManager] Failed to decode image\n";
            return false;
        }

        // Get the first frame
        ComPtr<IWICBitmapFrameDecode> frame;
        decoder->GetFrame(0, &frame);

        // Convert to 32‑bit RGBA
        ComPtr<IWICFormatConverter> converter;
        wicFactory->CreateFormatConverter(&converter);

        hr = converter->Initialize(
            frame.Get(),
            GUID_WICPixelFormat32bppRGBA,
            WICBitmapDitherTypeNone,
            nullptr,
            0.0,
            WICBitmapPaletteTypeCustom
        );

        if (FAILED(hr)) {
            std::wcerr << L"[TextureManager] Failed to convert image to RGBA\n";
            return false;
        }

        // Get image size
        UINT width, height;
        converter->GetSize(&width, &height);

        // Allocate pixel buffer
        std::vector<BYTE> pixels(width * height * 4);
        hr = converter->CopyPixels(
            nullptr,
            width * 4,
            pixels.size(),
            pixels.data()
        );

        if (FAILED(hr)) {
            std::wcerr << L"[TextureManager] Failed to copy pixels\n";
            return false;
        }

        // Create DX11 texture
        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = pixels.data();
        initData.SysMemPitch = width * 4;

        ComPtr<ID3D11Texture2D> texture;
        hr = GraphicsManager::GetDevice()->CreateTexture2D(&desc, &initData, &texture);

        if (FAILED(hr)) {
            std::wcerr << L"[TextureManager] Failed to create texture2D\n";
            return false;
        }

        // Create SRV
        hr = GraphicsManager::GetDevice()->CreateShaderResourceView(texture.Get(), nullptr, textureView);

        if (FAILED(hr)) {
            std::wcerr << L"[TextureManager] Failed to create SRV\n";
            return false;
        }

        return true;
    }

}
