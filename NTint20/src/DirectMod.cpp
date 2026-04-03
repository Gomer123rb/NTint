#include "../include/DirectMod.h"
#include <d3dcompiler.h>
#include <iostream>

namespace DirectMod {

ShaderVM vm;

static float worldMatrix[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

bool InitWindow(int width, int height, const wchar_t* title) {
    return WindowManager::CreateAppWindow(width, height, title);
}

bool InitGraphics(HWND hwnd) {
    if (!GraphicsManager::Initialize(hwnd))
        return false;

    InputManager::Initialize(hwnd);
    return true;
}

void BeginFrame() {
    GraphicsManager::Clear(0.0f, 0.2f, 0.4f, 1.0f);
}

void EndFrame() {
    GraphicsManager::Present();
}

void DrawCube(float x, float y, float z, float size) {
    // Basic cube vertices (position only for now)
    struct Vertex {
        float x, y, z;
    };

    Vertex vertices[] = {
        // Front face
        {-size + x, -size + y, -size + z},
        { size + x, -size + y, -size + z},
        { size + x,  size + y, -size + z},
        {-size + x,  size + y, -size + z},
        // Back face
        {-size + x, -size + y,  size + z},
        { size + x, -size + y,  size + z},
        { size + x,  size + y,  size + z},
        {-size + x,  size + y,  size + z}
    };

    UINT indices[] = {
        // Front
        0,1,2, 0,2,3,
        // Right
        1,5,6, 1,6,2,
        // Back
        5,4,7, 5,7,6,
        // Left
        4,0,3, 4,3,7,
        // Top
        3,2,6, 3,6,7,
        // Bottom
        4,5,1, 4,1,0
    };

    // Create vertex buffer
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.ByteWidth = sizeof(vertices);
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = vertices;

    ID3D11Buffer* vertexBuffer = nullptr;
    GraphicsManager::GetDevice()->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);

    // Create index buffer
    D3D11_BUFFER_DESC ibDesc = {};
    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    ibDesc.ByteWidth = sizeof(indices);
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA ibData = {};
    ibData.pSysMem = indices;

    ID3D11Buffer* indexBuffer = nullptr;
    GraphicsManager::GetDevice()->CreateBuffer(&ibDesc, &ibData, &indexBuffer);

    // Set vertex buffer
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    GraphicsManager::GetContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    GraphicsManager::GetContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    GraphicsManager::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    GraphicsManager::GetContext()->DrawIndexed(36, 0, 0);

    // Release buffers
    if (vertexBuffer) vertexBuffer->Release();
    if (indexBuffer) indexBuffer->Release();
}

void DrawMesh(int meshID, int materialID) {
    if (meshID == 0) {
        DrawCube(0.0f, 0.0f, 0.0f, 1.0f);
    }
    // TODO: Implement proper mesh loading and rendering
}

void SetWorldMatrix(const float* m) {
    std::memcpy(worldMatrix, m, sizeof(float) * 16);
}

void ShutdownGraphics() {
    GraphicsManager::Shutdown();
    InputManager::Shutdown();
}

void ShutdownWindow() {
    WindowManager::Shutdown();
}

HWND GetHWND() {
    return WindowManager::GetHWND();
}

void PollEvents() {
    WindowManager::PollEvents();
}

bool CompileVShader(const std::wstring& filename,
                    const std::string& entryPoint,
                    ID3D11VertexShader** vertexShader,
                    ID3DBlob** shaderBlob)
{
    return ShaderManager::CompileVertexShader(filename, entryPoint, vertexShader, shaderBlob);
}

bool CompilePShader(const std::wstring& filename,
                    const std::string& entryPoint,
                    ID3D11PixelShader** pixelShader)
{
    return ShaderManager::CompilePixelShader(filename, entryPoint, pixelShader);
}

bool LoadTexture(const std::wstring& filename, ID3D11ShaderResourceView** textureView) {
    return TextureManager::LoadTexture(filename, textureView);
}

void PResolveCollision(PhysicsObject& a, PhysicsObject& b) {
    return PhysicsManager::ResolveCollision(a, b);
}

bool PCheckCollision(const AABB& a, const AABB& b) {
    return PhysicsManager::CheckCollision(a, b);
}

void PUpdate(float deltaTime) {
    return PhysicsManager::Update(deltaTime);
}

void IntCgx() {
    vm.regs[0] = make_vec(1,2,3,1); // v0
}

void CgxV(float shadername, float sizeofshader){
    vm.load((const uint8_t*)&shadername, (size_t)sizeofshader);
}

void CgxR() {
    vm.run();
 }

 void DGetLightToDynamic(float nearbyviewoflight, float* x, float* y, float* z) {
    DirectX::XMFLOAT3 result = GetLightToDynamic(nearbyviewoflight);
    *x = result.x;
    *y = result.y;
    *z = result.z;
 }

 void DGetDynamicUV(float time, float* u, float* v) {
    DirectX::XMFLOAT2 result = GetDynamicUV(time);
    *u = result.x;
    *v = result.y;
 }

void DWriteDynamictoColor(float time, float* r, float* g, float* b) {
    DirectX::XMFLOAT3 result;
    WriteDynamictoColor(time, result);
    *r = result.x;
    *g = result.y;
    *b = result.z;
}

} // namespace DirectMod
