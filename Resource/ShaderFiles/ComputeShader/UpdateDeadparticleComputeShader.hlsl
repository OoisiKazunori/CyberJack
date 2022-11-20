#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/ExcuteIndirectHeader.hlsli"

struct InitData
{
    float4 pos;
    float4 color;
    float4 vel;
};

struct MatData
{
    matrix mat;
    float4 color;
};

cbuffer RootConstants : register(b0)
{    
    matrix scaleRotateBillboardMat;
    matrix viewProjection;
    matrix motherMat;
};

//更新
RWStructuredBuffer<InitData> particleData : register(u0);
RWStructuredBuffer<MatData> outputParticleData : register(u1);

[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    bool zeroParticeFlag = particleData[index].pos.x == 0.0f && particleData[index].pos.y== 0.0f && particleData[index].pos.z == 0.0f;

    if(particleData[index].color.a <= 0.0f || zeroParticeFlag)
    {
        return;
    }
    particleData[index].pos += particleData[index].vel;
    particleData[index].color.a  += -0.01f;
    
    //行列計算-------------------------
    matrix pMatWorld = scaleRotateBillboardMat;
    pMatWorld[0][3] = particleData[index].pos.x;
    pMatWorld[1][3] = particleData[index].pos.y;
    pMatWorld[2][3] = particleData[index].pos.z;
    
    pMatWorld = mul(motherMat,pMatWorld);

    //行列計算-------------------------    
    MatData outputMat;
    outputMat.mat = mul(viewProjection,pMatWorld);
    outputMat.color = particleData[index].color;
    outputParticleData[index] = outputMat;
}