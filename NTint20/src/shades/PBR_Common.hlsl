// =============================================================
// PBR_Common.hlsl
// Shared constants, textures, structs, and helper functions
// =============================================================

cbuffer FrameCB : register(b0)
{
    matrix view;
    matrix proj;
    float3 camPos;
    float pad0;
    float3 sunDir;
    float turbidity;
    float3 probeDims;
    float ssaoRadius;
    float3 sunColor;
    float scatterCoeff;
    float scatterStep;
    float sssStrength;
    float focalDist;
    float blurScale;
    float width;
    float height;
};

cbuffer ObjectCB : register(b1)
{
    matrix world;
    matrix worldInv;
};

struct DirLight
{
    float3 dir;
    float pad1;
    float3 color;
    float pad2;
};
cbuffer LightCB : register(b2)
{
    DirLight lights[3];
    uint lightCount;
    float3 pad3;
};

StructuredBuffer<float4x4> BoneMatrices : register(t3);

Texture2D albedoMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D metallicRoughAO : register(t2);
Texture2D depthMap : register(t4);
Texture2D noiseMap : register(t5);
Texture2D probeAtlas : register(t6);
Texture3D volumeDensity : register(t7);

SamplerState linearSamp : register(s0);
SamplerState pointSamp : register(s1);

struct VSInput
{
    float3 pos : POSITION;
    float3 norm : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD0;
    uint4 BoneIdx : BLENDINDICES;
    float4 BoneWt : BLENDWEIGHT;
};

struct VSOutputPBR
{
    float4 clipPos : SV_POSITION;
    float3 worldPos : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float3 tangent : TEXCOORD2;
    float3 bitan : TEXCOORD3;
    float2 uv : TEXCOORD4;
    float rough2 : TEXCOORD5;
    float3 F0 : TEXCOORD6;
};

float GetSSAO(float3 pos, float2 uv)
{
    float oc = 0;
    static float3 kernel[16]; // prefill on CPU
    float2 noiseScale = float2(width / 4.0, height / 4.0);
    float3 rn = noiseMap.Sample(pointSamp, uv * noiseScale).xyz;
    float3 tangent = normalize(rn - pos * dot(rn, pos));
    float3 bitan = cross(pos, tangent);
    float3 samples[16]; // precomputed
    for (int i = 0; i < 16; ++i)
    {
        float3 sp = pos + samples[i] * ssaoRadius;
        float4 projPos = mul(proj, mul(view, float4(sp, 1)));
        projPos.xyz /= projPos.w;
        float sceneDepth = depthMap.Sample(linearSamp, projPos.xy * 0.5 + 0.5).r;
        if (sceneDepth < projPos.z)
            oc += 1;
    }
    return 1 - oc / 16;
}

float3 ComputeSky(float3 viewDir)
{
    float mu = dot(viewDir, sunDir);
    float3 rayleigh = exp(-turbidity * (1 - mu));
    float3 mie = pow(max(mu, 0), 5);
    return rayleigh + mie * 0.1;
}

float3 Volumetric(float3 worldPos)
{
    float3 step = sunDir * scatterStep;
    float sum = 0;
    float3 pos = worldPos;
    [unroll]
    for (int i = 0; i < 32; ++i)
    {
        pos += step;
        float d = volumeDensity.SampleLevel(linearSamp, pos, 0).r;
        sum += exp(-d * scatterCoeff);
    }
    return sunColor * sum * 0.05;
}

float3 Subsurface(float2 uv, float3 diffuse, float ao)
{
    float3 sss = 0;
    static float w[5] = { 0.05, 0.25, 0.4, 0.25, 0.05 };
    for (int i = -2; i <= 2; ++i)
        for (int j = -2; j <= 2; ++j)
            sss += albedoMap.Sample(linearSamp, uv + float2(i, j) * 0.002).rgb * w[i + 2] * w[j + 2];
    return lerp(diffuse, sss, sssStrength) * ao;
}

float4 Blur5(Texture2D tex, float2 uv, float2 dir)
{
    float w[5] = { 0.0625, 0.25, 0.375, 0.25, 0.0625 };
    float4 sum = 0;
    [unroll]
    for (int i = 0; i < 5; ++i)
        sum += tex.Sample(linearSamp, uv + dir * (i - 2)) * w[i];
    return sum;
}
