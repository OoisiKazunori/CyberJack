#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"

cbuffer RootConstants : register(b0)
{
    float3 pos;
    float radius;
};

//�p�[�e�B�N�������蔻��
RWStructuredBuffer<ParticleHitBoxData> hitBoxData : register(u1);

//���������C���f�b�N�X
StructuredBuffer<uint3> hitIndexData : register(u1);

StructuredBuffer<uint> hitIndexCounter:register(u2);

//����̓����蔻��Ƃ��Ƃ�
bool CheckLinkHitBox(uint3 BASE_HIT_INDEX,uint3 CHECK_HIT_INDEX)
{
    bool linkHitFlagArray[7];
    linkHitFlagArray[0] = CHECK_HIT_INDEX == BASE_HIT_INDEX;                    //�^��
    linkHitFlagArray[1] = CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(0,1,0));  //��
    linkHitFlagArray[2] = CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(0,-1,0)); //��
    linkHitFlagArray[3] = CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(-1,0,0)); //��
    linkHitFlagArray[4] = CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(1,0,0));  //�E
    linkHitFlagArray[5] = CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(0,0,1));  //�O
    linkHitFlagArray[6] = CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(0,0,-1)); //��

    for(int i = 0;i < 7;++i)
    {
        if(linkHitFlagArray[i])
        {
            return true;
        }
    }
    return false;
}

//���b�V���p�[�e�B�N���̋������������C���f�b�N�X���Q�l�Ƀp�[�e�B�N���̋��������߂�
[numthreads(1, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = ThreadGroupIndex(groupId,groupIndex,groupThreadID,1024);

    //�����C���f�b�N�X�̏ꍇ�A�p�[�e�B�N���𓮂���������ǉ�����
    for(int i = 0;i < hitIndexCounter[0]; ++i)
    {
        //�����蔻��+�����N�t�����݂̏��������
        if(CheckLinkHitBox(hitIndexData[i],hitBoxData[index].id))
        {
            float3 posHitBoxVec = pos;
            float3 posParticleVec = hitBoxData[index].pos - pos;

            //�Ȃ��p�̔��f
            float cos = dot(posParticleVec,posHitBoxVec) / (length(posParticleVec) * lenght(posHitBoxVec));
            float sin = acos(cos);

            int angle = RadianToAngle(sin) - 90;
            float rate = angle / 90.0f;

            float3 vel = normalize(posParticleVec) * 5.5f * rate;
            vel.x = 0.0f;

            //�p�[�e�B�N���̊�{���W�ƃ��[�v�p�̍��W���K�v
            float3 larpPos = float3(0,0,0) + vel;

        }
    }
}
