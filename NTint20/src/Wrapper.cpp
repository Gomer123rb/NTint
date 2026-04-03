#include "../include/DirectMod.h"
#include "../include/ambient/amntroller.h"   
#include "../include/ambient/ambientwater.h"

extern "C" {

__declspec(dllexport)
bool WInitWindow(int width, int height, const wchar_t* title)
{
    return DirectMod::InitWindow(width, height, title);
}

__declspec(dllexport)
bool WInitGraphics(HWND hwnd)
{
    return DirectMod::InitGraphics(hwnd);
}

__declspec(dllexport)
void WBeginFrame()
{
    DirectMod::BeginFrame();
}

__declspec(dllexport)
void WEndFrame()
{
    DirectMod::EndFrame();
}

__declspec(dllexport)
void WDrawCube(float x, float y, float z, float size)
{
    DirectMod::DrawCube(x, y, z, size);
}

__declspec(dllexport)
void WDrawMesh(int meshID, int materialID)
{
    DirectMod::DrawMesh(meshID, materialID);
}

__declspec(dllexport)
void WSetWorldMatrix(const float* m)
{
    DirectMod::SetWorldMatrix(m);
}

__declspec(dllexport)
void WShutdownGraphics()
{
    DirectMod::ShutdownGraphics();
}

__declspec(dllexport)
void WShutdownWindow()
{
    DirectMod::ShutdownWindow();
}

__declspec(dllexport)
bool WInitWater(HWND hwnd, int presetIndex, AmbientDM::WaterContext* outCtx)
{
    if (!outCtx) return false;
    return AmbientDM::InitWater(hwnd, presetIndex, *outCtx);
}

__declspec(dllexport)
bool WInitAmbient(HWND hwnd, int presetIndex, AmbientDM::AmbientContext* outCtx)
{
    if (!outCtx) return false;
    return AmbientDM::InitAmbient(hwnd, presetIndex, *outCtx);
}

__declspec(dllexport)
void WComputeReflection(const AmbientDM::WaterContext* ctx, float* x, float* y, float* z)
{
    PXIVec3 r = AmbientDM::ComputeReflection(*ctx);
    *x = r.xl;
    *y = r.yl;
    *z = r.zl;
}

__declspec(dllexport)
void WComputeRefraction(const AmbientDM::WaterContext* ctx, float* x, float* y, float* z)
{
    PXIVec3 r = AmbientDM::ComputeRefraction(*ctx);
    *x = r.xl;
    *y = r.yl;
    *z = r.zl;
}

__declspec(dllexport)
float WWaveyWater(float x, float y, float time)
{
    return AmbientDM::Waveywater(x, y, time);
}

__declspec(dllexport)
void WComputeWaterLighting(const AmbientDM::WaterContext* ctx, float* r, float* g, float* b)
{
    PXIVec3 c = AmbientDM::ComputeWaterLighting(*ctx, 1.33f);
    *r = c.xl;
    *g = c.yl;
    *b = c.zl;
}

__declspec(dllexport)
void WPXIRIapplyForce(PXIRigidBody* body, const PXIVec3* force)
{
    if (!body || !force) return;
    body->applyForce(*force);
}

__declspec(dllexport)
void WPXIRIupdate(PXIRigidBody* body, float dt)
{
    if (!body) return;
    body->update(dt);
}

__declspec(dllexport)
void WPXIPWaddBody(PXIPhysicsWorld* world, PXIRigidBody* body)
{
    if (!world || !body) return;
    world->addBody(body);
}

__declspec(dllexport)
void WPXIPWupdate(PXIPhysicsWorld* world, float dt)
{
    if (!world) return;
    world->step(dt);
}

__declspec(dllexport)
HWND WGetHWND() {
    return DirectMod::GetHWND();
}

__declspec(dllexport)
void WPollEvents() {
    DirectMod::PollEvents();
}

__declspec(dllexport)
bool WCompileVShader(const wchar_t* filename, const char* entryPoint, ID3D11VertexShader** vertexShader, ID3DBlob** shaderBlob)
{
    return DirectMod::CompileVShader(filename, entryPoint, vertexShader, shaderBlob);
}

__declspec(dllexport)
bool WCompilePShader(const wchar_t* filename, const char* entryPoint, ID3D11PixelShader** pixelShader)
{
    return DirectMod::CompilePShader(filename, entryPoint, pixelShader);
}

__declspec(dllexport)
bool WLoadTexture(const wchar_t* filename, ID3D11ShaderResourceView** textureView)
{
    return DirectMod::LoadTexture(filename, textureView);
}

__declspec(dllexport)
void WPResolveCollision(DirectMod::PhysicsObject& a, DirectMod::PhysicsObject& b) {
    DirectMod::PResolveCollision(a, b);
}

__declspec(dllexport)
bool WPCheckCollision(const DirectMod::AABB& a, const DirectMod::AABB& b) {
    return DirectMod::PCheckCollision(a, b);
}

__declspec(dllexport)
void WPUpdate(float deltaTime) {
    DirectMod::PUpdate(deltaTime);
}

__declspec(dllexport)
void WIntGgx() {
    DirectMod::IntCgx();
}

__declspec(dllexport)
void WCgxV(float shadername, float sizeofshader){
    DirectMod::CgxV(shadername, sizeofshader);
}

__declspec(dllexport)
void WCgxR() {
    DirectMod::CgxR();
}

__declspec(dllexport)
void WDGetLightToDynamic(float nearbyviewoflight, float* x, float* y, float* z) {
    DirectMod::DGetLightToDynamic(nearbyviewoflight, x, y, z);
}

__declspec(dllexport)
void WDGetDynamicUV(float time, float* u, float* v) {
    DirectMod::DGetDynamicUV(time, u, v);
}

__declspec(dllexport)
void WDWriteDynamictoColor(float time, float* r, float* g, float* b) {
    DirectMod::DWriteDynamictoColor(time, r, g, b);
}

} // extern "C"
