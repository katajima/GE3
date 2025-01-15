#include"Object3d.hlsli"
#include"Light.hlsli"


Texture2D<float4> gTexture : register(t0);
Texture2D<float4> g_Normalmap : register(t1); // t1レジスタにバインドされる法線マップデータ
Texture2D<float4> g_Specularmap : register(t2); // t2レジスタにバインドされるスペキュラーマップデータ
Texture2D<float4> g_aoMap : register(t3); // t3レジスタにバインドされるスペキュラーマップデータ
SamplerState sSampler : register(s0);

struct Camera
{
    float3 worldPosition;
    float3 normal;
};
ConstantBuffer<Camera> gCamera : register(b2);


////------PixelShader------////
struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
    
};

PixelShaderOutput main(PixelShaderInput input)
{
    PixelShaderOutput output;
    
    float4 transformedUV = mul(float4(input.texcoord.xy, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(sSampler, transformedUV.xy);
    
    
    float3 normal = input.normal;
    float3 tangent =float3(0, 0, 0);
    float3 biNormal = float3(0, 0, 0);
    if (gMaterial.useNormalMap)
    {
        tangent = normalize(mul((float3) input.worldPosition, (float3) input.tangent));
        biNormal = normalize(mul((float3) input.worldPosition, (float3) input.biNormal));
        normal = normalize(mul((float3) input.worldPosition, (float3) input.normal));
    }
    
    if (gMaterial.enableLighting != 0) // Lightingする場合
    {      
        // サンプリング
        float amdientPower = 0;
        if (gMaterial.useNormalMap)
        {
            float3 localNormal = g_Normalmap.Sample(sSampler, input.texcoord).xyz;
        // タンジェントスペース
            localNormal = (localNormal - 0.5f) * 2.0f;
            normal = input.tangent * localNormal.x + input.biNormal * localNormal.y + input.transformedNormal * localNormal.z;
        }
        
            
        float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        
        
        // 平行光原
        float3 directionalLig = { 0, 0, 0 }; // 環境光 
        // ポイントライトの処理
        float3 pointLig = { 0, 0, 0 };
 
        float3 allDire = DirectionalLightFunc(input, textureColor, toEye, normal);
        float3 allPoint = PointLightFunc(input, textureColor, toEye, normal);
        float3 allSpot = SpotLightFunc(input, textureColor, toEye, normal);
        
        // リグを使うか
        if (gMaterial.useLig != 0)
        {
          
            if (gDirectionalLight.enableLighting)
            {
                allDire *= directionalLig;
               
            }
        }
        
        
        
        output.color.rgb = allDire + allPoint + allSpot;
        
        
        output.color.a = gMaterial.color.a * textureColor.a;

        if (textureColor.a <= 0.5f)
        {
            discard;
        }
        if (output.color.a <= 0.0f)
        {
            discard;
        }
        
    }
    else // Lightingしない場合。前回までと同じ演算
    {
        output.color = gMaterial.color * textureColor;
    }
    
    
    
    output.color = pow(output.color, 2.2f);
    
    return output;
}