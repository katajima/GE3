#include"Object3d.hlsli"

//色など三角形の表面の材質を決定するものMaterial
struct Material
{
    
    float4 color;
    int enableLighting;
    float32_t4x4 uvTransform;
    float shininess;
};
ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState sSampler : register(s0);


struct Camera
{
    float3 worldPosition;
};
ConstantBuffer<Camera> gCamera : register(b2);

static const int kMaxLight = 64;

// 平行光線
struct DirectionalLight
{
    float4 color; //!< ライトの色
    float3 direction; //!< ライトの向き
    float intensity; //!< 輝度
};
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

// ポイントライト
struct PointLight
{
    float4 color; //ライト色
    float3 position; // ライト位置
    float intensity; //輝度
    float radius; // !< ライトの届く最大距離
    float decay; //!< 減衰率 
    int enableLighting;
};

// 複数ポイントライト
struct PointLights
{
    PointLight pointLights[kMaxLight];
};
ConstantBuffer<PointLights> gPointLight : register(b3);

// スポットライト
struct SpotLight
{
    float4 color; //ライト色
    float3 position; // ライト位置
    float intensity; //輝度
    float3 direction; //!< ライトの向き
    float distance; //!< ライト届く距離
    float decay; //!< 減衰率 
    float cosAngle; //!< スポットライトの余弦
    float cosFalloffStart;
    int enableLighting;
};





// 複数ポイントライト
struct SpotLights
{
    SpotLight spotLights[kMaxLight];
};
ConstantBuffer<SpotLights> gSpotLight : register(b4);




////------PixelShader------////
struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

float Cos(float3 dire, float3 toEye, float3 normal)
{
    float3 halfVectorv = normalize((-dire) + toEye);
    float NdotL = dot(normalize(normal), halfVectorv);
    
    return pow(NdotL * 0.5f + 0.5f, 2.0f);
}

float SpecularPow(float3 dire, float3 toEye, float3 normal)
{
    float3 reflectLight = reflect(dire, normalize(normal));
    float RdotE = dot(reflectLight, toEye);
    return pow(saturate(RdotE), gMaterial.shininess);
}

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(sSampler, transformedUV.xy);
    
       
    
    if (gMaterial.enableLighting != 0) // Lightingする場合
    {
        float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        
       
        // 平行光源の処理
        //拡散反射
        float cos = Cos(gDirectionalLight.direction, toEye, input.normal);
        
        //鏡面反射
        float specularPow = SpecularPow(gDirectionalLight.direction, toEye, input.normal);
        
        //ライト
        float3 diffuseDirectionalLight = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        float3 specularDirectionalLight = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f);
       
        
        
        // ポイントライトの処理
        //拡散反射
        float3 diffusePointLight = { 0.0f, 0.0f, 0.0f };
        float3 specularPointLight = { 0.0f, 0.0f, 0.0f };
        for (uint32_t point_i = 0; point_i < kMaxLight; point_i++)
        {
            if (gPointLight.pointLights[point_i].enableLighting == 0)
            {
               continue;
            }
            
            float3 pointLightDirection = normalize(input.worldPosition - gPointLight.pointLights[point_i].position);
    
            
            float cosP = Cos(pointLightDirection, toEye, input.normal);
            //鏡面反射
            float specularPowP = SpecularPow(pointLightDirection, toEye, input.normal);
        
            
            float distanceP = length(gPointLight.pointLights[point_i].position - input.worldPosition);
            float attenuationFactorP = saturate(1.0 - (distanceP / gPointLight.pointLights[point_i].radius));
 
            diffusePointLight += gPointLight.pointLights[point_i].color.rgb * cosP * gPointLight.pointLights[point_i].intensity * attenuationFactorP;
            specularPointLight += gPointLight.pointLights[point_i].color.rgb * gPointLight.pointLights[point_i].intensity * specularPowP * attenuationFactorP * float3(1.0f, 1.0f, 1.0f);
        }
        diffusePointLight *= gMaterial.color.rgb * textureColor.rgb;
        
        ///スポットライト
        // スポットライトの計算
        
        float3 diffuseSpotLight = { 0.0f, 0.0f, 0.0f };
        float3 specularSpotLight = { 0.0f, 0.0f, 0.0f };
        for (uint32_t spot_i = 0; spot_i < kMaxLight; spot_i++)
        {
            if (gSpotLight.spotLights[spot_i].enableLighting != 0)
            {
             //   continue;
            }
            
            float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.spotLights[spot_i].position);
     
           
        
        
        // 距離減衰の計算
            float distanceS = length(gSpotLight.spotLights[spot_i].position - input.worldPosition);
            float attenuationFactorS = pow(saturate(-distanceS / gSpotLight.spotLights[spot_i].distance + 1.0), gSpotLight.spotLights[spot_i].decay);
        
        // フォールオフの計算
            float cosAngle = dot(spotLightDirectionOnSurface, normalize(gSpotLight.spotLights[spot_i].direction));
            float falloffFactor = saturate((cosAngle - gSpotLight.spotLights[spot_i].cosAngle) / (gSpotLight.spotLights[spot_i].cosFalloffStart - gSpotLight.spotLights[spot_i].cosAngle));
       
            
            
            float cosS = Cos(spotLightDirectionOnSurface, toEye, input.normal);
            //float specularPowS = SpecularPow(spotLightDirectionOnSurface, toEye, input.normal);
            
             // スポットライトの拡散反射と鏡面反射の計算
            //diffuseSpotLight += gSpotLight.spotLights[spot_i].color.rgb * cosS * gSpotLight.spotLights[spot_i].intensity * attenuationFactorS * falloffFactor;
            //specularSpotLight += gSpotLight.spotLights[spot_i].color.rgb * gSpotLight.spotLights[spot_i].intensity * specularPowS * attenuationFactorS * falloffFactor;
        }
        //diffuseSpotLight *= gMaterial.color.rgb * textureColor.rgb;
        //specularSpotLight *= gMaterial.color.rgb * textureColor.rgb;
        
       
        output.color.rgb =
        diffuseDirectionalLight + specularDirectionalLight +
        diffusePointLight + specularPointLight +
        diffuseSpotLight + specularSpotLight;
        
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