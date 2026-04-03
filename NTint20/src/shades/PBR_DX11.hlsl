// =============================================================
// PBR_DX11.hlsl
// DX11 PBR with MRT for HDR + bloom
// =============================================================

#include "PBR_Common.hlsl"

struct PSOut
{
    float4 hdr : SV_TARGET0; // full HDR color
    float4 bright : SV_TARGET1; // bright parts for bloom
};

PSOut PSMain(VSOutputPBR I)
{
    PSOut O;

    float3 N = normalize(I.normal);
    float3 V = normalize(camPos - I.worldPos);

    float3 albedo = pow(albedoMap.Sample(linearSamp, I.uv).rgb, 2.2);
    float3 mro = metallicRoughAO.Sample(linearSamp, I.uv).rgb;
    float metallic = mro.r;
    float rough = mro.g;
    float ao = mro.b;

    float3 F0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallic);

    float3 Lo = 0;
    [loop]
    for (uint i = 0; i < lightCount; ++i)
    {
        float3 L = normalize(-lights[i].dir);
        float3 H = normalize(V + L);
        float NdotL = saturate(dot(N, L));
        float NdotV = saturate(dot(N, V));
        float NdotH = saturate(dot(N, H));
        float HdotV = saturate(dot(H, V));

        float a2 = I.rough2 * I.rough2;
        float denom = (NdotH * NdotH * (a2 - 1) + 1);
        float D = a2 / (3.14159 * denom * denom + 1e-5);

        float k = (I.rough2 + 1) * (I.rough2 + 1) / 8.0;
        float Gv = NdotV / (NdotV * (1 - k) + k);
        float Gl = NdotL / (NdotL * (1 - k) + k);
        float G = Gv * Gl;

        float3 F = F0 + (1 - F0) * pow(1 - HdotV, 5);

        float3 spec = (D * G * F) / (4 * NdotV * NdotL + 1e-5);
        float3 kd = (1 - F) * (1 - metallic);

        float3 diff = kd * albedo / 3.14159;

        Lo += (diff + spec) * lights[i].color * NdotL;
    }

    float3 pbrColor = Lo * ao;

    float3 viewDir = normalize(I.worldPos - camPos);
    float3 sky = ComputeSky(viewDir);
    pbrColor += sky * 0.1;
    pbrColor += Volumetric(I.worldPos);
    pbrColor = Subsurface(I.uv, pbrColor, ao);

    O.hdr = float4(pbrColor, 1);

    float3 brightMask = step(float3(1, 1, 1), pbrColor);
    float3 bright = pbrColor * brightMask;
    O.bright = float4(bright, 1);

    return O;
}
