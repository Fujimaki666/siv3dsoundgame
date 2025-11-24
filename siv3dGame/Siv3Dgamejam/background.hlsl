// 背景色グラデーション用シェーダ（テクスチャ無し）
// ファイル名: background.hlsl

// ===== Siv3D VS/PS 定義 =====
namespace s3d
{
    struct PSInput
    {
        float4 position : SV_POSITION;
        float4 color : COLOR0;
        float2 uv : TEXCOORD0;
    };
}

// ===== Constant Buffer (Siv3D 必須) =====
cbuffer PSConstants2D : register(b0)
{
    float4 g_colorAdd;
    float4 g_sdfParam;
    float4 g_sdfOutlineColor;
    float4 g_sdfShadowColor;
    float4 g_internal;
}

// ===== Pixel Shader (背景色) =====
float4 PS_Shape(s3d::PSInput input) : SV_TARGET
{
    float3 bottom = float3(0.9, 0.9, 1.0);
    float3 top = float3(0.2, 0.2, 0.2);

    float3 col = lerp(bottom, top, input.uv.y);

    return float4(col, 1.0) + g_colorAdd;
}
