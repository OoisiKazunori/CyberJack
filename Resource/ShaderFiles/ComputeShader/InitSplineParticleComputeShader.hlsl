#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/ExcuteIndirectHeader.hlsli"

cbuffer RootConstants : register(b0)
{
    float4 limitPos[10];
    uint limitIndexMaxNum;
    uint initMaxIndex;
};

struct OutputData
{
    float4 pos;
};

//スプライン曲線
float3 SplinePosition(float4 LIMIT_INDEX_ARRAY[10],int START_INDEX,float RATE,int INDEX_MAX,bool LOOP_FLAG)
{
    if (START_INDEX < 1)
	{
		return LIMIT_INDEX_ARRAY[1].xyz;
	}
	float3 p0 = LIMIT_INDEX_ARRAY[START_INDEX - 1].xyz;
	float3 p1 = LIMIT_INDEX_ARRAY[START_INDEX].xyz;
	float3 p2;
	float3 p3;

	int subIndex = 3;
	if (LOOP_FLAG == true)
	{
		if (START_INDEX > INDEX_MAX - subIndex)
		{
			p2 = LIMIT_INDEX_ARRAY[1].xyz;
			p3 = LIMIT_INDEX_ARRAY[2].xyz;
		}
		else
		{
			p2 = LIMIT_INDEX_ARRAY[INDEX_MAX + 1].xyz;
			p3 = LIMIT_INDEX_ARRAY[INDEX_MAX + 2].xyz;
		}
	}
	else
	{
		if (START_INDEX > INDEX_MAX - 3)
        {
            return LIMIT_INDEX_ARRAY[INDEX_MAX - 3].xyz;
        }
		p2 = LIMIT_INDEX_ARRAY[START_INDEX + 1].xyz;
		p3 = LIMIT_INDEX_ARRAY[START_INDEX + 2].xyz;
	}

    float3 resultPos = 0.5 * ((2 * p1  + (-p0 + p2) * RATE) + (2 * p0 - 5 * p1 + 4 * p2 - p3) * (RATE * RATE) + (-p0 + 3 * p1 - 3 * p2 + p3) * (RATE * RATE * RATE));
    return resultPos;
}

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
    
    //スプライン曲線上の位置を見る
    float3 basePos = SplinePosition(limitPos,startIndex,rate,limitIndexMaxNum,false);

	float3 offset;
	offset.x = cos(ConvertToRadian(RandVec3(index * 50,360,0).z)) * 50;
	offset.y = sin(ConvertToRadian(RandVec3(index * 50,360,0).z)) * 50;
	//offset.z = cos(ConvertToRadian(RandVec3(index * 50,360,0).z)) * RandVec3(index * 10,50,-50);

    //スプライン曲線に沿った位置からオフセットを足した値がパーティクルの位置
    float3 particlePos = basePos + offset;

    worldPosData[index].pos.xyz = particlePos;
}
