#include"Object2D.hlsli"

//色など三角形の表面の材質を決定するものMaterial
struct Material
{ 
    float4 color;
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState sSampler : register(s0);

////------PixelShader------////
struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};


// バイキュービック補間関数
float CubicHermite(float A, float B, float C, float D, float t)
{
    float a = (-A + 3 * B - 3 * C + D) / 2.0;
    float b = (2 * A - 5 * B + 4 * C - D) / 2.0;
    float c = (-A + C) / 2.0;
    float d = B;
    return a * t * t * t + b * t * t + c * t + d;
}

// 2Dテクスチャのバイキュービックサンプリング
float4 SampleBicubic(Texture2D tex, SamplerState samp, float2 uv, float2 texSize)
{
    float2 texCoord = uv * texSize - 0.5;
    float2 f = frac(texCoord);
    texCoord = floor(texCoord);

    float4 color[4];
    for (int i = -1; i <= 2; i++)
    {
        color[i + 1] = float4(
            CubicHermite(
                tex.SampleLevel(samp, (texCoord + float2(-1, i)) / texSize, 0).r,
                tex.SampleLevel(samp, (texCoord + float2(0, i)) / texSize, 0).r,
                tex.SampleLevel(samp, (texCoord + float2(1, i)) / texSize, 0).r,
                tex.SampleLevel(samp, (texCoord + float2(2, i)) / texSize, 0).r,
                f.x),
            CubicHermite(
                tex.SampleLevel(samp, (texCoord + float2(-1, i)) / texSize, 0).g,
                tex.SampleLevel(samp, (texCoord + float2(0, i)) / texSize, 0).g,
                tex.SampleLevel(samp, (texCoord + float2(1, i)) / texSize, 0).g,
                tex.SampleLevel(samp, (texCoord + float2(2, i)) / texSize, 0).g,
                f.x),
            CubicHermite(
                tex.SampleLevel(samp, (texCoord + float2(-1, i)) / texSize, 0).b,
                tex.SampleLevel(samp, (texCoord + float2(0, i)) / texSize, 0).b,
                tex.SampleLevel(samp, (texCoord + float2(1, i)) / texSize, 0).b,
                tex.SampleLevel(samp, (texCoord + float2(2, i)) / texSize, 0).b,
                f.x),
            CubicHermite(
                tex.SampleLevel(samp, (texCoord + float2(-1, i)) / texSize, 0).a,
                tex.SampleLevel(samp, (texCoord + float2(0, i)) / texSize, 0).a,
                tex.SampleLevel(samp, (texCoord + float2(1, i)) / texSize, 0).a,
                tex.SampleLevel(samp, (texCoord + float2(2, i)) / texSize, 0).a,
                f.x)
        );
    }

    return float4(
        CubicHermite(color[0].r, color[1].r, color[2].r, color[3].r, f.y),
        CubicHermite(color[0].g, color[1].g, color[2].g, color[3].g, f.y),
        CubicHermite(color[0].b, color[1].b, color[2].b, color[3].b, f.y),
        CubicHermite(color[0].a, color[1].a, color[2].a, color[3].a, f.y)
    );
}

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    
    float4 textureColor = gTexture.Sample(sSampler, input.texcoord);
    
    
    output.color = gMaterial.color * textureColor;
    
    return output;
}