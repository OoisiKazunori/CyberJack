#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"

//ì¸óÕ
RWStructuredBuffer<ParticleData> updateParticleData : register(u0);
RWStructuredBuffer<matrix> motherMatData : register(u1);

//èoóÕ
AppendStructuredBuffer<GPUParticleInput> inputGPUParticleData : register(u2);

cbuffer RootConstants : register(b0)
{    
    matrix scaleRotaBillBoardMat;
};

[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    if(1000000 < index)
    {
        return;
    }

    ParticleData particleData = updateParticleData[index];
    matrix worldMat = scaleRotaBillBoardMat;
    worldMat[0][3] = particleData.pos.x;
    worldMat[1][3] = particleData.pos.y;
    worldMat[2][3] = particleData.pos.z;
    worldMat = mul(motherMatData[particleData.id],worldMat);

    GPUParticleInput inputData;
    inputData.worldMat = worldMat;
    inputData.color = particleData.color;
    inputGPUParticleData.Append(inputData);
}
