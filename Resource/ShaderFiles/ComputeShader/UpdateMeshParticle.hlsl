#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/ExcuteIndirectHeader.hlsli"

struct OutputData
{
    matrix mat;    
    float4 color;
    matrix motherMat;
};

cbuffer RootConstants : register(b0)
{    
    matrix scaleRotateBillboardMat;
    matrix viewProjection;
    uint motherMatMax;
};

struct ParticleData
{
    float3 pos;
    float4 color;
    uint id;
};

//ì¸óÕ
RWStructuredBuffer<ParticleData> particleInitData : register(u0);
RWStructuredBuffer<matrix> inputMotherData : register(u1);

//èoóÕ
RWStructuredBuffer<float3> posData : register(u2);
RWStructuredBuffer<float4> colorData : register(u3);
RWStructuredBuffer<matrix> outputMotherMat : register(u4);

[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = ThreadGroupIndex(groupId,groupIndex,groupThreadID,1024);

    ParticleData particleData = particleInitData[index];

    posData[index] = particleData.pos;
    colorData[index] = particleData.color;
    outputMotherMat[index] = inputMotherData[particleData.id];
}
