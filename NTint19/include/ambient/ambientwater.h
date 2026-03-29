#ifndef AMBIENTWATER_H
#define AMBIENTWATER_H
#include "../include/DirectMod.h"

typedef struct NTintEnvironment NTintEnvironment;

// -----------------------------
// Water ambient presets
// -----------------------------

static const NTintEnvironment NT_WATER_PRESET0 = {
    { 0.20f, 0.35f, 0.55f },
    0.45f,
    0.18f,
    0.12f
};

static const NTintEnvironment NT_WATER_PRESET1 = {
    { 0.32f, 0.48f, 0.62f },
    0.52f,
    0.22f,
    0.16f
};

static const NTintEnvironment NT_WATER_PRESET2 = {
    { 0.55f, 0.70f, 0.82f },
    0.68f,
    0.14f,
    0.10f
};

static const NTintEnvironment NT_WATER_COM = {
    { 0.36f, 0.51f, 0.66f },
    0.55f,
    0.20f,
    0.14f
};

static const NTintEnvironment* NT_WATER_PRESETS[] = {
    &NT_WATER_PRESET0,
    &NT_WATER_PRESET1,
    &NT_WATER_PRESET2,
    &NT_WATER_COM
};

static const int NT_WATER_PRESET_COUNT =
    sizeof(NT_WATER_PRESETS) / sizeof(NT_WATER_PRESETS[0]);

// -----------------------------
// Water lighting math utilities
// -----------------------------

inline PXIVec3 reflectLight(const PXIVec3& L, const PXIVec3& N) {
    float dotNL = N.xl * L.xl + N.yl * L.yl + N.zl * L.zl;

    return PXIVec3(
        2 * dotNL * N.xl - L.xl,
        2 * dotNL * N.yl - L.yl,
        2 * dotNL * N.zl - L.zl,
        true
    );
}

inline PXIVec3 refractLight(const PXIVec3& L, const PXIVec3& N, float eta) {
    float dotNL = N.xl * L.xl + N.yl * L.yl + N.zl * L.zl;
    float k = 1.0f - eta * eta * (1.0f - dotNL * dotNL);

    if (k < 0.0f) {
        return PXIVec3(0,0,0,true);
    }

    return PXIVec3(
        eta * L.xl - (eta * dotNL + sqrtf(k)) * N.xl,
        eta * L.yl - (eta * dotNL + sqrtf(k)) * N.yl,
        eta * L.zl - (eta * dotNL + sqrtf(k)) * N.zl,
        true
    );
}

#endif // AMBIENTWATER_H
