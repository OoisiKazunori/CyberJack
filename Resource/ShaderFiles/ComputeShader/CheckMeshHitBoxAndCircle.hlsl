#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"
#include"../ShaderHeader/Collision.hlsli"

cbuffer RootConstants : register(b0)
{
    float3 pos;
    float radius;
    float particleRadius;
    uint xMax;
    uint xyMax;
};

//���b�V�������蔻��
RWStructuredBuffer<MeshHitBox> meshHitBoxPos : register(u0);
RWStructuredBuffer<uint3> meshHitBoxID : register(u1);

//�����̋��Ɣ�����������
AppendStructuredBuffer<uint3> hitBoxData : register(u1);

//���b�V���p�[�e�B�N���Ƌ��̔���
[numthreads(1, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = ThreadGroupIndex(groupId,xMax,xyMax);

    CircleData meshHitBox;
    meshHitBox.pos = meshHitBoxPos[index];
    meshHitBox.radius = particleRadius;

    CircleData hitBox;
    hitBox.pos = pos;
    hitBox.radius = radius;

    //�����蔻�肪��ꂽ�瓖�������C���f�b�N�X��ۑ����A�Փˌ�̏����Ɏg��
    if(CheckCircleAndCircle(meshHitBox,hitBox))
    {
        hitBoxData.Append(meshHitBoxID[index]);
    }
}
