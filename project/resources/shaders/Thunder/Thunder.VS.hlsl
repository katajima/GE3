#include "Thunder.hlsli"

// ランダム値を生成する関数
float randomOffset(float2 uv, float scale)
{
    float rand = frac(sin(dot(uv, float2(12.9898, 78.233))) * 43758.5453);
    return (rand - 0.5) * scale; // スケール調整 (-scale/2 ～ scale/2)
}

// 雷の中心からの強度を計算する関数
float computeIntensity(float3 position, float centerX, float falloff)
{
    return saturate(1.0 - abs(position.x - centerX) * falloff);
}

// 頂点シェーダー関数
ShaderIO VertexShaderFunction(float3 position : POSITION, float3 normal : NORMAL, float2 texcoord : TEXCOORD0)
{
    ShaderIO output;

    // ワールド空間座標への変換
    float4 worldPosition = mul(float4(position, 1.0), gTransformationMatrix.World);

    // ランダムなオフセットを適用
    float randomOffsetValue = randomOffset(texcoord, 0.1); // 例: スケール 0.1
    worldPosition.x += randomOffsetValue;

    // スクリーン座標変換
    float4 worldViewProjPosition = mul(worldPosition, gTransformationMatrix.WVP);

    // 法線ベクトルの変換 (ワールド空間)
    float3 transformedNormal = mul(float4(normal, 0.0), gTransformationMatrix.WorldInverseTranspose).xyz;

    // 雷の中心からの強度を計算 (中心を x=0、減衰係数 1.0 と仮定)
    float intensity = computeIntensity(worldPosition.xyz, 0.0, 1.0);

    // 出力データの設定
    output.position = worldViewProjPosition; // スクリーン座標
    output.worldPosition = worldPosition.xyz; // ワールド空間座標
    output.normal = transformedNormal; // 法線
    output.texcoord = texcoord; // テクスチャ座標
    output.intensity = intensity; // 雷の強度

    return output;
}
