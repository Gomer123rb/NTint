#ifndef DIRECTMOD_H
#define DIRECTMOD_H

#include <windows.h>
#include <vector>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <DirectXMath.h>

#define OPC_MOV  0x01
#define OPC_ADD  0x02
#define OPC_MUL  0x03
#define OPC_DP3  0x04
#define OPC_FRC  0x05
#define OPC_SLT  0x06
#define OPC_SGE  0x07
#define OPC_MAD  0x08
#define OPC_DP4  0x09
#define OPC_RCP  0x0A
#define OPC_RSQ  0x0B
#define OPC_MIN  0x0C
#define OPC_MAX  0x0D
#define OPC_TEX  0x0E
#define OPC_TEXB 0x0F
#define OPC_DEF  0x10
#define OPC_LIT  0x11
#include "PXIVec3.h"
#include "WindowManager.h"
#include "GraphicsManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "ambient/amntroller.h"
#include "ntccpu.h"
#include "ntchit.h"
#include "PXICollider.h"
#include "PXIPhysicsWorld.h"
#include "PXIRigidBody.h"
#include "glop.hpp"

namespace DirectMod {

    struct Vec4 {
        float x, y, z, w;
    };

    inline Vec4 make_vec(float x = 0, float y = 0, float z = 0, float w = 0) {
        return {x, y, z, w};
    }

    class ShaderVM {
    public:
        ShaderVM() {
            regs.resize(128, make_vec());
        }

        void load(const uint8_t* data, size_t size) {
            code.assign(data, data + size);
        }

        void run() {
                    size_t pc = 0;
        while (pc < code.size()) {
            uint8_t op = code[pc];

            // DEF: [OPC_DEF][id][x][y][z][w]
            if (op == OPC_DEF) {
                if (pc + 18 > code.size()) break;
                uint8_t id = code[pc+1];
                float x,y,z,w;
                std::memcpy(&x, &code[pc+2], 4);
                std::memcpy(&y, &code[pc+6], 4);
                std::memcpy(&z, &code[pc+10],4);
                std::memcpy(&w, &code[pc+14],4);
                regs[id & 0x7F] = make_vec(x,y,z,w);
                pc += 18;
                continue;
            }

            // MAD: [OPC_MAD][dst][s1][s2][s3]
            if (op == OPC_MAD) {
                if (pc + 5 > code.size()) break;
                uint8_t d = code[pc+1];
                uint8_t a = code[pc+2];
                uint8_t b = code[pc+3];
                uint8_t c = code[pc+4];

                Vec4 A = getSrc(a);
                Vec4 B = getSrc(b);
                Vec4 C = getSrc(c);
                Vec4& D = getDst(d);

                D.x = A.x*B.x + C.x;
                D.y = A.y*B.y + C.y;
                D.z = A.z*B.z + C.z;
                D.w = A.w*B.w + C.w;

                pc += 5;
                continue;
            }

            // All other ops are 4 bytes: [op][dst][s1][s2/packed]
            if (pc + 4 > code.size()) break;
            uint8_t d = code[pc+1];
            uint8_t s1 = code[pc+2];
            uint8_t s2 = code[pc+3];

            Vec4& D = getDst(d);
            Vec4 A = getSrc(s1);
            Vec4 B = getSrc(s2); // may be unused for unary ops

            switch (op) {
                case OPC_MOV:
                    execMov(D, A, s2); // s2 = packed dst/src mask
                    break;

                case OPC_ADD:
                    D = make_vec(A.x+B.x, A.y+B.y, A.z+B.z, A.w+B.w);
                    break;

                case OPC_MUL:
                    D = make_vec(A.x*B.x, A.y*B.y, A.z*B.z, A.w*B.w);
                    break;

                case OPC_DP3: {
                    float v = A.x*B.x + A.y*B.y + A.z*B.z;
                    D = make_vec(v,v,v,v);
                } break;

                case OPC_DP4: {
                    float v = A.x*B.x + A.y*B.y + A.z*B.z + A.w*B.w;
                    D = make_vec(v,v,v,v);
                } break;

                case OPC_FRC: {
                    uint8_t swiz = s2;
                    Vec4 swizzled = applySwizzle(A, swiz);
                    D = make_vec(
                        swizzled.x - std::floor(swizzled.x),
                        swizzled.y - std::floor(swizzled.y),
                        swizzled.z - std::floor(swizzled.z),
                        swizzled.w - std::floor(swizzled.w)
                    );
                } break;

                case OPC_SLT:
                    D = make_vec(
                        A.x < B.x ? 1.0f : 0.0f,
                        A.y < B.y ? 1.0f : 0.0f,
                        A.z < B.z ? 1.0f : 0.0f,
                        A.w < B.w ? 1.0f : 0.0f
                    );
                    break;

                case OPC_SGE:
                    D = make_vec(
                        A.x >= B.x ? 1.0f : 0.0f,
                        A.y >= B.y ? 1.0f : 0.0f,
                        A.z >= B.z ? 1.0f : 0.0f,
                        A.w >= B.w ? 1.0f : 0.0f
                    );
                    break;

                case OPC_RCP: {
                    uint8_t swiz = s2;
                    Vec4 swizzled = applySwizzle(A, swiz);
                    D = make_vec(
                        swizzled.x ? 1.0f/swizzled.x : 0.0f,
                        swizzled.y ? 1.0f/swizzled.y : 0.0f,
                        swizzled.z ? 1.0f/swizzled.z : 0.0f,
                        swizzled.w ? 1.0f/swizzled.w : 0.0f
                    );
                } break;

                case OPC_RSQ: {
                    uint8_t swiz = s2;
                    Vec4 swizzled = applySwizzle(A, swiz);
                    D = make_vec(
                        swizzled.x ? 1.0f/std::sqrt(std::fabs(swizzled.x)) : 0.0f,
                        swizzled.y ? 1.0f/std::sqrt(std::fabs(swizzled.y)) : 0.0f,
                        swizzled.z ? 1.0f/std::sqrt(std::fabs(swizzled.z)) : 0.0f,
                        swizzled.w ? 1.0f/std::sqrt(std::fabs(swizzled.w)) : 0.0f
                    );
                } break;

                case OPC_LIT: {
                    // lit computes lighting coefficients with full swizzle support
                    // inst[3] contains swizzle byte (2 bits per component)
                    uint8_t swiz = s2;  // s2 is actually the swizzle byte now
                    Vec4 swizzled = applySwizzle(A, swiz);
                    
                    // Input: swizzled = (ambient, diffuse, specular_power, range)
                    // Output: D = (ambient, max(0, diffuse), specular, 1.0)
                    float ambient = 1.0f;
                    float diffuse = std::fmax(0.0f, swizzled.y);
                    float specular = (swizzled.y > 0.0f && swizzled.z > 0.0f) ? 
                        std::pow(std::fmax(0.0f, swizzled.z), swizzled.w * 128.0f) : 0.0f;
                    D = make_vec(ambient, diffuse, specular, 1.0f);
                } break;

                case OPC_MIN:
                    D = make_vec(
                        std::fmin(A.x,B.x),
                        std::fmin(A.y,B.y),
                        std::fmin(A.z,B.z),
                        std::fmin(A.w,B.w)
                    );
                    break;

                case OPC_MAX:
                    D = make_vec(
                        std::fmax(A.x,B.x),
                        std::fmax(A.y,B.y),
                        std::fmax(A.z,B.z),
                        std::fmax(A.w,B.w)
                    );
                    break;

                case OPC_TEX:
                case OPC_TEXB:
                    D = sampleTexture(A, s2); // A = coord, s2 = sampler id
                    break;

                default:
                    // unknown opcode: stop
                    return;
            }

            pc += 4;
         }
        }

    public:
        std::vector<Vec4> regs;
        std::vector<uint8_t> code;

        Vec4 getSrc(uint8_t id) {
            bool neg = (id & 0x80) != 0;
            uint8_t idx = id & 0x7F;
            if (idx >= regs.size()) regs.resize(idx+1, make_vec());
            Vec4 v = regs[idx];
            if (neg) {
                v.x = -v.x; v.y = -v.y; v.z = -v.z; v.w = -v.w;
            }
            return v;
        }

        Vec4& getDst(uint8_t id) {
            uint8_t idx = id & 0x7F;
            if (idx >= regs.size()) regs.resize(idx+1, make_vec());
            return regs[idx];
        }

        void execMov(Vec4& dst, const Vec4& src, uint8_t packed) {
            uint8_t dstMask = (packed >> 4) & 0x0F;
            uint8_t srcMask = packed & 0x0F;

            auto apply = [&](int c, float s, uint8_t sm) {
                if (dstMask & (1 << c)) {
                    if (sm & (1 << c)) {
                        switch (c) {
                            case 0: dst.x = s; break;
                            case 1: dst.y = s; break;
                            case 2: dst.z = s; break;
                            case 3: dst.w = s; break;
                        }
                    }
                }
            };

            apply(0, src.x, srcMask);
            apply(1, src.y, srcMask);
            apply(2, src.z, srcMask);
            apply(3, src.w, srcMask);
        }

        Vec4 sampleTexture(const Vec4& uv, uint8_t sampler) {
            // Stub
            return make_vec(uv.x, uv.y, 0.0f, 1.0f);
        }

        // Apply swizzle to a Vec4 (2 bits per component encoding)
        Vec4 applySwizzle(const Vec4& v, uint8_t swiz) {
            const float comps[4] = {v.x, v.y, v.z, v.w};
            Vec4 result;
            result.x = comps[(swiz >> 0) & 0x3];
            result.y = comps[(swiz >> 2) & 0x3];
            result.z = comps[(swiz >> 4) & 0x3];
            result.w = comps[(swiz >> 6) & 0x3];
            return result;
        }
    };
    bool InitGraphics(HWND hwnd);
    void ShutdownGraphics();
    bool InitWindow(int width, int height, const wchar_t* title);
    void ShutdownWindow();

    void BeginFrame();
    void EndFrame();

    void DrawCube(float x, float y, float z, float size);

    HWND GetHWND();
    void PollEvents();

    bool CompileVShader(const std::wstring& filename,
                        const std::string& entryPoint,
                        ID3D11VertexShader** vertexShader,
                        ID3DBlob** shaderBlob);

    bool CompilePShader(const std::wstring& filename,
                        const std::string& entryPoint,
                        ID3D11PixelShader** pixelShader);

    bool LoadTexture(const std::wstring& filename, ID3D11ShaderResourceView** textureView);

    void PResolveCollision(PhysicsObject& a, PhysicsObject& b);
    bool PCheckCollision(const AABB& a, const AABB& b);
    void PUpdate(float deltaTime);

    void IntCgx();
    void CgxV(float shadername, float sizeofshader);
    void CgxR();

    void DGetLightToDynamic(float nearbyviewoflight, float* x, float* y, float* z);
    void DGetDynamicUV(float time, float* u, float* v);
    void DWriteDynamictoColor(float time, float* r, float* g, float* b);

    // Forward declarations for DynamicRend functions
    DirectX::XMFLOAT3 GetLightToDynamic(float nearbyviewoflight);
    DirectX::XMFLOAT2 GetDynamicUV(float time);
    void WriteDynamictoColor(float time, DirectX::XMFLOAT3& outColor);

    void DrawMesh(int meshID, int materialID);
    void SetWorldMatrix(const float* m);
}

#endif // DIRECTMOD_H
