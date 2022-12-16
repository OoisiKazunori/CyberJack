#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"

//BB�̒l
RWStructuredBuffer<BoundingBoxData> bbPosData : register(u0);
//�����蔻����W
RWStructuredBuffer<MeshHitBox> hitBoxData : register(u1);

cbuffer RootConstants : register(b2)
{
    float radius;
    uint xMax;
    uint xyMax;
};

//BoundingBox���ɋ��̓����蔻��̍��W���ϓ��ɕ��ׂ�
[numthreads(1, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = ThreadGroupIndex(groupId,xMax,xyMax);

    float diameter = radius;
    //�[���珇�ɋ�����ׂ�
    float3 pos = (bbPosData[0].minPos + radius) + groupId * diameter;

    hitBoxData[index].pos = pos;
    hitBoxData[index].id = groupId;
}
