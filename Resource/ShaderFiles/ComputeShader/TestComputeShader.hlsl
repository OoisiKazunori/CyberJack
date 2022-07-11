#include"../ShaderHeader/KazMathHeader.hlsli"

struct SceneConstantBuffer
{
    float4 velocity;
    float4 offset;
    float4 color;
    float4x4 projection;
    float4 padding[9];
};

//https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_draw_arguments
struct D3D12_DRAW_ARGUMENTS
{
    uint VertexCountPerInstance;
    uint InstanceCount;
    uint StartVertexLocation;
    uint StartInstanceLocation;
};


struct IndirectCommand
{
    uint64_t cbvAddress;    
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
    matrix view;        //?ｿｽr?ｿｽ?ｿｽ?ｿｽ[?ｿｽs?ｿｽ?ｿｽ
    matrix projection;  //?ｿｽv?ｿｽ?ｿｽ?ｿｽW?ｿｽF?ｿｽN?ｿｽV?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽs?ｿｽ?ｿｽ
    uint increSize;     //?ｿｽ關費ｿｽo?ｿｽb?ｿｽt?ｿｽ@?ｿｽﾌ構?ｿｽ?ｿｽ?ｿｽﾌサ?ｿｽC?ｿｽY
    uint64_t gpuAddress; //?ｿｽ關費ｿｽo?ｿｽb?ｿｽt?ｿｽ@?ｿｽﾌ先頭?ｿｽA?ｿｽh?ｿｽ?ｿｽ?ｿｽX
};

//?ｿｽ?ｿｽ?ｿｽﾍ用?ｿｽﾌバ?ｿｽb?ｿｽt?ｿｽ@-------------------------
StructuredBuffer<InputData> inputBuffer : register(t0);
//?ｿｽ?ｿｽ?ｿｽﾍ用?ｿｽﾌバ?ｿｽb?ｿｽt?ｿｽ@-------------------------

//?ｿｽo?ｿｽﾍ用?ｿｽﾌバ?ｿｽb?ｿｽt?ｿｽ@-------------------------
//?ｿｽs?ｿｽ?ｿｽ
AppendStructuredBuffer<OutputData> matrixData : register(u1);
//?ｿｽ?ｿｽ?ｿｽﾍ更?ｿｽV
AppendStructuredBuffer<InputData> updateInputData : register(u2);
//?ｿｽ`?ｿｽ?ｿｽR?ｿｽ}?ｿｽ?ｿｽ?ｿｽh
AppendStructuredBuffer<IndirectCommand> outputCommands : register(u3);
//?ｿｽo?ｿｽﾍ用?ｿｽﾌバ?ｿｽb?ｿｽt?ｿｽ@-------------------------

static const int NUM = 10;

[numthreads(NUM, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex)
{
    uint index = (groupId.x * NUM) + groupIndex;

    //?ｿｽs?ｿｽ?ｿｽv?ｿｽZ-------------------------
    float3 outputPos = inputBuffer[index].pos.xyz;
    
    outputPos = float3(0.0f + index * 20.0f,0.0f,20.0f);
    matrix pMatTrans = Translate(outputPos);
    matrix pMatRot = Rotate(float3(0.0f,0.0f,0.0f));
    matrix pMatScale = Scale(float3(15.0f, 15.0f, 15.0f));
    
    matrix pMatWorld = MatrixIdentity();
    pMatWorld = mul(pMatScale, pMatWorld);
    pMatWorld = mul(pMatRot, pMatWorld);
    pMatWorld = mul(pMatTrans, pMatWorld);
    //?ｿｽs?ｿｽ?ｿｽv?ｿｽZ-------------------------
    
    
    //?ｿｽo?ｿｽﾍ用-------------------------
    OutputData outputMat;
    matrix lView = view;
    matrix lproj = projection;

    //?ｿｽs?ｿｽ?ｿｽo?ｿｽ?ｿｽ
    outputMat.mat = MatrixIdentity();
    outputMat.mat = mul(pMatWorld,outputMat.mat);
    outputMat.mat = mul(lView,    outputMat.mat);
    outputMat.mat = mul(lproj,    outputMat.mat);
    outputMat.color = inputBuffer[index].color;
    matrixData.Append(outputMat);
    //matrixData[index] = outputMat;


    //?ｿｽ?ｿｽ?ｿｽﾍ更?ｿｽV?ｿｽo?ｿｽ?ｿｽ-------------------------
    InputData inputData;
    inputData.pos = float4(outputPos.xyz, 0.0f);
    inputData.velocity = inputBuffer[index].velocity;
    inputData.color = inputBuffer[index].color;
    updateInputData.Append(inputData);
    //updateInputData[index] = inputData;

    
    //?ｿｽ`?ｿｽ?ｿｽR?ｿｽ}?ｿｽ?ｿｽ?ｿｽh?ｿｽo?ｿｽ?ｿｽ-------------------------
    if(index < 1)
    {
        IndirectCommand outputCommand;
        outputCommand.cbvAddress = gpuAddress + index * increSize;
        outputCommand.drawArguments.VertexCountPerInstance = 3;
        outputCommand.drawArguments.InstanceCount = NUM;
        outputCommand.drawArguments.StartVertexLocation = 0;
        outputCommand.drawArguments.StartInstanceLocation = 0;
        outputCommands.Append(outputCommand);
        //outputCommands[index] = outputCommand;
    }
    //?ｿｽo?ｿｽﾍ用-------------------------

}