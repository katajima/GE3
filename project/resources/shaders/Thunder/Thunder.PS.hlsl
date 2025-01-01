#include "Thunder.hlsli"

// 定数バッファで外部パラメータを取得
struct ThunderParameters
{
    float falloffRate; // 減衰率
    float3 baseColorStart; // 雷の暗い部分の色
    float3 baseColorEnd; // 雷の明るい部分の色
    float alphaScale; // アルファ値スケール
};
ConstantBuffer<ThunderParameters> gThunderParameters : register(b1);

float4 PixelShaderFunction(ShaderIO input) : SV_Target
{
    // 雷の輝き計算
    float glow = exp(-pow(input.intensity *gThunderParameters.falloffRate, 2.0)); // 中心で明るく外側で減衰

    // 雷の色 (外部パラメータで設定)
    float3 baseColor = lerp(gThunderParameters.baseColorStart, gThunderParameters.baseColorEnd, glow);

    // フェード効果 (アルファ値を調整可能に)
    float alpha = glow * gThunderParameters.alphaScale;

    return float4(baseColor * glow, alpha);
}
