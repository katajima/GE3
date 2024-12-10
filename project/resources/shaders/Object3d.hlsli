
struct VertexShaderOutput
{
    float32_t4 position : SV_POSITION;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
    float32_t3 worldPosition : POSITION0;
     // 追加
    float3 tangent : TANGENT0; // 接ベクトル
    float3 biNormal : BINORMAL0; // 従ベクトル
    float3 transformedNormal : NORMAL1; // ピクセルシェーダで使用する法線
};

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
    float32_t3 reflectLight = reflect(normalize(dire), normalize(normal)); // 反射ベクトル
    float32_t RdotE = dot(reflectLight, normalize(toEye)); // 視線ベクトルと反射ベクトルのドット積

    return pow(saturate(RdotE), shininess);
}


