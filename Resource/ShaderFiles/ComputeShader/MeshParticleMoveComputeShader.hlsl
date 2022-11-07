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
    int indexMaxNum;
};

struct UpdateData
{
    float4 pos;
};

RWStructuredBuffer<UpdateData> worldPosArrayData : register(u0);
RWStructuredBuffer<OutputData> matrixData : register(u1);

[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = (groupThreadID.y * 1204) + groupThreadID.x + groupThreadID.z;
    index += 1024 * groupId.x;

    if(indexMaxNum < index)
    {
        return;
    }

    //行列計算-------------------------
    matrix pMatWorld = scaleRotateBillboardMat;
    pMatWorld[0][3] = worldPosArrayData[index].pos.x;
    pMatWorld[1][3] = worldPosArrayData[index].pos.y;
    pMatWorld[2][3] = worldPosArrayData[index].pos.z;
    //行列計算-------------------------

    //出力--------------------------------------------
    OutputData outputMat;
    outputMat.mat = mul(viewProjection,pMatWorld); 
    
    outputMat.color = float4(0.5,0,0,1);
    matrixData[index] = outputMat;
    //出力--------------------------------------------
}
