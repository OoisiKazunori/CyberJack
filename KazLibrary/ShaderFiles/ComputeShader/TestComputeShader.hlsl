#include"../ShaderHeader/KazMathHeader.hlsli"

struct SceneConstantBuffer
{
    float4 velocity;
    float4 offset;
    float4 color;
    float4x4 projection;
    float4 padding[9];
};

struct IndirectCommand
{
    uint2 cbvAddress;
    uint4 drawArguments;
};


struct InputData
{
    float3 pos;
    float4 velocity;
    float4 color;
};

struct OutputData
{
    matrix mat;
    float4 color;
};


cbuffer RootConstants : register(b0)
{
    matrix view;        //�r���[�s��
    matrix projection;  //�v���W�F�N�V�����s��
    float increSize;    //�C���N�������g�̃T�C�Y
    float gpuAddress;   //�\���̃o�b�t�@�̐擪�A�h���X
};

//���͗p�̃o�b�t�@-------------------------
StructuredBuffer<InputData> inputBuffer : register(t0);
//���͗p�̃o�b�t�@-------------------------

//�o�͗p�̃o�b�t�@-------------------------
//�s��v�Z
AppendStructuredBuffer<OutputData> matrixData : register(u0);
//���͍X�V
AppendStructuredBuffer<InputData> updateInputData : register(u1);
//�C���_�C���N�g�R�}���h
AppendStructuredBuffer<IndirectCommand> outputCommands : register(u2);
//�o�͗p�̃o�b�t�@-------------------------

[numthreads(128, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex)
{
    uint index = (groupId.x * 128) + groupIndex;
    
   
    
    //���W�v�Z-------------------------
    float3 outputPos = inputBuffer[index].pos;
    
    outputPos += float3(1.0f, 0.0f, 0.0f);
    if (50.0f <= outputPos.x)
    {
        outputPos = 0.0f;
    }
    
    matrix matTrans = Translate(outputPos);
    matrix matRot = RotateX(0.0f) * RotateY(0.0f) * RotateZ(0.0f);
    matrix matScale = Scale(float3(1.0f, 1.0f, 1.0f));
    matrix matWorld = matScale * matRot * matTrans;
    //���W�v�Z-------------------------
    
    
    //���W�o��-------------------------
    OutputData outputMat;
    outputMat.mat = matWorld;
    outputMat.color = float4(1.0f,1.0f,1.0f,1.0f);
    
    matrixData.Append(outputMat);
    updateInputData.Append(inputBuffer[index]);
    //���W�o��-------------------------
    
    
    //�`��R�}���h�o��-------------------------
    IndirectCommand outputCommand;
    outputCommand.cbvAddress = 0 + index * 32;
    outputCommand.drawArguments = float4(3, 1, 0, 0);
    outputCommands.Append(outputCommand);
    //�`��R�}���h�o��-------------------------

}