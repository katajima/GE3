
#include "Ocean.hlsli"



HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
    InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
    uint PatchID : SV_PrimitiveID)
{
    HS_CONSTANT_DATA_OUTPUT Output;

    // 各辺のテッセレーション係数を設定（ここでは例として15を設定）
    Output.EdgeTessFactor[0] = 1000000.0f; // 辺0
    Output.EdgeTessFactor[1] = 1000000.0f; // 辺1
    Output.EdgeTessFactor[2] = 1000000.0f; // 辺2
    Output.EdgeTessFactor[3] = 1000000.0f; // 辺3

    // 内部テッセレーション係数を設定（ここでは例として15を設定）
    Output.InsideTessFactor[0] = 1000000.0f; // 内部テッセレーション係数1
    Output.InsideTessFactor[1] = 1000000.0f; // 内部テッセレーション係数2

    return Output;
}

[domain("quad")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_CONTROL_POINT_OUTPUT main( 
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HS_CONTROL_POINT_OUTPUT Output;

	Output.vPosition = ip[i].vPosition;

	return Output;
}
