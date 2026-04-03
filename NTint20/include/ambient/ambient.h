#ifndef NTINT_ENV_PROFILES_H
#define NTINT_ENV_PROFILES_H

typedef struct NTintEnvironment {
    float ambientColor[3];
    float ambientLuminance;
    float contrast;
    float saturation;
} NTintEnvironment;

static const int NT_EOU_PRESET_COUNT = 14;

static const NTintEnvironment NT_EOU_PRESET0 = {
    { 0.576120f, 0.622052f, 0.731100f },
    0.626656f,
    0.297556f,
    0.104805f
};

static const NTintEnvironment NT_EOU_PRESET1 = {
    { 0.393938f, 0.539541f, 0.643303f },
    0.519071f,
    0.247418f,
    0.181109f
};

static const NTintEnvironment NT_EOU_PRESET2 = {
    { 0.617741f, 0.624379f, 0.531201f },
    0.642136f,
    0.271477f,
    0.124392f
};

static const NTintEnvironment NT_EOU_PRESET3 = {
    { 0.609394f, 0.646520f, 0.695631f },
    0.658864f,
    0.153896f,
    0.064012f
};

static const NTintEnvironment NT_EOU_PRESET4 = {
    { 0.350122f, 0.439287f, 0.455378f },
    0.425022f,
    0.251210f,
    0.102370f
};

static const NTintEnvironment NT_EOU_PRESET5 = {
    { 0.309102f, 0.415851f, 0.163810f },
    0.374265f,
    0.240753f,
    0.235382f
};

static const NTintEnvironment NT_EOU_PRESET6 = {
    { 0.521138f, 0.582038f, 0.336679f },
    0.549850f,
    0.228161f,
    0.247525f
};

static const NTintEnvironment NT_EOU_PRESET7 = {
    { 0.396356f, 0.564211f, 0.782489f },
    0.555688f,
    0.152850f,
    0.284094f
};

static const NTintEnvironment NT_EOU_PRESET8 = {
    { 0.426403f, 0.480322f, 0.295628f },
    0.454028f,
    0.247601f,
    0.195734f
};

static const NTintEnvironment NT_EOU_PRESET9 = {
    { 0.589833f, 0.592690f, 0.564648f },
    0.610032f,
    0.221320f,
    0.108970f
};

static const NTintEnvironment NT_EOU_PRESET10 = {
    { 0.586738f, 0.815821f, 0.945982f },
    0.761904f,
    0.157674f,
    0.249378f
};

static const NTintEnvironment NT_EOU_PRESET11 = {
    { 0.525003f, 0.615271f, 0.656107f },
    0.600455f,
    0.210198f,
    0.137739f
};

static const NTintEnvironment NT_EOU_PRESET12 = {
    { 0.412466f, 0.549879f, 0.604074f },
    0.525003f,
    0.239795f,
    0.176152f
};

static const NTintEnvironment NT_EOU_COM = {
    { 0.485720f, 0.575989f, 0.569695f },
    0.561767f,
    0.224608f,
    0.170128f
};

static const NTintEnvironment* NT_EOU_PRESETS[] = {
    &NT_EOU_PRESET0,
    &NT_EOU_PRESET1,
    &NT_EOU_PRESET2,
    &NT_EOU_PRESET3,
    &NT_EOU_PRESET4,
    &NT_EOU_PRESET5,
    &NT_EOU_PRESET6,
    &NT_EOU_PRESET7,
    &NT_EOU_PRESET8,
    &NT_EOU_PRESET9,
    &NT_EOU_PRESET10,
    &NT_EOU_PRESET11,
    &NT_EOU_PRESET12,
    &NT_EOU_COM
};

#endif // NTINT_ENV_PROFILES_H
