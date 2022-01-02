struct InDataVS 
{
	float4 position : Position;
};

float4 main(InDataVS input) : SV_Position
{
	return input.position;
}