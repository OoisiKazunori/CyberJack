#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/ExcuteIndirectHeader.hlsli"

struct OutputData
{
    float4 pos;
};

struct InputData
{
    float4 pos;
    float2 uv;
};

//çXêV
//RWStructuredBuffer<InputData> vertciesData : register(u0);
//èoóÕ
//RWStructuredBuffer<OutputData> worldPosData : register(u1);

//cbuffer RootConstants : register(b0)
//{
//    float4 pos;
//    uint vertMaxNum;
//    uint bias;
//};

SamplerState g_samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};
Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

[numthreads(1, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1 * groupId.x;

    float2 uv = float2(0,0);
    float4 textureColor = tex.SampleLevel(g_samPoint,uv,0);
}