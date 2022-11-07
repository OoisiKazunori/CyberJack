#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/ExcuteIndirectHeader.hlsli"

struct OutputData
{
    matrix mat;
    float4 color;
};

cbuffer RootConstants : register(b0)
{    
    matrix scaleRotateBillboardMat;
    matrix viewProjection;
    int indexMax;
};


RWStructuredBuffer<float4> worldPosArrayData : register(u0);
RWStructuredBuffer<OutputData> matrixData : register(u1);

[numthreads(8, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = (groupThreadID.y * 1204) + groupThreadID.x + groupThreadID.z;
    index += 1024 * groupId.x;

    if(8 < index)
    {
        return;
    }

    //�s��v�Z-------------------------
    matrix pMatWorld = scaleRotateBillboardMat;
    pMatWorld[0][3] = worldPosArrayData[index].x;
    pMatWorld[1][3] = worldPosArrayData[index].y;
    pMatWorld[2][3] = worldPosArrayData[index].z;
    //�s��v�Z-------------------------

    //�o��--------------------------------------------
    OutputData outputMat;
    outputMat.mat = mul(viewProjection,pMatWorld);
    outputMat.color = float4(0.5,0,0,1);
    matrixData[index] = outputMat;
    //�o��--------------------------------------------
}
