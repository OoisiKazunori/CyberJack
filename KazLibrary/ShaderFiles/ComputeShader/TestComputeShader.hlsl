#include"../ShaderHeader/KazMathHeader.hlsli"

struct SceneConstantBuffer
{
    float4 velocity;
    float4 offset;
    float4 color;
    float4x4 projection;
    float4 padding[9];
};

struct D3D12_DRAW_ARGUMENTS
{
   uint VertexCountPerInstance;
   uint InstanceCount;
   uint StartVertexLocation;
   uint StartInstanceLocation;
};

struct IndirectCommand
{
    uint2 cbvAddress;
    D3D12_DRAW_ARGUMENTS drawArguments;
};


struct InputData
{
    float4 pos;
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
    matrix view; //�r���[�s��
    matrix projection; //�v���W�F�N�V�����s��
    uint gpuAddress; //�\���̃o�b�t�@�̐擪�A�h���X
    uint increSize; //�C���N�������g�̃T�C�Y
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

static const int NUM = 1024;

[numthreads(NUM, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex)
{
    uint index = (groupId.x * NUM) + groupIndex;
    
    //���W�v�Z-------------------------
    float3 outputPos = inputBuffer[index].pos.xyz;
    
    outputPos += float3(1.0f, 0.0f, 0.0f);
    if (50.0f <= outputPos.x)
    {
        outputPos = 0.0f;
    }
    matrix pMatTrans = Translate(outputPos);
    matrix pMatRot = Rotate(0.0f);
    matrix pMatScale = Scale(float3(1.0f, 1.0f, 1.0f));
    
    matrix pMatWorld = MatrixIdentity();
    pMatWorld = mul(pMatTrans, pMatWorld);
    pMatWorld = mul(pMatRot, pMatWorld);
    pMatWorld = mul(pMatScale, pMatWorld);
    //���W�v�Z-------------------------
    
    
    //���W�o��-------------------------
    OutputData outputMat;
    outputMat.mat = pMatWorld;
    outputMat.mat = mul(view, outputMat.mat);
    outputMat.mat = mul(projection, outputMat.mat); 
    outputMat.color = inputBuffer[index].color;
    matrixData.Append(outputMat);
    
    InputData inputData;
    inputData.pos = float4(outputPos.xyz, 0.0f);
    inputData.velocity = inputBuffer[index].velocity;
    inputData.color = inputBuffer[index].color;
 
    updateInputData.Append(inputData);
    //���W�o��-------------------------
    
    
    //�`��R�}���h�o��-------------------------
    IndirectCommand outputCommand;
    outputCommand.cbvAddress = gpuAddress + index * increSize;
    outputCommand.drawArguments.InstanceCount = 3;
    outputCommand.drawArguments.StartInstanceLocation = 1;
    outputCommand.drawArguments.StartVertexLocation = 0;
    outputCommand.drawArguments.VertexCountPerInstance = 0;
    outputCommands.Append(outputCommand);
    //�`��R�}���h�o��-------------------------

}