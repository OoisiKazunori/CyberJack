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
//構造体でなければシェーダーコンパイル時にメモリアクセス違反が吐かれる。先生と原因相談中。
struct OutputMatData
{
    matrix worldMat;
};
struct OutputColorData
{
    float4 color;
};
//入力
RWStructuredBuffer<OutputMatData> worldMatData : register(u0);
RWStructuredBuffer<OutputColorData> colorData : register(u1);

AppendStructuredBuffer<OutputData>drawData:register(u2);

//ワールド行列の座標や色からカリング処理を行い、描画用のバッファに変換する処理
[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = ThreadGroupIndex(groupId,groupIndex,groupThreadID,1024);
        
    OutputMatData worldMat = worldMatData[index];
    OutputColorData color = colorData[index];

    //カリング処理全般をここに追加予定

    OutputData outputMat;
    outputMat.mat = mul(viewProjection,worldMat.worldMat);
    outputMat.color = color.color;
    drawData.Append(outputMat);
}
