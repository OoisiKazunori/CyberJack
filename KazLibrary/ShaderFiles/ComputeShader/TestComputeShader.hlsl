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

cbuffer RootConstants : register(b0)
{
    float xOffset;        // Half the width of the triangles.
    float zOffset;        // The z offset for the triangle vertices.
    float cullOffset;    // The culling plane offset in homogenous space.
    float commandCount;    // The number of commands to be processed.
};

//定数バッファ
StructuredBuffer<SceneConstantBuffer> inputData : register(t0);
//インダイレクトコマンド
StructuredBuffer<IndirectCommand> indirectData : register(t1);
//結果保存
AppendStructuredBuffer<IndirectCommand> outputCommands : register(u0);

[numthreads(128, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex)
{
    uint index = (groupId.x * 128) + groupIndex;
    
    matrix matTrans = Translate(float3(0.0f, 0.0f, 0.0f));
    matrix matRot = RotateX(0.0f) * RotateY(0.0f) * RotateZ(0.0f);
    matrix matScale = Scale(float3(0.1f, 0.1f, 0.1f));
    
    matrix matWorld = matScale * matRot * matTrans;
    
    
    outputCommands.Append(indirectData[index]);
}