static const int kMaxLight = 3;

// 平行光線
struct DirectionalLight
{
    float4 color; //!< ライトの色
    float3 direction; //!< ライトの向き
    float intensity; //!< 輝度
    float ilg; // リグ
    int enableLighting;
    float3 groundColor; // 地面色
    float3 skyColor; // 天球色
    float3 groundNormal; // 地面法線方向
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
    float ilg; // リグ
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


struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : POSITION0;
    
    float3 tangent : TANGENT0;
    float3 biNormal : BINORMAL0;
    float3 transformedNormal : NORMAL1; // 頂点シェーダから渡された法線 
};


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

float Cos(float3 dire, float3 toEye, float3 normal)
{
    float3 halfVectorv = normalize((-dire) + toEye);
    float NdotL = dot(normalize(normal), halfVectorv);

    return pow(NdotL * 0.5f + 0.5f, 2.0f);
}

float SpecularPow(float3 dire, float3 toEye, float3 normal, float shininess)
{
    float3 reflectLight = reflect(dire, normalize(normal));
    float RdotE = dot(reflectLight, toEye);
   
    return pow(saturate(RdotE), shininess);
}

float SpecularPow2(float3 dire, float3 toEye, float3 normal, float shininess)
{
    float3 reflectLight = reflect(dire, normalize(normal)); // 反射ベクトル
    float RdotE = dot(reflectLight, toEye); // 視線ベクトルと反射ベクトルのドット積

    return pow(saturate(RdotE), shininess);
}

float3 DirectionalLightFunc(PixelShaderInput input,float4 textureColor, float3 toEye, float3 normal)
{
    float3 diffuse = { 0, 0, 0 };
    float3 specular = { 0, 0, 0 };
        
    if (gDirectionalLight.enableLighting)
    {
       
              // 平行光源の処理
              // 拡散反射
        float cos = Cos(gDirectionalLight.direction, toEye, normal);

            
              // 鏡面反射
        float specularPow = 0.0f;
        if (gMaterial.shininess >= 1.0f)
        {
            specularPow = SpecularPow(gDirectionalLight.direction, toEye, normal, gMaterial.shininess);
        }
            
        if (gMaterial.useSpeculerMap)
        {
              //  float specPower = g_Specularmap.Sample(sSampler, input.texcoord).r;
            
              //  specularPow *= specPower * 10.0f;
                
              //  amdientPower = g_aoMap.Sample(sSampler, input.texcoord).r;
        }
                  
            
            
            
            
            // リムライト
        float3 limColor = { 0, 0, 0 };
            //float power1 = 1.0f - max(0.0f, dot(gDirectionalLight.direction, input.normal));
            //float power2 = 1.0f - max(0.0f, input.normal.z * -1.0f);
            //float limPower = power1 * power2;
            //limPower = pow(limPower, 1.3f);
            
            //limColor = limPower * gDirectionalLight.color.rgb;
            
            
            
            //   // 半球ライト
        float3 hemiLight = { 0, 0, 0 };
            //if (gMaterial.useHem != 0)
            //{
            //    float t = dot(normalize(input.normal), normalize(gDirectionalLight.groundNormal));
            //    t = (t + 1.0f) / 2.0f;
            //    hemiLight = lerp(gDirectionalLight.groundColor, gDirectionalLight.skyColor, t);
            //}

               // 拡散反射
        diffuse = gMaterial.color.rgb * textureColor.rgb * cos * gDirectionalLight.intensity;

            //diffuseDirectionalLight /= 3.1415926f;
            
               // 鏡面反射
        specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f);

            
            
            
             // 環境光
            //directionalLig = diffuseDirectionalLight + specularDirectionalLight + limColor;
            
        if (gMaterial.useNormalMap)
        {
               // directionalLig *= float3(amdientPower, amdientPower, amdientPower);
        }
            //directionalLig.x += gDirectionalLight.ilg;
            //directionalLig.y += gDirectionalLight.ilg;
            //directionalLig.z += gDirectionalLight.ilg;
    
            //directionalLig += hemiLight;
    }
    
    return diffuse + specular;
};

float3 PointLightFunc(PixelShaderInput input, float4 textureColor, float3 toEye, float3 normal)
{
    float3 diffusePointLight = { 0.0f, 0.0f, 0.0f };
    float3 specularPointLight = { 0.0f, 0.0f, 0.0f };       
    for (int point_i = 0; point_i < kMaxLight; point_i++)
    {
        if (gPointLight.pointLights[point_i].enableLighting == 0)
        {
            continue;
        }
            
            
            
        float3 pointLightDirection = normalize(input.worldPosition - gPointLight.pointLights[point_i].position);
    
            
        float cosP = Cos(pointLightDirection, toEye, normal);
            //鏡面反射
        float specularPowP = SpecularPow2(pointLightDirection, toEye, normal, gMaterial.shininess);
        
            
        float distanceP = length(gPointLight.pointLights[point_i].position - input.worldPosition);
        float attenuationFactorP = saturate(1.0 - (distanceP / gPointLight.pointLights[point_i].radius));
 
            
        float intensity = gPointLight.pointLights[point_i].intensity;
        if (gMaterial.useLig)
        {
            intensity *= gPointLight.pointLights[point_i].ilg;

        }
            
            
        diffusePointLight += gPointLight.pointLights[point_i].color.rgb * cosP * intensity * attenuationFactorP;
        specularPointLight += gPointLight.pointLights[point_i].color.rgb * gPointLight.pointLights[point_i].intensity * specularPowP * attenuationFactorP * float3(1.0f, 1.0f, 1.0f);
            
            
    }
        
    diffusePointLight *= gMaterial.color.rgb * textureColor.rgb;
    
    
    return (diffusePointLight + specularPointLight);
}

float3 SpotLightFunc(PixelShaderInput input, float4 textureColor, float3 toEye, float3 normal)
{
    float3 diffuseSpotLight = { 0.0f, 0.0f, 0.0f };
    float3 specularSpotLight = { 0.0f, 0.0f, 0.0f };
    for (uint32_t spot_i = 0; spot_i < kMaxLight; spot_i++)
    {
        if (gSpotLight.spotLights[spot_i].enableLighting == 0)
        {
            continue;
        }
            
        float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.spotLights[spot_i].position);
     
           
        
        
        // 距離減衰の計算
        float distanceS = length(gSpotLight.spotLights[spot_i].position - input.worldPosition);
        float attenuationFactorS = pow(saturate(-distanceS / gSpotLight.spotLights[spot_i].distance + 1.0), gSpotLight.spotLights[spot_i].decay);
        
        // フォールオフの計算
        float cosAngle = dot(spotLightDirectionOnSurface, normalize(gSpotLight.spotLights[spot_i].direction));
        float falloffFactor = saturate((cosAngle - gSpotLight.spotLights[spot_i].cosAngle) / (gSpotLight.spotLights[spot_i].cosFalloffStart - gSpotLight.spotLights[spot_i].cosAngle));
       
            
            
        float cosS = Cos(spotLightDirectionOnSurface, toEye, normal);
        float specularPowS = SpecularPow2(spotLightDirectionOnSurface, toEye, normal, gMaterial.shininess);
            
             // スポットライトの拡散反射と鏡面反射の計算
        diffuseSpotLight += gSpotLight.spotLights[spot_i].color.rgb * cosS * gSpotLight.spotLights[spot_i].intensity * attenuationFactorS * falloffFactor;
        specularSpotLight += gSpotLight.spotLights[spot_i].color.rgb * gSpotLight.spotLights[spot_i].intensity * specularPowS * attenuationFactorS * falloffFactor;
    }
        
    diffuseSpotLight *= gMaterial.color.rgb * textureColor.rgb;
    
    
    return (diffuseSpotLight + specularSpotLight);
}