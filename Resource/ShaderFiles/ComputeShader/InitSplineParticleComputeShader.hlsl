#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/ExcuteIndirectHeader.hlsli"

cbuffer RootConstants : register(b0)
{
    uint initMaxIndex;
};

struct OutputData
{
    uint startIndex;
    float rate;
	float rateVel;
    float2 offset;
    float4 color;
};

//o—Í
RWStructuredBuffer<OutputData> worldPosData : register(u0);

[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    //ƒ‰ƒ“ƒ_ƒ€‚Å‹Èü‚Ì”ÍˆÍ“à‚Å—”‚ÅÀ•W‚ğŒˆ‚ß‚é
    int startIndex = RandVec3(index,initMaxIndex,0).x;
    float rate = RandVec3(index,1,0).y;
    
	float2 offset;
	offset.x = cos(ConvertToRadian(RandVec3(index * 50,360,0).z)) * 50;
	offset.y = sin(ConvertToRadian(RandVec3(index * 50,360,0).z)) * 50;

    worldPosData[index].startIndex = startIndex;
	worldPosData[index].rate = rate;
	worldPosData[index].rateVel = RandVec3(index,0.030f,0.0f).x;
	worldPosData[index].offset = offset;
	worldPosData[index].color = float4(RandVec3(index * 50,1,0),1);
}
