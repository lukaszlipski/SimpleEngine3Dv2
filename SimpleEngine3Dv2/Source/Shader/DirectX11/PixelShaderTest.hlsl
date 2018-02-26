
Texture2D TextureTest;
SamplerState SamplerTest;

cbuffer PerObject2
{
	float Float;
	float Float2;
};


struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	return float4(1 * Float,0,0,1);
}