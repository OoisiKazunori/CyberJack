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
    matrix view;        //ビュー行列
    matrix projection;  //プロジェクション行列
    float increSize;    //インクリメントのサイズ
    float gpuAddress;   //構造体バッファの先頭アドレス
};

//入力用のバッファ-------------------------
StructuredBuffer<InputData> inputBuffer : register(t0);
//入力用のバッファ-------------------------

//出力用のバッファ-------------------------
//行列計算
AppendStructuredBuffer<OutputData> matrixData : register(u0);
//入力更新
AppendStructuredBuffer<InputData> updateInputData : register(u1);
//インダイレクトコマンド
AppendStructuredBuffer<IndirectCommand> outputCommands : register(u2);
//出力用のバッファ-------------------------

[numthreads(128, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex)
{
    uint index = (groupId.x * 128) + groupIndex;
    
   
    
    //座標計算-------------------------
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
    //座標計算-------------------------
    
    
    //座標出力-------------------------
    OutputData outputMat;
    outputMat.mat = matWorld;
    outputMat.color = float4(1.0f,1.0f,1.0f,1.0f);
    
    matrixData.Append(outputMat);
    updateInputData.Append(inputBuffer[index]);
    //座標出力-------------------------
    
    
    //描画コマンド出力-------------------------
    IndirectCommand outputCommand;
    outputCommand.cbvAddress = 0 + index * 32;
    outputCommand.drawArguments = float4(3, 1, 0, 0);
    outputCommands.Append(outputCommand);
    //描画コマンド出力-------------------------

}