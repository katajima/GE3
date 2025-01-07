#include"Particle.hlsli"



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
    //float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    //float4 textureColor = gTexture.Sample(sSampler, transformedUV.xy);
    float4 textureColor = gTexture.Sample(sSampler, input.texcoord);
    output.color = /*gMaterial.color * */textureColor * input.color;
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