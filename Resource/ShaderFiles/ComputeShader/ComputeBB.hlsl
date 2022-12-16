#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"

//���_���W
RWStructuredBuffer<float4> vertciesData : register(u0);
//�o��
RWStructuredBuffer<BoundingBoxData> bbPosData : register(u1);

cbuffer RootConstants : register(b2)
{
    matrix scaleRotaMat;
};

//���f���̌`�ɉ�����BoundingBox�����ׂɂ�XYZ���ꂼ��̎��̍ő�l�A�ŏ��l���v�Z����
[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = ThreadGroupIndex(groupId,groupIndex,groupThreadID,1024);

    //�ŏ��l�v�Z
    bbPosData[0].minPos = MinFloat3(bbPosData[0].minPos, vertciesData[index].xyz);
    //�ő�l�v�Z
    bbPosData[0].maxPos = MaxFloat3(bbPosData[0].maxPos, vertciesData[index].xyz);
}
