struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : POSITION0;
     // 追加
    float3 tangent : TANGENT0; // 接ベクトル
    float3 biNormal : BINORMAL0; // 従ベクトル
    float3 transformedNormal : NORMAL1; // ピクセルシェーダで使用する法線
    
    // スキニング
    float4 weight : WEIGHT0;
    int4 index : INDEX0;
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
    
    // スキニング
    float4 weight : WEIGHT0;
    int4 index : INDEX0;
};



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
