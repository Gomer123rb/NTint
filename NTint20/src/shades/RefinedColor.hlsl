cbuffer ColorSettings : register(b0)
{
    float Saturation;
    float Contrast;
    float Gamma;
    float3 TintColor;
    float TintStrength;
}

Texture2D InputTexture : register(t0);
SamplerState Sampler : register(s0);

struct VSInput
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

float3 ApplySaturation(float3 color, float saturation)
{
    float gray = dot(color, float3(0.3, 0.59, 0.11));
    return lerp(float3(gray, gray, gray), color, saturation);
}

float3 ApplyContrast(float3 color, float contrast)
{
    return (color - 0.5) * contrast + 0.5;
}

float3 ApplyGamma(float3 color, float gamma)
{
    return pow(color, 1.0 / gamma);
}

PSInput VSMain(VSInput input)
{
    PSInput o;
    o.pos = float4(input.pos, 1.0);
    o.uv = input.uv;
    return o;
}

float4 PSMain(PSInput input) : SV_Target
{
    float4 baseColor = InputTexture.Sample(Sampler, input.uv);
    float3 color = baseColor.rgb;

    color = ApplySaturation(color, Saturation * 0.5 + 1);
    color = ApplyContrast(color, Contrast * 0.5 + 1);
    color = ApplyGamma(color, Gamma * 0.5 + 1);
    color = lerp(color, TintColor, TintStrength);

    return float4(color, baseColor.a);
}
