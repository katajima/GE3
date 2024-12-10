#include"Object2D.hlsli"

//�F�ȂǎO�p�`�̕\�ʂ̍ގ������肷�����Material
struct Material
{
    
    float32_t4 color;
};

ConstantBuffer<Material> gMaterial : register(b0);
//ConstantBuffer<Material> gMaterial1 : register(b1);
//ConstantBuffer<Material> gMaterial2 : register(b2);
Texture2D<float32_t4> gTexture : register(t0);
//Texture2D<float32_t4> gTexture1 : register(t4);
SamplerState sSampler : register(s0);

////------PixelShader------////
struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};


PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    
    float32_t4 textureColor = gTexture.Sample(sSampler, input.texcoord);
    
    
    output.color = gMaterial.color * textureColor;
    
    return output;
}