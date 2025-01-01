// Thunder.hlsli

// 頂点シェーダーおよびピクセルシェーダー間のデータ構造
struct ShaderIO
{
    float4 position : SV_POSITION; // スクリーン座標
    float2 texcoord : TEXCOORD0; // テクスチャ座標 (UV)
    float3 normal : NORMAL0; // 法線ベクトル (ワールド空間)
    float3 worldPosition : POSITION0; // ワールド座標
    float intensity : TEXCOORD1; // 雷の輝き強度
};


struct TransformationMatrix
{
    float4x4 WVP; // ワールド・ビュー・プロジェクション行列
    float4x4 World; // ワールド行列
    float4x4 WorldInverseTranspose; // ワールド行列の逆転置行列（法線計算用）
    float time; // 時間 (秒単位)
    float3 padding; // アライメント調整用
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);