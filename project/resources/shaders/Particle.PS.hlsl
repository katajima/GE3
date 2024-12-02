#include"Particle.hlsli"

//色など三角形の表面の材質を決定するものMaterial
struct Material
{
    
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
};

struct DirectionalLight
{
    float4 color; //!< ライトの色
    float3 direction; //!< ライトの向き
    float intensity; //!< 輝度
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
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
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(sSampler, transformedUV.xy);
    output.color = gMaterial.color * textureColor * input.color;
    if (output.color.a == 0.0f)
    {
        discard;
    }
    
    //output.color = textureColor;
    //output.color.rg = transformedUV.xy;
    //output.color.b = 0.0f;
   // output.color.a = 1.0f;
    
    return output;
}