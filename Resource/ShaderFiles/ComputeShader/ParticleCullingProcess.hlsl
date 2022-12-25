#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"

struct OutputData
{
    matrix mat;    
    float4 color;
};


cbuffer RootConstants : register(b0)
{    
    matrix viewProjection;
};
//�\���̂łȂ���΃V�F�[�_�[�R���p�C�����Ƀ������A�N�Z�X�ᔽ���f�����B�搶�ƌ������k���B
struct OutputMatData
{
    matrix worldMat;
};
struct OutputColorData
{
    float4 color;
};
//����
RWStructuredBuffer<OutputMatData> worldMatData : register(u0);
RWStructuredBuffer<OutputColorData> colorData : register(u1);

AppendStructuredBuffer<OutputData>drawData:register(u2);

//���[���h�s��̍��W��F����J�����O�������s���A�`��p�̃o�b�t�@�ɕϊ����鏈��
[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = ThreadGroupIndex(groupId,groupIndex,groupThreadID,1024);
        
    OutputMatData worldMat = worldMatData[index];
    OutputColorData color = colorData[index];

    //�J�����O�����S�ʂ������ɒǉ��\��

    OutputData outputMat;
    outputMat.mat = mul(viewProjection,worldMat.worldMat);
    outputMat.color = color.color;
    drawData.Append(outputMat);
}
