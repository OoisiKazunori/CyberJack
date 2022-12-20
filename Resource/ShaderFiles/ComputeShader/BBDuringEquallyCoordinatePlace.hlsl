#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"

//BB�̒l
RWStructuredBuffer<BoundingBoxData> bbPosData : register(u0);
//�����蔻����W
RWStructuredBuffer<float3> hitBoxData : register(u1);
//�����蔻���ID
RWStructuredBuffer<uint3> idData : register(u2);

cbuffer RootConstants : register(b0)
{
    float diameter;
    uint xMax;
    uint xyMax;
};

//BoundingBox���ɋ��̓����蔻��̍��W���ϓ��ɕ��ׂ�
[numthreads(1, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = ThreadGroupIndex(groupId,xMax,xyMax);

    //�[���珇�ɋ�����ׂ�
    float3 pos = (bbPosData[0].minPos + diameter / 2.0f) + groupId * diameter;

    hitBoxData[index] = pos;
    idData[index] = groupId;
}
