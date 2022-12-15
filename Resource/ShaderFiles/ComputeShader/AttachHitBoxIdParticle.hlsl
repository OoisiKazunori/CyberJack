#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/Collision.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"

cbuffer RootConstants : register(b0)
{
    float particleRadius;
    float meshHitBoxRadius;
    uint MeshHitBoxMaxNum;
};

//BB�̂��ꂼ��̎��̍ő�l�A�ŏ��l������
struct BBData
{
    float3 minPos;
    float3 maxPos;
};

//�����蔻����W
RWStructuredBuffer<MeshHitBox> hitBoxData : register(u0);
//�p�[�e�B�N�����
RWStructuredBuffer<ParticleHitBoxData> particleData : register(u1);

//���̓����蔻��ƃp�[�e�B�N���̓����蔻�������āA���Ɠ���ID���p�[�e�B�N���ɕt�^����
[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint particleIndex = ThreadGroupIndex(groupId,groupIndex,groupThreadID,1024);

    CircleData particleHitBox;
    particleHitBox.pos = particleData[particleIndex].pos;
    particleHitBox.radius = particleRadius;

    //���̔���ƃp�[�e�B�N���̔�����m���߂�
    for(int i = 0; i < MeshHitBoxMaxNum;++i)
    {
        CircleData meshHitBox;
        meshHitBox.pos = hitBoxData[i].pos;
        meshHitBox.radius = meshHitBoxRadius;

        //�L���Ȃ狅��ID���p�[�e�B�N����ID�ɓ����
        if(CheckCircleAndCircle(meshHitBox,particleHitBox))
        {
            particleData[particleIndex].id = hitBoxData[i].id;
        }
    }
}
