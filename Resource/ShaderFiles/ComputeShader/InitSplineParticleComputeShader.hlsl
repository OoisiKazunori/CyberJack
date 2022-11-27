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
    float2 offset;
    float4 color;
};

//更新
RWStructuredBuffer<float4> vertciesData : register(u0);
//出力
RWStructuredBuffer<OutputData> worldPosData : register(u1);

[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    //ランダムで曲線の範囲内で乱数で座標を決める
    int startIndex = RandVec3(index,initMaxIndex,0).x;
    float rate = RandVec3(index,1,0).y;
    
	float2 offset;
	offset.x = cos(ConvertToRadian(RandVec3(index * 50,360,0).z)) * 50;
	offset.y = sin(ConvertToRadian(RandVec3(index * 50,360,0).z)) * 50;

    worldPosData[index].startIndex = startIndex;
	worldPosData[index].rate = rate;
	worldPosData[index].offset = offset;
	worldPosData[index].color = float4(RandVec3(index * 50,1,0),1);
}
