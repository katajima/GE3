#include"Ocean.hlsli"





VS_CONTROL_POINT_OUTPUT main(VS_INPUT input)
{
    VS_CONTROL_POINT_OUTPUT output;

    // ƒ[ƒ‹ƒhÀ•WŒn‚É•ÏŠ·
    float4 worldPosition = mul(float4(input.position, 1.0), gTransformationMatrix.World);
    output.vPosition = worldPosition.xyz;

    return output;
}
