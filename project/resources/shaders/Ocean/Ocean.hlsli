// Ocean.hlsli
#define NUM_CONTROL_POINTS 4

struct VertexShaderOutput
{
    float4 position : SV_POSITION; // スクリーン座標
    float2 texcoord : TEXCOORD0; // テクスチャ座標
    float3 normal : NORMAL; // 法線
    float3 worldPosition : WORLDPOS; // ワールド座標
};

struct VS_INPUT
{
    float3 position : POSITION0; // 頂点位置
    float2 texcoord : TEXCOORD0; // テクスチャ座標
    float3 normal : NORMAL0; // 法線
};

struct VS_CONTROL_POINT_OUTPUT
{
    float3 vPosition : WORLDPOS; // ワールド座標としての制御点位置
};

struct PixelShaderInput
{
    float4 position : SV_POSITION; // スクリーン座標
    float2 texcoord : TEXCOORD0; // テクスチャ座標
    float3 normal : NORMAL; // 法線
    float3 worldPosition : WORLDPOS; // ワールド座標
};
////------PixelShader------////
struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};




// ドメインシェーダーの出力構造体
struct DS_OUTPUT
{
    float4 vPosition : SV_POSITION; // スクリーン座標
    float2 texcoord : TEXCOORD0; // テクスチャ座標
    float3 vNormal : NORMAL; // 法線
    float3 vWorldPos : WORLDPOS; // ワールド座標
};

// ハルシェーダーから渡されるデータ構造
struct HS_CONTROL_POINT_OUTPUT
{
    float3 vPosition : WORLDPOS; // ワールド座標
};

// ハルシェーダーの定数データ
struct HS_CONSTANT_DATA_OUTPUT
{
    // 4辺のテッセレーション係数を格納する配列（4 辺）
    float EdgeTessFactor[4] : SV_TessFactor;

    // 内部のテッセレーション係数を格納する配列（2 値）
    float InsideTessFactor[2] : SV_InsideTessFactor;
};


struct TransformationMatrix
{
    float4x4 WVP; // ワールド・ビュー・プロジェクション行列
    float4x4 World; // ワールド行列
    float4x4 WorldInverseTranspose; // ワールド行列の逆転置行列（法線計算用）
};
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);



