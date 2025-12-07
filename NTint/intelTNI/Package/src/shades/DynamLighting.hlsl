//dx12 and directmod code idk
//----------------------------------------
// uh idk what to code here i guess, so i guess ill just code shaders
#include "PBR.hlsl"


[RootSignature(
    "CBV(b0,space=0),"
    "CBV(b1,space=0),"
    "CBV(b2,space=0),"
    "DescriptorTable(SRV(t0,t1,t2),space=0),"
    "DescriptorTable(SRV(t3),space=0),"
    "DescriptorTable(SRV(t4),space=0),"
    "DescriptorTable(SRV(t5),space=0),"
    "DescriptorTable(SRV(t6),space=0),"
    "DescriptorTable(SRV(t7),space=0),"
    "StaticSampler(s0,filter=MIN_MAG_MIP_LINEAR),"
    "StaticSampler(s1,filter=POINT)"
)]

// Constant buffers
cbuffer FrameCB : register(b0, space0)
{
    float4x4 view;
    float4x4 proj;
    float3   camPos;
    float    pad0;
    float3   sunDir;
    float    pad1;
    float    turbidity;
};

cbuffer ObjectCB : register(b1, space0)
{
    float4x4 world;
    float4x4 worldInv;
};

cbuffer LightCB : register(b2, space0)
{
    // just a few vars for now, can't wait to have NTint v0.1.8!!!
    float4 lightColor;
    float4 lightDir;
};

// Resources
Texture2D    albedoMap        : register(t0, space0);
Texture2D    metallicRoughAO  : register(t1, space0);
Texture2D    normalMap        : register(t2, space0);
Texture2D    boneMatricesTex  : register(t3, space0); // if using texture-backed bones; otherwise use StructuredBuffer
StructuredBuffer<float4x4> BoneMatrices : register(t3, space0);

SamplerState linearSamp : register(s0);
SamplerState pointSamp  : register(s1);

// Vertex / Pixel IO
struct VSInput
{
    float3 pos     : POSITION;
    float3 norm    : NORMAL;
    float3 tangent : TANGENT;
    float2 uv      : TEXCOORD0;
    uint4  BoneIdx : BLENDINDICES;
    float4 BoneWt  : BLENDWEIGHT;
};

struct VSOutput
{
    float4 clipPos  : SV_POSITION;
    float3 worldPos : TEXCOORD0;
    float3 normal   : TEXCOORD1;
    float3 tangent  : TEXCOORD2;
    float3 bitan    : TEXCOORD3;
    float2 uv       : TEXCOORD4;
    float3 F0       : TEXCOORD5;
    float  rough2   : TEXCOORD6;
};

// Function definitions (outside VSMain)
float3 EvaluateLighting(float3 normal, float3 viewDir, float3 F0, half rough2)
{
    // Example lighting logic (placeholder)
    float3 lightDir = normalize(float3(0.5, 1.0, 0.3));
    float NdotL = saturate(dot(normal, lightDir));
    float3 diffuse = NdotL * float3(1.0, 1.0, 1.0); // white light
    float specPower = 1.0 / rough2;
    float3 specular = F0 * pow(saturate(dot(normal, normalize(lightDir + viewDir))), specPower);
    return diffuse + specular;
}

float3 RefinedColor(float3 r, float3 g, float3 b, float3 a)
{
    // Simple average for now
    return (r + g + b + a) * 0.25;
}

// Vertex Shader Entry Point
VSOutput VSMain(VSInput IN)
{
    VSOutput O;

    // Skinning
    float4x4 skinMat = float4x4(0);
    [unroll] for (int i = 0; i < 4; ++i)
        skinMat += BoneMatrices[IN.BoneIdx[i]] * IN.BoneWt[i];

    float4 skP = mul(float4(IN.pos, 1), skinMat);
    float3 skN = normalize(mul(float4(IN.norm, 0), skinMat).xyz);
    float3 skT = normalize(mul(float4(IN.tangent, 0), skinMat).xyz);

    // World transforms
    float4 wP4 = mul(skP, world);
    O.worldPos = wP4.xyz;
    O.clipPos = mul(mul(wP4, view), proj);

    // Tangent space
    O.normal = normalize(mul(float4(skN, 0), worldInv).xyz);
    O.tangent = normalize(mul(float4(skT, 0), world).xyz);
    O.bitan = cross(O.normal, O.tangent);

    // UV and material sampling
    O.uv = IN.uv;
    float3 mro = metallicRoughAO.Sample(linearSamp, IN.uv).rgb;
    half metallic = mro.r;
    half rough = saturate(mro.g);
    half rough2 = max(rough * rough, 1e-4); // avoid div-by-zero
    O.rough2 = rough * rough;

    float3 albedo = pow(albedoMap.Sample(linearSamp, IN.uv).rgb, 2.2);
    O.F0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallic);

    return O;
}

float4 PSMain(VSOutput IN) : SV_TARGET
{
    float3 viewDir = normalize(camPos - IN.worldPos);
    float3 lit = EvaluateLighting(IN.normal, viewDir, IN.F0, IN.rough2);
    float3 finalColor = RefinedColor(lit, lit, lit, lit); // placeholder

    float3 tonemapped = saturate(finalColor); // small placeholder
    tonemapped = pow(tonemapped, 1.0/2.2); // gamma correction
    return float4(tonemapped, 1);
}