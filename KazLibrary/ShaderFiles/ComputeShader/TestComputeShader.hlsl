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
    matrix view;
    matrix projection;
    uint64_t gpuAddress;    //定数バッファの先頭アドレス
    uint increSize;         //構造体のサイズ
};

//入力用バッファ-------------------------
StructuredBuffer<InputData> inputBuffer : register(t0);
//入力用バッファ-------------------------

//出力用バッファ-------------------------
//行列用の情報
AppendStructuredBuffer<OutputData> matrixData : register(u0);
//更新用のバッファ
AppendStructuredBuffer<InputData> updateInputData : register(u1);
//DrawIndirectのバッファ
AppendStructuredBuffer<IndirectCommand> outputCommands : register(u2);
//出力用バッファ-------------------------

static const int NUM = 1024;

[numthreads(NUM, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex)
{
    uint index = (groupId.x * NUM) + groupIndex;
    
    //行列の計算-------------------------
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
    //行列の計算-------------------------
    
    
    //行列と色の出力-------------------------
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
    //行列と色の出力-------------------------
    
    
    //DrawIndirectの出力-------------------------
    IndirectCommand outputCommand;
    outputCommand.cbvAddress = gpuAddress + index * increSize;
    outputCommand.drawArguments.InstanceCount = 3;
    outputCommand.drawArguments.StartInstanceLocation = 1;
    outputCommand.drawArguments.StartVertexLocation = 0;
    outputCommand.drawArguments.VertexCountPerInstance = 0;
    outputCommands.Append(outputCommand);
    //DrawIndirectの出力-------------------------

}