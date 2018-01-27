
cbuffer PerObject
{
	float3x3 Test;
	float4x4 WVP;
	float4 Color;
	float Float;
	int Integer;
};

struct VS_INPUT
{
	float3 Position : POSITION;
	float2 TexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	output.Position = float4(input.Position + Float, 1);
	output.TexCoord = input.TexCoord;
	return output;
}