Texture2D brightTex : register(t0);
SamplerState linearSamp : register(s0);

cbuffer PostCB : register(b0)
{
    float2 invResolution;
    float2 pad;
};

float4 Blur5(Texture2D tex, float2 uv, float2 dir)
{
    const float w[5] = { 0.0625, 0.25, 0.375, 0.25, 0.0625 };
    float4 sum = 0;

    [unroll]
    for (int i = 0; i < 5; ++i)
    {
        float2 offs = dir * (i - 2);
        sum += tex.Sample(linearSamp, uv + offs) * w[i];
    }

    return sum;
}

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
    float2 dirH = float2(invResolution.x, 0);
    float2 dirV = float2(0, invResolution.y);

    float4 blurH = Blur5(brightTex, I.uv, dirH);
    float4 blurV = Blur5(brightTex, I.uv, dirV);

    return blurV;
}
