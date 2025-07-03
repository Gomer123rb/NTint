#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <string.h>

namespace DirectMod {
    class ShaderManager {
    public:
        // Compiles a vertex shader and returns true if successful.
        static bool CompileVertexShader(const std::wstring& filename,
                                        const std::string& entryPoint,
                                        ID3D11VertexShader** vertexShader,
                                        ID3DBlob** shaderBlob);

        // Compiles a pixel shader and returns true if successful.
        static bool CompilePixelShader(const std::wstring& filename,
                                       const std::string& entryPoint,
                                       ID3D11PixelShader** pixelShader);
    };
}

#endif // SHADERMANAGER_H
