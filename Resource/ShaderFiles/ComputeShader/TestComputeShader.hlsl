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
    matrix view;        //ビュー行列
    matrix projection;  //プロジェクション行列
    uint increSize;     //定数バッファの構造体サイズ
    uint64_t gpuAddress; //定数バッファの先頭アドレス
};

//入力用のバッファ-------------------------
StructuredBuffer<InputData> inputBuffer : register(t0);
//入力用のバッファ-------------------------

//出力用のバッファ-------------------------
//行列
AppendStructuredBuffer<OutputData> matrixData : register(u0);
//入力更新
AppendStructuredBuffer<InputData> updateInputData : register(u1);
//描画コマンド
AppendStructuredBuffer<IndirectCommand> outputCommands : register(u2);
//出力用のバッファ-------------------------

static const int NUM = 10;

[numthreads(NUM, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex)
{
    uint index = (groupId.x * NUM) + groupIndex;

    //行列計算-------------------------
    float3 outputPos = inputBuffer[index].pos.xyz;
    
    outputPos = float3(0.0f + index * 20.0f,0.0f,20.0f);
    matrix pMatTrans = Translate(outputPos);
    matrix pMatRot = Rotate(float3(0.0f,0.0f,0.0f));
    matrix pMatScale = Scale(float3(15.0f, 15.0f, 15.0f));
    
    matrix pMatWorld = MatrixIdentity();
    pMatWorld = mul(pMatScale, pMatWorld);
    pMatWorld = mul(pMatRot, pMatWorld);
    pMatWorld = mul(pMatTrans, pMatWorld);
    //行列計算-------------------------
    
    
    //出力用-------------------------
    OutputData outputMat;
    matrix lView = view;
    matrix lproj = projection;

    //行列出力
    outputMat.mat = MatrixIdentity();
    outputMat.mat = mul(pMatWorld,outputMat.mat);
    outputMat.mat = mul(lView,    outputMat.mat);
    outputMat.mat = mul(lproj,    outputMat.mat);
    outputMat.color = inputBuffer[index].color;
    matrixData.Append(outputMat);
    //matrixData[index] = outputMat;


    //入力更新出力-------------------------
    InputData inputData;
    inputData.pos = float4(outputPos.xyz, 0.0f);
    inputData.velocity = inputBuffer[index].velocity;
    inputData.color = inputBuffer[index].color;
    updateInputData.Append(inputData);
    //updateInputData[index] = inputData;

    
    //描画コマンド出力-------------------------
    IndirectCommand outputCommand;
    outputCommand.cbvAddress = gpuAddress + index * increSize;
    outputCommand.drawArguments.VertexCountPerInstance = 3;
    outputCommand.drawArguments.InstanceCount = 1;
    outputCommand.drawArguments.StartVertexLocation = 0;
    outputCommand.drawArguments.StartInstanceLocation = 0;
    outputCommands.Append(outputCommand);
    //outputCommands[index] = outputCommand;
    //出力用-------------------------

}