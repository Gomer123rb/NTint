#include "../include/DirectMod.h"
#include <cmath>

namespace AmbientDM {

    // ------------------------------------------------------------
    // Initialize water context
    // ------------------------------------------------------------
    bool InitWater(HWND hwnd, int presetIndex, WaterContext& outCtx)
    {
        if (presetIndex < 0 || presetIndex >= NT_WATER_PRESET_COUNT)
            return false;

        // Load preset
        outCtx.env = NT_WATER_PRESETS[presetIndex];

        // Initial light direction (placeholder)
        outCtx.lightDir = PXIVec3(0.3f, 0.8f, -0.4f, true);

        // Initial view direction (camera forward)
        outCtx.viewDir = PXIVec3(0.0f, 0.0f, -1.0f, true);

        // Initial flat normal
        outCtx.waveNormal = PXIVec3(0.0f, 1.0f, 0.0f, true);

        return true;
    }

    bool InitAmbient(HWND hwnd, int presetIndex, AmbientContext& outCtx)
    {
        if (presetIndex < 0 || presetIndex >= NT_EOU_PRESET_COUNT)
            return false;

        // Load preset
        outCtx.env = NT_EOU_PRESETS[presetIndex];

        // Initial light direction (placeholder)
        outCtx.lightDir = PXIVec3(0.5f, 0.7f, -0.2f, true);

        // Initial view direction (camera forward)
        outCtx.viewDir = PXIVec3(0.0f, 0.0f, -1.0f, true);

        return true;
    }

    // ------------------------------------------------------------
    // Compute reflection vector
    // ------------------------------------------------------------
    PXIVec3 ComputeReflection(const WaterContext& ctx)
    {
        return reflectLight(ctx.lightDir, ctx.waveNormal);
    }

    // ------------------------------------------------------------
    // Compute refraction vector
    // ------------------------------------------------------------
    PXIVec3 ComputeRefraction(const WaterContext& ctx)
    {
        // Water IOR = 1.33
        return refractLight(ctx.lightDir, ctx.waveNormal, 1.0f / 1.33f);
    }

    // ------------------------------------------------------------
    // Wave height function
    // ------------------------------------------------------------
    float Waveywater(float x, float y, float z, float time)
    {
        return sinf(x * 0.1f + time) * cosf(y * 0.1f + time) * 0.5f;
    }

    // ------------------------------------------------------------
    // Final water lighting (Fresnel blend)
    // ------------------------------------------------------------
    PXIVec3 ComputeWaterLighting(const WaterContext& ctx, float eta)
    {
        PXIVec3 R = ComputeReflection(ctx);
        PXIVec3 T = ComputeRefraction(ctx);

        float viewDotNormal =
            ctx.viewDir.xl * ctx.waveNormal.xl +
            ctx.viewDir.yl * ctx.waveNormal.yl +
            ctx.viewDir.zl * ctx.waveNormal.zl;

        float fresnel = powf(1.0f - viewDotNormal, 5.0f);

        return R.mulLighting(fresnel)
                .addLighting(T.mulLighting(1.0f - fresnel));
    }

} // namespace AmbientDM
