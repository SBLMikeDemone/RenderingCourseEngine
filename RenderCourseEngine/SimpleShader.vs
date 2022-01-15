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
    float4x4 objectToView;
    float4x4 normalToView;
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
    float4x4 worldToView;
    float3 eyePosition;
};

struct InDataVS 
{
	float3 position : Position;
	float2 textureCoord : TEXCOORD;
	float3 normal : Normal;
};

struct OutDataVS 
{
	float4 viewPosition : SV_Position;
	float2 textureCoord : TEXCOORD;
	float3 viewNormal : Normal;
	float3 worldPosition : Position1;
	float3 worldNormal : Normal1;
};

OutDataVS main(InDataVS inData)
{
    OutDataVS outData;
    outData.viewPosition = mul(transform.objectToView, float4(inData.position, 1.0));
    outData.viewNormal = normalize(mul(transform.normalToView, float4(inData.normal, 0.f)).xyz);
    outData.textureCoord = inData.textureCoord.xy;
    outData.worldPosition = inData.position;
    outData.worldNormal = inData.normal;

    return outData;
}