
Texture2D TextureTest;
SamplerState SamplerTest;


struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	return float4(1,1,1,1);
}