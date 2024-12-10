#include"Object3d.hlsli"



//�F�ȂǎO�p�`�̕\�ʂ̍ގ������肷�����Material
struct Material
{
    
    float32_t4 color;
    int32_t enableLighting;
    float32_t4x4 uvTransform;
    float32_t shininess;
    int32_t useLig;
    int32_t useHem;
    int32_t useNormalMap;
    int32_t useSpeculerMap;
};
ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
Texture2D<float4> g_Normalmap : register(t1); // t1���W�X�^�Ƀo�C���h�����@���}�b�v�f�[�^
Texture2D<float4> g_Specularmap : register(t2); // t2���W�X�^�Ƀo�C���h�����X�y�L�����[�}�b�v�f�[�^
Texture2D<float4> g_aoMap : register(t3); // t3���W�X�^�Ƀo�C���h�����X�y�L�����[�}�b�v�f�[�^
SamplerState sSampler : register(s0);

static const int kMaxLight = 3;

struct Camera
{
    float32_t3 worldPosition;
    float32_t3 normal;
};
ConstantBuffer<Camera> gCamera : register(b2);


// ���s����
struct DirectionalLight
{
    float32_t4 color; //!< ���C�g�̐F
    float32_t3 direction; //!< ���C�g�̌���
    float32_t intensity; //!< �P�x
    float32_t ilg; // ���O
    int32_t enableLighting;
    float32_t3 groundColor; // �n�ʐF
    float32_t3 skyColor; // �V���F
    float32_t3 groundNormal; // �n�ʖ@������
};
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

// �|�C���g���C�g
struct PointLight
{
    float32_t4 color; //���C�g�F
    float32_t3 position; // ���C�g�ʒu
    float intensity; //�P�x
    float radius; // !< ���C�g�̓͂��ő勗��
    float32_t decay; //!< ������ 
    float32_t ilg; // ���O
    int32_t enableLighting;
    
};

// �����|�C���g���C�g
struct PointLights
{
    PointLight pointLights[kMaxLight];
};
ConstantBuffer<PointLights> gPointLight : register(b3);

// �X�|�b�g���C�g
struct SpotLight
{
    float32_t4 color; //���C�g�F
    float32_t3 position; // ���C�g�ʒu
    float32_t intensity; //�P�x
    float32_t3 direction; //!< ���C�g�̌���
    float32_t distance; //!< ���C�g�͂�����
    float32_t decay; //!< ������ 
    float32_t cosAngle; //!< �X�|�b�g���C�g�̗]��
    float32_t cosFalloffStart;
    int32_t enableLighting;
};





// �����|�C���g���C�g
struct SpotLights
{
    SpotLight spotLights[kMaxLight];
};
ConstantBuffer<SpotLights> gSpotLight : register(b4);




// �m�[�}���}�b�v



////------PixelShader------////
struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
    
};





PixelShaderOutput main(PixelShaderInput input)
{
    PixelShaderOutput output;
    
    float4 transformedUV = mul(float4(input.texcoord.xy, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(sSampler, transformedUV.xy);
    
    
    float3 normal = input.normal;
    float3 tangent =float3(0, 0, 0);
    float3 biNormal = float3(0, 0, 0);
    if (gMaterial.useNormalMap)
    {
        tangent = normalize(mul((float3) input.worldPosition, (float3) input.tangent));
        biNormal = normalize(mul((float3) input.worldPosition, (float3) input.biNormal));
        normal = normalize(mul((float3) input.worldPosition, (float3) input.normal));
    }
    
    if (gMaterial.enableLighting != 0) // Lighting����ꍇ
    {
       // �T���v�����O
        float amdientPower = 0;
        if (gMaterial.useNormalMap)
        {
            float3 localNormal = g_Normalmap.Sample(sSampler, input.texcoord).xyz;
        // �^���W�F���g�X�y�[�X
            localNormal = (localNormal - 0.5f) * 2.0f;
            normal = input.tangent * localNormal.x + input.biNormal * localNormal.y + input.transformedNormal * localNormal.z;
    
           
        }
        
            
        float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        
        
        // ���s����
        float32_t3 diffuseDirectionalLight = { 0, 0, 0 }; // �g�U����
        float32_t3 specularDirectionalLight = { 0, 0, 0 }; // ���ʔ���
        float32_t3 directionalLig = { 0, 0, 0 }; // ����
        if (gDirectionalLight.enableLighting)
        {
              // ���s�����̏���
              // �g�U����
            float32_t cos = Cos(gDirectionalLight.direction, toEye, normal);

              // ���ʔ���
            float32_t specularPow = SpecularPow2(gDirectionalLight.direction, toEye, normal, gMaterial.shininess);
        
            if (gMaterial.useSpeculerMap)
            {
                float specPower = g_Specularmap.Sample(sSampler, input.texcoord).r;
            
                specularPow *= specPower * 10.0f;
                
                amdientPower = g_aoMap.Sample(sSampler, input.texcoord).r;
            }
            



            
            
            // �������C�g
            float3 limColor = { 0, 0, 0 };
            //float power1 = 1.0f - max(0.0f, dot(gDirectionalLight.direction, input.normal));
            //float power2 = 1.0f - max(0.0f, input.normal.z * -1.0f);
            //float limPower = power1 * power2;
            //limPower = pow(limPower, 1.3f);
            
            //limColor = limPower * gDirectionalLight.color.rgb;
            
            
            
            //   // �������C�g
            float3 hemiLight = { 0, 0, 0 };
            //if (gMaterial.useHem != 0)
            //{
            //    float t = dot(normalize(input.normal), normalize(gDirectionalLight.groundNormal));
            //    t = (t + 1.0f) / 2.0f;
            //    hemiLight = lerp(gDirectionalLight.groundColor, gDirectionalLight.skyColor, t);
            //}

               // �g�U����
            diffuseDirectionalLight = gMaterial.color.rgb * textureColor.rgb * cos * gDirectionalLight.intensity;

           // diffuseDirectionalLight /= 3.1415926f;
            
               // ���ʔ���
            specularDirectionalLight = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f);

            
            
            
             // ����
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
        
        
        
        
        // �|�C���g���C�g�̏���
        //�g�U����
        float32_t3 diffusePointLight = { 0.0f, 0.0f, 0.0f };
        float32_t3 specularPointLight = { 0.0f, 0.0f, 0.0f };
        float3 pointLig = { 0, 0, 0 };
        for (int32_t point_i = 0; point_i < kMaxLight; point_i++)
        {
            if (gPointLight.pointLights[point_i].enableLighting == 0)
            {
                continue;
            }
            
            
            
            float32_t3 pointLightDirection = normalize(input.worldPosition - gPointLight.pointLights[point_i].position);
    
            
            float32_t cosP = Cos(pointLightDirection, toEye, normal);
            //���ʔ���
            float32_t specularPowP = SpecularPow2(pointLightDirection, toEye, normal, gMaterial.shininess);
        
            
            float32_t distanceP = length(gPointLight.pointLights[point_i].position - input.worldPosition);
            float32_t attenuationFactorP = saturate(1.0 - (distanceP / gPointLight.pointLights[point_i].radius));
 
            
            float32_t intensity = gPointLight.pointLights[point_i].intensity;
            if (gMaterial.useLig)
            {
                intensity *= gPointLight.pointLights[point_i].ilg;

            }
            
            
            diffusePointLight += gPointLight.pointLights[point_i].color.rgb * cosP * intensity * attenuationFactorP;
            specularPointLight += gPointLight.pointLights[point_i].color.rgb * gPointLight.pointLights[point_i].intensity * specularPowP * attenuationFactorP * float32_t3(1.0f, 1.0f, 1.0f);
            
            
        }
        
        diffusePointLight *= gMaterial.color.rgb * textureColor.rgb;
        
        ///�X�|�b�g���C�g
        // �X�|�b�g���C�g�̌v�Z
        
        float3 diffuseSpotLight = { 0.0f, 0.0f, 0.0f };
        float3 specularSpotLight = { 0.0f, 0.0f, 0.0f };
        for (uint32_t spot_i = 0; spot_i < kMaxLight; spot_i++)
        {
            if (gSpotLight.spotLights[spot_i].enableLighting == 0)
            {
                continue;
            }
            
            float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.spotLights[spot_i].position);
     
           
        
        
        // ���������̌v�Z
            float distanceS = length(gSpotLight.spotLights[spot_i].position - input.worldPosition);
            float attenuationFactorS = pow(saturate(-distanceS / gSpotLight.spotLights[spot_i].distance + 1.0), gSpotLight.spotLights[spot_i].decay);
        
        // �t�H�[���I�t�̌v�Z
            float cosAngle = dot(spotLightDirectionOnSurface, normalize(gSpotLight.spotLights[spot_i].direction));
            float falloffFactor = saturate((cosAngle - gSpotLight.spotLights[spot_i].cosAngle) / (gSpotLight.spotLights[spot_i].cosFalloffStart - gSpotLight.spotLights[spot_i].cosAngle));
       
            
            
            float cosS = Cos(spotLightDirectionOnSurface, toEye, normal);
            float specularPowS = SpecularPow2(spotLightDirectionOnSurface, toEye, normal, gMaterial.shininess);
            
             // �X�|�b�g���C�g�̊g�U���˂Ƌ��ʔ��˂̌v�Z
            diffuseSpotLight += gSpotLight.spotLights[spot_i].color.rgb * cosS * gSpotLight.spotLights[spot_i].intensity * attenuationFactorS * falloffFactor;
            specularSpotLight += gSpotLight.spotLights[spot_i].color.rgb * gSpotLight.spotLights[spot_i].intensity * specularPowS * attenuationFactorS * falloffFactor;
        }
        
        diffuseSpotLight *= gMaterial.color.rgb * textureColor.rgb;
        
       
        
        
         // ���O���g����
        float3 allDire = (diffuseDirectionalLight + specularDirectionalLight);
        float3 allPoint = (diffusePointLight + specularPointLight);
        float3 allSpot = (diffuseSpotLight + specularSpotLight);
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
    else // Lighting���Ȃ��ꍇ�B�O��܂łƓ������Z
    {
        output.color = gMaterial.color * textureColor;
    }
    
    
    
    output.color = pow(output.color, 2.2f);
    
    return output;
}