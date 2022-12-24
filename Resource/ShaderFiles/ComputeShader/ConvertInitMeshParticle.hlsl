#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/ExcuteIndirectHeader.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"

//ì¸óÕ
ConsumeStructuredBuffer<ParticleData>particleInitData : register(u0);
//èoóÕ
RWStructuredBuffer<ParticleData> updateParticleData : register(u1);

[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = ThreadGroupIndex(groupId,groupIndex,groupThreadID,1024);
    updateParticleData[index] = particleInitData.Consume();
}
