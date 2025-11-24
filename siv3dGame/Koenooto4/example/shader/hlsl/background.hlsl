// background.hlsl

namespace s3d
{
    struct VSInput
    {
        float2 position : POSITION;
        float2 uv       : TEXCOORD0;
        float4 color    : COLOR0;
    };

    struct PSInput
    {
        float4 position : SV_POSITION;
        float2 uv       : TEXCOORD0;
        float4 color    : COLOR0;
    };
}

// Siv3D 既定の 2D 定数バッファ
cbuffer VSConstants2D : register(b0)
{
    row_major float2x4 g_transform;
    float4 g_colorMul;
}

cbuffer PSConstants2D : register(b0)
{
    float4 g_colorAdd;
    float4 g_sdfParam;
    float4 g_sdfOutlineColor;
    float4 g_sdfShadowColor;
    float4 g_internal;
}

// ==== Vertex Shader ====
// 余計な関数を作らず、ここで直接作る
s3d::PSInput VSMain(s3d::VSInput input)
{
    s3d::PSInput o;

    // 2D 変換行列で (x,y) -> (x',y') を作成
    // x' = dot((x,y), _11_21) + _13_23
    // y' = dot((x,y), _12_22) + _14_24
    float2 p = input.position;
    float2 xy = g_transform._13_23 + p.x * g_transform._11_21 + p.y * g_transform._12_22;

    // z,w は行列の 2 行目の 3,4 列を使うのが Siv3D の既定
    float2 zw = g_transform._23_24;

    o.position = float4(xy, zw);
    o.uv       = input.uv;
    o.color    = input.color * g_colorMul;
    return o;
}

// ==== Pixel Shader ====
float4 PSMain(s3d::PSInput input) : SV_TARGET
{
    // 下→上へのグラデーション
    float3 bottom = float3(0.9, 0.9, 1.0);
    float3 top    = float3(0.2, 0.2, 0.2);
    float3 col    = lerp(bottom, top, input.uv.y);
    return float4(col, 1.0) + g_colorAdd;
}
