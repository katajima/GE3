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

    // 複数方向の波を加算する
    // 波の進行方向を設定
    float waveX = gWaveParameters.amplitude *
                  cos(gWaveParameters.frequency * WorldPosition.x - 
                      gWaveParameters.speed * gWaveParameters.time);
    float waveZ = gWaveParameters.amplitude *
                  cos(gWaveParameters.frequency * WorldPosition.z - 
                      gWaveParameters.speed * gWaveParameters.time);
    float waveDiagonal = gWaveParameters.amplitude *
                         sin(gWaveParameters.frequency * (WorldPosition.x + WorldPosition.z) - 
                             gWaveParameters.speed * gWaveParameters.time);

    // 複数の方向で波を加算
    WorldPosition.y += waveX + waveZ + waveDiagonal;

    // ワールド座標を保存
    Output.vWorldPos = WorldPosition;

    // スクリーン座標に変換
    Output.vPosition = mul(float4(WorldPosition, 1.0), gTransformationMatrix.WVP);

    
    
    
    
    // 法線の再計算（前後の位置から計算）
    float dX = gWaveParameters.amplitude * gWaveParameters.frequency *
               cos(gWaveParameters.frequency * WorldPosition.x - 
                   gWaveParameters.speed * gWaveParameters.time);
    float dZ = gWaveParameters.amplitude * gWaveParameters.frequency *
               cos(gWaveParameters.frequency * WorldPosition.z - 
                   gWaveParameters.speed * gWaveParameters.time);
    float dDiag = gWaveParameters.amplitude * gWaveParameters.frequency *
                  cos(gWaveParameters.frequency * (WorldPosition.x + WorldPosition.z) - 
                      gWaveParameters.speed * gWaveParameters.time);
    
    // 複数方向の波の接線とビタングントを計算
    float3 tangent = float3(1.0, dX, 0.0f); // X, Z軸方向の接線
    float3 bitangent = float3(1.0, 0.0f, 1.0f); // 斜め方向のビタングント（必要に応じて調整）

    // 法線ベクトルの計算（接線とビタングントの外積）
    Output.vNormal = normalize(cross(bitangent, tangent));

    // テクスチャ座標の計算（適当な方法で計算、例えばUVマッピングなど）
    Output.texcoord = WorldPosition.xy; // 仮にX, Y 座標をテクスチャ座標に使う

    return Output;
}



