struct DirLight
{
    float3 color;
    float pad0;
    float3 direction;
    float pad1;
};

struct PointLight
{
    float3 color;
    float falloff;
    float3 position;
    float pad0;
};

struct Surface
{
	float3 albedo;
	float roughness;
	float3 specularF0; // characteristic spec color
	float pad0;
};

struct Transform
{
    float4x4 objectToClip;
    float4x4 objectToView;
    float4x4 normalToView;
    float4x4 worldToView;
};

cbuffer cbObjectData : register(b0)
{
    Surface surface;
    Transform transform;
};

cbuffer cbViewData : register(b1)
{
    DirLight dirLight;
    PointLight pointLight[6];
    uint timeValue;
    uint frameNum;
    uint resolutionX;
    uint resolutionY;
};

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
	output.viewPosition = mul(transform.worldToView, input.position);
	output.position = input.position;
	output.textureCoord = input.textureCoord;
	return output;
}