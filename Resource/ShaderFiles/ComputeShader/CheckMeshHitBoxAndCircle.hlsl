#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"
#include"../ShaderHeader/Collision.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"

cbuffer RootConstants : register(b0)
{
    uint cpuHitBoxNum;
    float particleRadius;
};
struct CPUHitBoxData
{
    float3 pos;
    float radius;
};

//���b�V�������蔻��
RWStructuredBuffer<MeshHitBox> meshHitBoxArrayData : register(u0);
//CPU�����蔻��
RWStructuredBuffer<CPUHitBoxData> cpuHitBoxArrayData : register(u1);
//�����̋��Ɣ�����������
AppendStructuredBuffer<MeshHitBox> hitBoxAppendData : register(u2);

//���b�V���p�[�e�B�N���Ƌ��̔���
[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    if(216 <= index)
    {
        return;
    }

    CircleData meshHitBox;
    meshHitBox.pos = meshHitBoxArrayData[index].pos;
    meshHitBox.radius = particleRadius;

    MeshHitBox idData;
    idData.meshID = meshHitBoxArrayData[index].meshID;
    idData.id = meshHitBoxArrayData[index].id;

    CircleData hitBox;
    for(int i = 0;i < cpuHitBoxNum; ++i)
    {
        hitBox.pos = cpuHitBoxArrayData[i].pos;
        hitBox.radius = cpuHitBoxArrayData[i].radius;

        idData.pos = hitBox.pos;
        //�����蔻�肪��ꂽ�瓖�������C���f�b�N�X��ۑ����A�Փˌ�̏����Ɏg��
        if(CheckCircleAndCircle(meshHitBox,hitBox))
        {
            hitBoxAppendData.Append(idData);
            return;
        }
    }
    
    idData.meshID = -1;
    idData.id = uint3(-1,-1,-1);
    hitBoxAppendData.Append(idData);
}
