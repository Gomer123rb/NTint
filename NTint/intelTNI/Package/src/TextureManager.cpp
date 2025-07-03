#include "../include/TextureManager.h"
#include <iostream.h>

namespace DirectMod {
    bool TextureManager::LoadTexture(const std::wstring& filename, ID3D11ShaderResourceView** textureView) {
        // Stub implementation.
        std::wcerr << L"[TextureManager] Loading texture: " << filename << L" (not implemented)" << std::endl;
        return false;
    }
}
