#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"

//BB�̒l
RWStructuredBuffer<BoundingBoxData> bbPosData : register(u0);
//�����蔻����W
RWStructuredBuffer<float3> hitBoxData : register(u1);
//�����蔻���ID
RWStructuredBuffer<uint3> idData : register(u2);
//�f�o�b�N�p�A�����蔻��m�F
AppendStructuredBuffer<GPUParticleInput> outputData : register(u3);

cbuffer RootConstants : register(b0)
{
    float diameter;
    uint xMax;
    uint xyMax;
};

//BoundingBox���ɋ��̓����蔻��̍��W���ϓ��ɕ��ׂ�
[numthreads(1, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID)
{
    uint index = ThreadGroupIndex(groupId,xMax,xyMax);

    //�[���珇�ɋ�����ׂ�
    float3 pos = (bbPosData[0].minPos + diameter / 2.0f) + groupId * diameter;
    hitBoxData[index] = pos;
    idData[index] = groupId;

}


//BoundingBox���ɋ��̓����蔻��̍��W���ϓ��ɕ��ׂ�
[numthreads(1, 1, 1)]
void DebugCSmain(uint3 groupId : SV_GroupID)
{
    uint index = ThreadGroupIndex(groupId,xMax,xyMax);

    //�[���珇�ɋ�����ׂ�
    float3 pos = (bbPosData[0].minPos + diameter / 2.0f) + groupId * diameter;
    hitBoxData[index] = pos;
    idData[index] = groupId;

    //BB���ɂ�����Ɣz�u�o���Ă��邩�v�Z����B
    GPUParticleInput debugOutput;
    debugOutput.worldMat = CalucurateWorldMat(pos,float3(0.1f,0.1f,0.1f),float3(0.0f,0.0f,0.0f));
    debugOutput.color = float4(1,1,1,1);
    outputData.Append(debugOutput);
}
