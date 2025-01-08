#include"object3d.hlsli"

struct TransformationMatrix
{
    float32_t4x4 WVP;
    float32_t4x4 World;
    float32_t4x4 WorldInverseTranspose;
};
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

 
struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
    // 追加
    float3 tangent : TANGENT0; // 接ベクトル
    float3 biNormal : BINORMAL0; // 従ベクトル 
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    
    output.position = mul(input.position, gTransformationMatrix.WVP);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float32_t3x3)gTransformationMatrix.WorldInverseTranspose));
    output.worldPosition = mul(input.position, gTransformationMatrix.World).xyz;
    // 頂点シェーダでの法線、接ベクトル、従ベクトルの変換 
    output.biNormal = normalize(mul((float3x3) gTransformationMatrix.World, (float3) input.biNormal));
    output.tangent = normalize(mul((float3x3) gTransformationMatrix.World, (float3) input.tangent));

    // ピクセルシェーダで使用する法線を計算 
    output.transformedNormal = normalize(mul((float3x3)gTransformationMatrix.WorldInverseTranspose, input.normal));
    
    return output;
}


