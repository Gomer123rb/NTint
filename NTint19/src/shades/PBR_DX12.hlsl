// =============================================================
// PBR_DX12.hlsl — DXC‑clean, no texture sampling in VS
// =============================================================

#include "PBR_Common.hlsl"

float3 EvaluateLighting(float3 N, float3 V, float3 F0, float rough2)
{
    float3 L = normalize(float3(0.5, 1.0, 0.3));
    float NdotL = saturate(dot(N, L));
    float3 diffuse = NdotL * float3(1.0, 1.0, 1.0);

    float specPow = 1.0 / rough2;
    float3 spec = F0 * pow(saturate(dot(N, normalize(L + V))), specPow);

    return diffuse + spec;
}

float3 RefinedColor(float3 a, float3 b, float3 c, float3 d)
{
    return (a + b + c + d) * 0.25;
}

VSOutputPBR VSMain(VSInput IN)
{
    VSOutputPBR O;

    float4x4 skin = (float4x4) 0;
    [unroll]
    for (int i = 0; i < 4; ++i)
        skin += BoneMatrices[IN.BoneIdx[i]] * IN.BoneWt[i];

    float4 p = mul(float4(IN.pos, 1), skin);
    float3 n = normalize(mul(float4(IN.norm, 0), skin).xyz);
    float3 t = normalize(mul(float4(IN.tangent, 0), skin).xyz);

    float4 wp = mul(p, world);
    O.worldPos = wp.xyz;
    O.clipPos = mul(mul(wp, view), proj);

    O.normal = normalize(mul(float4(n, 0), worldInv).xyz);
    O.tangent = normalize(mul(float4(t, 0), world).xyz);
    O.bitan = cross(O.normal, O.tangent);

    O.uv = IN.uv;

    O.rough2 = 0;
    O.F0 = float3(0, 0, 0);

    return O;
}

float4 PSMain(VSOutputPBR IN) : SV_TARGET
{
    float3 albedo = pow(albedoMap.Sample(linearSamp, IN.uv).rgb, 2.2);
    float3 mro = metallicRoughAO.Sample(linearSamp, IN.uv).rgb;

    float metallic = mro.r;
    float rough = saturate(mro.g);
    float ao = mro.b;

    float rough2 = max(rough * rough, 1e-4);
    float3 F0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallic);

    float3 V = normalize(camPos - IN.worldPos);
    float3 N = normalize(IN.normal);

    float3 lit = EvaluateLighting(N, V, F0, rough2);
    float3 finalColor = RefinedColor(lit, lit, lit, lit);

    float3 tonemap = pow(saturate(finalColor), 1.0 / 2.2);
    return float4(tonemap, 1);
}
