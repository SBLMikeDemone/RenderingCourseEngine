struct InDataVS 
{
	float4 position : Position;
	float3 color : Color;
};

struct OutDataVS 
{
	float4 viewPosition : SV_Position;
	float4 position : Position;
	float3 color : Color;
};

OutDataVS main(InDataVS input)
{
	OutDataVS output;
	output.viewPosition = input.position;
	output.position = input.position;
	output.color = input.color;
	return output;
}