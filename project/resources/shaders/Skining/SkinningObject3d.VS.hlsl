#include"SkinningObject3d.hlsli"
struct TransformationMatrix
{
    float4x4 WVP;
    float4x4 World;
    float4x4 WorldInverseTranspose;
};
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);


struct Well
{
    float4x4 skeletonSpaceMatrix;
    float4x4 skeletonSpaceInverseTransposeMatrix;
};
StructuredBuffer<Well> gMatrixPalette : register(t4);

struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    // 追加
    float3 tangent : TANGENT0; // 接ベクトル
    float3 biNormal : BINORMAL0; // 従ベクトル
     // スキニング
    float4 weight : WEIGHT0;
    int4 index : INDEX0;
};
struct Skinned
{
    float4 position;
    float3 normal;
};
Skinned Skinning(VertexShaderInput input)
{
    Skinned skinned;
    
    // 位置の変換
    skinned.position = mul(input.position, gMatrixPalette[input.index.x].skeletonSpaceMatrix) * input.weight.x;
    skinned.position += mul(input.position, gMatrixPalette[input.index.y].skeletonSpaceMatrix) * input.weight.y;
    skinned.position += mul(input.position, gMatrixPalette[input.index.z].skeletonSpaceMatrix) * input.weight.z;
    skinned.position += mul(input.position, gMatrixPalette[input.index.w].skeletonSpaceMatrix) * input.weight.w;
    skinned.position.w = 1.0f; // 確実に1を入れる

   // 法線の変換
    skinned.normal =  mul(input.normal, (float3x3) gMatrixPalette[input.index.x].skeletonSpaceInverseTransposeMatrix) * input.weight.x;
    skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[input.index.y].skeletonSpaceInverseTransposeMatrix) * input.weight.y;
    skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[input.index.z].skeletonSpaceInverseTransposeMatrix) * input.weight.z;
    skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[input.index.w].skeletonSpaceInverseTransposeMatrix) * input.weight.w;
    skinned.normal = normalize(skinned.normal);

    
    return skinned;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    Skinned skinned = Skinning(input);
    
    
    output.position = mul(skinned.position, gTransformationMatrix.WVP);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(skinned.normal, (float3x3) gTransformationMatrix.WorldInverseTranspose));
    output.worldPosition = mul(skinned.position, gTransformationMatrix.World).xyz;
    // 頂点シェーダでの法線、接ベクトル、従ベクトルの変換 
    output.biNormal = normalize(mul((float3x3) gTransformationMatrix.World, (float3) input.biNormal));
    output.tangent = normalize(mul((float3x3) gTransformationMatrix.World, (float3) input.tangent));

    // ピクセルシェーダで使用する法線を計算 
    output.transformedNormal = normalize(mul((float3x3) gTransformationMatrix.WorldInverseTranspose, input.normal));
    
    return output;
}