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
    matrix view; //?r???[?s??
    matrix projection; //?v???W?F?N?V?????s??
    uint increSize; //?C???N???????g??T?C?Y
    uint64_t gpuAddress; //?\????o?b?t?@????A?h???X
};

//????p??o?b?t?@-------------------------
StructuredBuffer<InputData> inputBuffer : register(t0);
//????p??o?b?t?@-------------------------

//?o??p??o?b?t?@-------------------------
//?s??v?Z
RWStructuredBuffer<OutputData> matrixData : register(u0);
//????X?V
RWStructuredBuffer<InputData> updateInputData : register(u1);
//?C???_?C???N?g?R?}???h
RWStructuredBuffer<IndirectCommand> outputCommands : register(u2);
//?o??p??o?b?t?@-------------------------

static const int NUM = 2;

[numthreads(NUM, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex)
{
    uint index = (groupId.x * NUM) + groupIndex;
    //uint index = 1;
    if(1 <= index)
    {
        index = 1;
    }

    //???W?v?Z-------------------------
    float3 outputPos = inputBuffer[index].pos.xyz;
    
    //outputPos += float3(1.0f, 0.0f, 0.0f);
    //if (50.0f <= outputPos.x)
    //{
    //    outputPos = 0.0f;
    //}

    outputPos = float3(0.0f + index * 20.0f,0.0f,20.0f);
    matrix pMatTrans = Translate(outputPos);
    matrix pMatRot = Rotate(float3(0.0f,0.0f,0.0f));
    matrix pMatScale = Scale(float3(15.0f, 15.0f, 15.0f));
    
    matrix pMatWorld = MatrixIdentity();
    pMatWorld = mul(pMatScale, pMatWorld);
    pMatWorld = mul(pMatRot, pMatWorld);
    pMatWorld = mul(pMatTrans, pMatWorld);
    //???W?v?Z-------------------------
    
    
    //???W?o??-------------------------
    OutputData outputMat;
    matrix lView = view;
    matrix lproj = projection;

    outputMat.mat = MatrixIdentity();
    outputMat.mat = mul(pMatWorld,outputMat.mat);
    outputMat.mat = mul(lView,    outputMat.mat);
    outputMat.mat = mul(lproj,    outputMat.mat);
    outputMat.color = inputBuffer[index].color;
    //matrixData.Append(outputMat);
    matrixData[index] = outputMat;
  
    InputData inputData;
    inputData.pos = float4(outputPos.xyz, 0.0f);
    inputData.velocity = inputBuffer[index].velocity;
    inputData.color = inputBuffer[index].color;
    //updateInputData.Append(inputData);
    updateInputData[index] = inputData;
    //???W?o??-------------------------
    
    
    //?`??R?}???h?o??-------------------------
    IndirectCommand outputCommand;
    outputCommand.cbvAddress = gpuAddress + index * increSize;
    outputCommand.drawArguments.VertexCountPerInstance = 3;
    outputCommand.drawArguments.InstanceCount = 1;
    outputCommand.drawArguments.StartVertexLocation = 0;
    outputCommand.drawArguments.StartInstanceLocation = 0;
    outputCommands[index] = outputCommand;
    //outputCommands.Append(outputCommand);    
    //?`??R?}???h?o??-------------------------

}