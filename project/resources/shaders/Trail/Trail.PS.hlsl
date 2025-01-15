#include "Trail.hlsli"

//色など三角形の表面の材質を決定するものMaterial
struct Material
{
    
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
    float shininess;
    int useLig;
    int useHem;
    int useNormalMap;
    int useSpeculerMap;
};
ConstantBuffer<Material> gMaterial : register(b0);


Texture2D<float4> gTexture : register(t0);
SamplerState sSampler : register(s0);

////------PixelShader------////
struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};


PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output; 
    float4 transformedUV = mul(float4(input.texcoord.xy, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(sSampler, transformedUV.xy);
    
    
    output.color = gMaterial.color * textureColor;
   // output.color.a = gMaterial.color.a * textureColor.a;
    if (textureColor.a <= 0.5f)
    {
        discard;
    }
    if (output.color.a <= 0.0f)
    {
        discard;
    }
    
    
    return output;
}