Texture2D hdrTex : register(t0);
Texture2D bloomTex : register(t1);
Texture2D depthMap : register(t2);
SamplerState linearSamp : register(s0);

cbuffer PostCB : register(b0)
{
    float focalDist;
    float blurScale;
    float2 invResolution;
};

struct VSInput
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct VSQuadOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

VSQuadOut VSMain(VSInput IN)
{
    VSQuadOut O;
    O.pos = float4(IN.pos, 1.0);
    O.uv = IN.uv;
    return O;
}

float4 PSMain(VSQuadOut I) : SV_TARGET
{
    float3 hdr = hdrTex.Sample(linearSamp, I.uv).rgb;
    float3 bloom = bloomTex.Sample(linearSamp, I.uv).rgb;

    float depth = depthMap.Sample(linearSamp, I.uv).r;
    float coc = saturate(abs((depth - focalDist) / depth) * blurScale);

    float3 color = hdr + bloom;
    color = color / (color + 1);
    color = pow(color, 1.0 / 2.2);

    return float4(color, 1);
}
