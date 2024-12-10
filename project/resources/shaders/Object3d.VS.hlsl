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
    // �ǉ�
    float3 tangent : TANGENT0; // �ڃx�N�g��
    float3 biNormal : BINORMAL0; // �]�x�N�g��
    
    
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    
    output.position = mul(input.position, gTransformationMatrix.WVP);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float32_t3x3)gTransformationMatrix.WorldInverseTranspose));
    output.worldPosition = mul(input.position, gTransformationMatrix.World).xyz;
    // ���_�V�F�[�_�ł̖@���A�ڃx�N�g���A�]�x�N�g���̕ϊ� 
    output.biNormal = normalize(mul((float3x3) gTransformationMatrix.World, (float3) input.biNormal));
    output.tangent = normalize(mul((float3x3) gTransformationMatrix.World, (float3) input.tangent));

    // �s�N�Z���V�F�[�_�Ŏg�p����@�����v�Z 
    output.transformedNormal = normalize(mul((float3x3)gTransformationMatrix.WorldInverseTranspose, input.normal));
    
    return output;
}


