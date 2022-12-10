#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/ExcuteIndirectHeader.hlsli"

cbuffer RootConstants : register(b0)
{
    matrix scaleRotateBillboardMat;
    matrix viewProjection;
    matrix motherMat;
};

struct HitBoxData
{
    float3 trianglePos[3];
    float3 normal;
};

//�O�p�`�̏��
RWStructuredBuffer<HitBoxData> hitBoxData : register(u0);

struct OutputData
{
    matrix mat;
    float4 color;
};
RWStructuredBuffer<OutputData> outputData : register(u1);

[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

	for(int trianglePosIndex = 0;trianglePosIndex < 3; ++trianglePosIndex)
    {
        //�s��v�Z-------------------------
        matrix pMatWorld = scaleRotateBillboardMat;
        pMatWorld[0][3] = hitBoxData[index].trianglePos[trianglePosIndex].x;
        pMatWorld[1][3] = hitBoxData[index].trianglePos[trianglePosIndex].y;
        pMatWorld[2][3] = hitBoxData[index].trianglePos[trianglePosIndex].z;
        //�s��v�Z-------------------------

        pMatWorld = mul(motherMat,pMatWorld);
        //�o��--------------------------------------------
        OutputData outputMat;
        outputMat.mat = mul(viewProjection,pMatWorld);
        outputMat.color = float4(1,1,1,1);
        outputData[index * 3 + trianglePosIndex] = outputMat;
        //�o��--------------------------------------------
    }
}