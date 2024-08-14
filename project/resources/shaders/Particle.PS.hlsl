#include"Particle.hlsli"

//色など三角形の表面の材質を決定するものMaterial
struct Material {
    
    float32_t4 color;
    int32_t enableLighting;
    float32_t4x4 uvTransform;
};

struct DirectionalLight
{
    float32_t4 color; //!< ライトの色
    float32_t3 direction; //!< ライトの向き
    float intensity; //!< 輝度
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight: register(b1);
Texture2D<float32_t4> gTexture : register(t0);
SamplerState sSampler : register(s0);

////------PixelShader------////
struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};


PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;  
    float32_t4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(sSampler, transformedUV.xy);
    output.color = gMaterial.color * textureColor * input.color;
    if (output.color.a == 0.0f)
    {
        discard;
    }
    
    return output;
}