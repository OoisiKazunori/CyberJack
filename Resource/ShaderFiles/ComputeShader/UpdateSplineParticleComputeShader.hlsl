#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/ExcuteIndirectHeader.hlsli"

cbuffer RootConstants : register(b0)
{
    matrix scaleRotateBillboardMat;
    matrix viewProjection;
};

cbuffer LimitPosConstData : register(b1)
{
    float4 limitPos[10];
    uint limitIndexMaxNum;
};

struct UpdateData
{
    uint startIndex;
    float rate;
    float2 offset;
    float4 color;
};

struct OutputData
{
    matrix mat;
    float4 color;
};

//�X�V
RWStructuredBuffer<UpdateData> updateData : register(u0);
//�o��
RWStructuredBuffer<OutputData> worldPosColorData : register(u1);

[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;


    //�X�v���C���Ȑ��̋���--------------------------------------------
    updateData[index].rate += 0.01f;
    if(1.0f < updateData[index].rate)
    {
        updateData[index].rate = 0.0f;
        ++updateData[index].startIndex;
    }
    if(limitIndexMaxNum <= updateData[index].startIndex)
    {
        updateData[index].startIndex = 0;
    }
    //�X�v���C���Ȑ��̋���--------------------------------------------

    //�X�v���C���Ȑ���̈ʒu������
    float3 basePos = SplinePosition(limitPos,updateData[index].startIndex,updateData[index].rate,limitIndexMaxNum,false);
	float3 offset = float3(updateData[index].offset.xy,0);

    //�X�v���C���Ȑ��ɉ������ʒu����I�t�Z�b�g�𑫂����l���p�[�e�B�N���̈ʒu
    float3 particlePos = basePos + offset;

    //�s��v�Z-------------------------
    matrix pMatWorld = scaleRotateBillboardMat;
    pMatWorld[0][3] = particlePos.x;
    pMatWorld[1][3] = particlePos.y;
    pMatWorld[2][3] = particlePos.z;
    //�s��v�Z-------------------------

    //�o��--------------------------------------------
    OutputData outputMat;
    outputMat.mat = mul(viewProjection,pMatWorld);     
    outputMat.color = updateData[index].color;
    worldPosColorData[index] = outputMat;
    //�o��--------------------------------------------
}
