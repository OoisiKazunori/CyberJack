#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"

cbuffer RootConstants : register(b0)
{
    float radius;
    uint xMax;
    uint xyMax;
};

//BBの値
RWStructuredBuffer<BoundingBoxData> bbPosData : register(u0);
//当たり判定座標
RWStructuredBuffer<MeshHitBox> hitBoxData : register(u1);

//BoundingBox内に球の当たり判定の座標を均等に並べる
[numthreads(1, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = ThreadGroupIndex(groupId,xMax,xyMax);

    float diameter = radius * 2;
    //端から順に球を並べる
    float3 pos = (bbPosData[0].minPos + radius) + groupId * diameter;

    hitBoxData[index].pos = pos;
    hitBoxData[index].id = groupId;
}
