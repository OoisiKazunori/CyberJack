#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"

//頂点座標
RWStructuredBuffer<float4> vertciesData : register(u0);
//出力
RWStructuredBuffer<BoundingBoxData> bbPosData : register(u1);

cbuffer RootConstants : register(b2)
{
    matrix scaleRotaMat;
};

//モデルの形に沿ったBoundingBoxを作る為ににXYZそれぞれの軸の最大値、最小値を計算する
[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = ThreadGroupIndex(groupId,groupIndex,groupThreadID,1024);

    //最小値計算
    bbPosData[0].minPos = MinFloat3(bbPosData[0].minPos, vertciesData[index].xyz);
    //最大値計算
    bbPosData[0].maxPos = MaxFloat3(bbPosData[0].maxPos, vertciesData[index].xyz);
}
