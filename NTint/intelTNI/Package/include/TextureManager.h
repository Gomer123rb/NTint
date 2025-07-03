#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <d3d11.h>
#include <string.h>

namespace DirectMod {
    class TextureManager {
    public:
        // Stub: Load a texture from file.
        static bool LoadTexture(const std::wstring& filename, ID3D11ShaderResourceView** textureView);
    };
}

#endif // TEXTUREMANAGER_H
