#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"


//�p�[�e�B�N�������蔻��
RWStructuredBuffer<ParticleHitBoxData> hitBoxData : register(u0);
//���������C���f�b�N�X
RWStructuredBuffer<MeshHitBox> hitIndexData : register(u1);
//�o��
AppendStructuredBuffer<GPUParticleInput> inputGPUParticleData : register(u2);

RWStructuredBuffer<float3> larpPosData : register(u3);

//����̓����蔻��Ƃ��Ƃ�
bool CheckLinkHitBox(uint3 BASE_HIT_INDEX,uint3 CHECK_HIT_INDEX)
{
    bool linkHitFlagArray[7];
    linkHitFlagArray[0] = (bool)(CHECK_HIT_INDEX == BASE_HIT_INDEX);                    //�^��
    linkHitFlagArray[1] = (bool)(CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(0,1,0)));  //��
    linkHitFlagArray[2] = (bool)(CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(0,-1,0))); //��
    linkHitFlagArray[3] = (bool)(CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(-1,0,0))); //��
    linkHitFlagArray[4] = (bool)(CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(1,0,0)));  //�E
    linkHitFlagArray[5] = (bool)(CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(0,0,1)));  //�O
    linkHitFlagArray[6] = (bool)(CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(0,0,-1))); //��

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
[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = ThreadGroupIndex(groupId,groupIndex,groupThreadID,1024);

    ParticleData particleData;
    particleData.color = hitBoxData[index].color;
    particleData.pos = hitBoxData[index].pos;

    float larpVel = 0.01f;
    float3 basePos = hitBoxData[index].pos;
    //�����C���f�b�N�X�̏ꍇ�A�p�[�e�B�N���𓮂���������ǉ�����
    for(int i = 0;i < 1; ++i)
    {
        //�����蔻��+�����N�t�����݂̏��������
        if(CheckLinkHitBox(hitIndexData[i].id,hitBoxData[index].id))
        {
            float3 posHitBoxVec = hitIndexData[i].pos;
            float3 posParticleVec = hitBoxData[index].pos - hitIndexData[i].pos;
    
            //�Ȃ��p�̔��f
            float cos = dot(posParticleVec,posHitBoxVec) / (length(posParticleVec) * length(posHitBoxVec));
            float sin = acos(cos);
    
            int angle = RadianToAngle(sin) - 90;
            float rate = angle / 90.0f;
    
            float3 vel = normalize(posParticleVec) * 5.5f * rate;
   
            larpVel = 0.1f;
            basePos = hitBoxData[index].pos + vel;            
        }
        else
        {
            basePos = hitBoxData[index].pos;
        }
    }
    //�s��v�Z�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[    
    larpPosData[index] = Larp(basePos,larpPosData[index],larpVel);
    particleData.pos = larpPosData[index];

    matrix worldMat = CalucurateWorldMat(float3(0,0,0),float3(0.02,0.02,0.02),float3(0,0,0));
    worldMat[0][3] = particleData.pos.x;
    worldMat[1][3] = particleData.pos.y;
    worldMat[2][3] = particleData.pos.z;
    //worldMat = mul(motherMatData[particleData.id],worldMat);    
    //�s��v�Z�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

    //�p�[�e�B�N�����̕`��,�����������ǂ������\������
    GPUParticleInput inputData;    
    inputData.worldMat = worldMat;
    inputData.color = float4(1,1,1,1);

    for(int hitIndex = 0;hitIndex < 216; ++hitIndex)
    {
        bool isHitFlag = hitIndexData[hitIndex].meshID == hitBoxData[index].meshID && hitIndexData[hitIndex].id == hitBoxData[index].id;
        if(isHitFlag)
        {
            inputData.color = float4(1,0,0,1);
            break;
        }
    }

    inputGPUParticleData.Append(inputData);
    //�p�[�e�B�N�����̕`��,�����������ǂ������\������
}
