// DynamLighting.hlsl — FXC-safe, no skinning, no texture sampling in VS

#include "PBR_Common.hlsl"

// Uses cbuffers, textures, samplers from PBR_Common.hlsl only.

struct VSInputDyn
{
    float3 pos     : POSITION;
    float3 norm    : NORMAL;
    float3 tangent : TANGENT;
    float2 uv      : TEXCOORD0;
};

struct VSOutputDyn
{
    float4 clipPos  : SV_POSITION;
    float3 worldPos : TEXCOORD0;
    float3 normal   : TEXCOORD1;
    float3 tangent  : TEXCOORD2;
    float3 bitan    : TEXCOORD3;
    float2 uv       : TEXCOORD4;
};

float3 EvaluateLighting(float3 normal, float3 viewDir, float3 F0, half rough2)
{
    float3 lightDir = normalize(float3(0.5, 1.0, 0.3));
    float  NdotL    = saturate(dot(normal, lightDir));
    float3 diffuse  = NdotL * float3(1.0, 1.0, 1.0);

    float  specPower = 1.0 / rough2;
    float3 halfVec   = normalize(lightDir + viewDir);
    float3 specular  = F0 * pow(saturate(dot(normal, halfVec)), specPower);

    return diffuse + specular;
}

float3 RefinedColor(float3 r, float3 g, float3 b, float3 a)
{
    return (r + g + b + a) * 0.25;
}

// Vertex shader: geometry only, no texture sampling
VSOutputDyn VSMain(VSInputDyn IN)
{
    VSOutputDyn O;

    float4 wP4 = mul(float4(IN.pos, 1.0f), world);
    O.worldPos = wP4.xyz;
    O.clipPos  = mul(mul(wP4, view), proj);

    float3 n = normalize(mul(float4(IN.norm,    0.0f), worldInv).xyz);
    float3 t = normalize(mul(float4(IN.tangent, 0.0f), world).xyz);
    float3 b = cross(n, t);

    O.normal  = n;
    O.tangent = t;
    O.bitan   = b;

    O.uv = IN.uv;

    return O;
}

// Pixel shader: sample textures, compute PBR terms, do lighting
float4 PSMain(VSOutputDyn IN) : SV_TARGET
{
    float3 mro = metallicRoughAO.Sample(linearSamp, IN.uv).rgb;
    half metallic = mro.r;
    half rough    = saturate(mro.g);
    half rough2   = max(rough * rough, 1e-4);

    float3 albedo = pow(albedoMap.Sample(linearSamp, IN.uv).rgb, 2.2);
    float3 F0     = lerp(float3(0.04, 0.04, 0.04), albedo, metallic);

    float3 viewDir = normalize(camPos - IN.worldPos);
    float3 lit     = EvaluateLighting(IN.normal, viewDir, F0, rough2);
    float3 finalColor = RefinedColor(lit, lit, lit, lit);

    float3 tonemapped = saturate(finalColor);
    tonemapped = pow(tonemapped, 1.0 / 2.2);

    return float4(tonemapped, 1);
}
