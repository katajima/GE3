
struct VertexShaderOutput
{
    float32_t4 position : SV_POSITION;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
    float32_t3 worldPosition : POSITION0;
     // �ǉ�
    float3 tangent : TANGENT0; // �ڃx�N�g��
    float3 biNormal : BINORMAL0; // �]�x�N�g��
    float3 transformedNormal : NORMAL1; // �s�N�Z���V�F�[�_�Ŏg�p����@��
};

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : POSITION0;
    
    float3 tangent : TANGENT0;
    float3 biNormal : BINORMAL0;
    float3 transformedNormal : NORMAL1; // ���_�V�F�[�_����n���ꂽ�@�� 
};

float Cos(float32_t3 dire, float32_t3 toEye, float32_t3 normal)
{
    float32_t3 halfVectorv = normalize((-dire) + toEye);
    float32_t NdotL = dot(normalize(normal), halfVectorv);

    return pow(NdotL * 0.5f + 0.5f, 2.0f);
}

float SpecularPow(float32_t3 dire, float32_t3 toEye, float32_t3 normal, float32_t shininess)
{
    float32_t3 reflectLight = reflect(dire, normalize(normal));
    float32_t RdotE = dot(reflectLight, toEye);
   
    return pow(saturate(RdotE), shininess);
}

float SpecularPow2(float32_t3 dire, float32_t3 toEye, float32_t3 normal, float32_t shininess)
{
    float32_t3 reflectLight = reflect(dire, normalize(normal)); // ���˃x�N�g��
    float32_t RdotE = dot(reflectLight, toEye); // �����x�N�g���Ɣ��˃x�N�g���̃h�b�g��

    return pow(saturate(RdotE), shininess);
}


