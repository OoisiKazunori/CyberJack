#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"

//入力
RWStructuredBuffer<ParticleData> updateParticleData : register(u0);
RWStructuredBuffer<matrix> motherMatData : register(u1);

//構造体でなければシェーダーコンパイル時にメモリアクセス違反が吐かれる。先生と原因相談中。
struct OutputMatData
{
    matrix worldMat;
};
struct OutputColorData
{
    float4 color;
};
//出力
AppendStructuredBuffer<OutputMatData> worldMatData : register(u2);
AppendStructuredBuffer<OutputColorData> colorData : register(u3);

cbuffer RootConstants : register(b0)
{    
    matrix scaleRotaBillBoardMat;
};

[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = ThreadGroupIndex(groupId,groupIndex,groupThreadID,1024);

    ParticleData particleData = updateParticleData[index];
    matrix worldMat = scaleRotaBillBoardMat;
    worldMat[3][0] = particleData.pos.x;
    worldMat[3][1] = particleData.pos.y;
    worldMat[3][2] = particleData.pos.z;
    worldMat = mul(motherMatData[particleData.id],worldMat);

    //OutputMatData outputMatData;
    //outputMatData.worldMat = worldMat; 
    //worldMatData.Append(outputMatData);

    //OutputColorData outputColorData;
    //outputColorData.color = particleData.color; 
    //colorData.Append(outputColorData);
}
