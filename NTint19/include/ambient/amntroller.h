#ifndef AMBIENTDM_WATER_H
#define AMBIENTDM_WATER_H

#include "ambient.h"
#include "ambientwater.h"
#include "ambientdarkroom.h"
#include "PXIVec3.h"          // instead of DirectMod.h
#include <windows.h>

namespace AmbientDM {

    struct WaterContext {
        PXIVec3 lightDir;
        PXIVec3 viewDir;
        PXIVec3 waveNormal;
        const NTintEnvironment* env;
    };

    struct AmbientContext {
        PXIVec3 lightDir;
        PXIVec3 viewDir;
        const NTintEnvironment* env;
    };

    bool InitWater(HWND hwnd, int presetIndex, WaterContext& outCtx);
    bool InitAmbient(HWND hwnd, int presetIndex, AmbientContext& outCtx);

    PXIVec3 ComputeRefraction(const WaterContext& ctx);
    PXIVec3 ComputeReflection(const WaterContext& ctx);
    PXIVec3 ComputeWaterLighting(const WaterContext& ctx, float eta);

    float Waveywater(float x, float y, float time);   // or 4 args, but match .cpp

    inline PXIVec3 MakeWaveNormal(float x, float y, float time) {
        float hL = Waveywater(x - 1.0f, y, time - 0.5f);
        float hR = Waveywater(x + 1.0f, y, time - 0.5f);
        float hD = Waveywater(x, y - 1.0f, time - 0.5f);
        float hU = Waveywater(x, y + 1.0f, time - 0.5f);

        float nx = hL - hR;
        float ny = 2.0f;
        float nz = hD - hU;

        float len = sqrtf(nx*nx + ny*ny + nz*nz);
        return PXIVec3(nx / len, ny / len, nz / len, true);
    }

} // namespace AmbientDM

#endif
