struct InDataVS 
{
	float4 position : Position;
	float2 textureCoord : TEXCOORD;
};

struct OutDataVS 
{
	float4 viewPosition : SV_Position;
	float4 position : Position;
	float2 textureCoord : TEXCOORD;
};

OutDataVS main(InDataVS input)
{
	OutDataVS output;
	output.viewPosition = input.position;
	output.position = input.position;
	output.textureCoord = input.textureCoord;
	return output;
}