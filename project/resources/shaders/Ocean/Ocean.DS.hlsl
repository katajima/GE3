#include "Ocean.hlsli"

// 波のパラメータ
struct WaveParameters
{
    float amplitude; // 波の振幅
    float frequency; // 波の周波数
    float speed; // 波の速度
    float time; // 現在の時間
};

ConstantBuffer<WaveParameters> gWaveParameters : register(b5);

[domain("quad")]
DS_OUTPUT main(
    HS_CONSTANT_DATA_OUTPUT input,
    float2 domain : SV_DomainLocation, // x, yの2つのバリセントリック座標
    const OutputPatch<HS_CONTROL_POINT_OUTPUT, 4> patch) // 4制御点
{
    DS_OUTPUT Output;

    // 四辺形のバリセントリック座標を使って位置を補間
    float3 WorldPosition =
        patch[0].vPosition * (1.0f - domain.x - domain.y + domain.x * domain.y) + // 左上
        patch[1].vPosition * domain.x * (1.0f - domain.y) + // 右上
        patch[3].vPosition * domain.x * domain.y + // 右下
        patch[2].vPosition * (1.0f - domain.x) * domain.y; // 左下

    // 波の影響をZ軸に加える（Y軸を基準に）
    WorldPosition.z += gWaveParameters.amplitude * (
        cos(gWaveParameters.frequency * WorldPosition.y - gWaveParameters.speed * gWaveParameters.time) +
        sin(gWaveParameters.frequency * (WorldPosition.x + WorldPosition.y) - gWaveParameters.speed * gWaveParameters.time)
    );

    // ワールド座標を保存
    Output.vWorldPos = WorldPosition;

    // スクリーン座標に変換
    Output.vPosition = mul(float4(WorldPosition, 1.0), gTransformationMatrix.WVP);

    // --- 法線の再計算 ---
    float dX = -gWaveParameters.amplitude * gWaveParameters.frequency *
               sin(gWaveParameters.frequency * (WorldPosition.x + WorldPosition.y) - gWaveParameters.speed * gWaveParameters.time);
    
    float dY = -gWaveParameters.amplitude * gWaveParameters.frequency * (
               sin(gWaveParameters.frequency * WorldPosition.y - gWaveParameters.speed * gWaveParameters.time) +
               cos(gWaveParameters.frequency * (WorldPosition.x + WorldPosition.y) - gWaveParameters.speed * gWaveParameters.time)
    );

    float dZ = 1.0; // Z軸方向の基準（波をZ軸に適用しているため）

    // 正しい接線ベクトル
    float3 tangent = normalize(float3(1.0, dX, 0.0)); // X方向の変化
    float3 bitangent = normalize(float3(0.0, dY, dZ)); // Y方向の変化

    // 法線を計算（tangent × bitangent）
    Output.vNormal = normalize(cross(tangent, bitangent));

    // テクスチャ座標の計算
    Output.texcoord = WorldPosition.xy; // 仮にX, Y 座標をテクスチャ座標に使う

    return Output;
}
