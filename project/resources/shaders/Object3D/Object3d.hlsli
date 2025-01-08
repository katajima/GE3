
struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : POSITION0;
     // 追加
    float3 tangent : TANGENT0; // 接ベクトル
    float3 biNormal : BINORMAL0; // 従ベクトル
    float3 transformedNormal : NORMAL1; // ピクセルシェーダで使用する法線
};






