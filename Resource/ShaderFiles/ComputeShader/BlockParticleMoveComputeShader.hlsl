#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/ExcuteIndirectHeader.hlsli"

struct UpdateData
{
    float4 pos;
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
    matrix billBoard;
};

//�X�V
RWStructuredBuffer<UpdateData> updateData : register(u0);
//�o��
RWStructuredBuffer<OutputData> matrixData : register(u1);

static const int THREAD_MAX = 10;

[numthreads(THREAD_MAX, THREAD_MAX, THREAD_MAX)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = (groupThreadID.y * THREAD_MAX) + groupThreadID.x + ((THREAD_MAX * THREAD_MAX) * groupThreadID.z);
    index += (THREAD_MAX * THREAD_MAX * THREAD_MAX) * groupId.x;

    uint outPutIndex = index;

    //�s��v�Z-------------------------
    matrix pMatTrans = Translate(updateData[outPutIndex].pos.xyz);
    matrix pMatRot = Rotate(float3(0.0f,0.0f,0.0f));
    float scale = 0.05f;
    matrix pMatScale = Scale(float3(scale, scale, scale));
    
    matrix pMatWorld = MatrixIdentity();
    pMatWorld = mul(pMatScale, pMatWorld);
    pMatWorld = mul(pMatRot, pMatWorld);
    pMatWorld = mul(billBoard, pMatWorld);
    pMatWorld = mul(pMatTrans, pMatWorld);
    //�s��v�Z-------------------------
    
    OutputData outputMat;
    outputMat.mat = MatrixIdentity();
    outputMat.mat = mul(pMatWorld,  outputMat.mat);
    outputMat.mat = mul(view,       outputMat.mat);
    outputMat.mat = mul(projection, outputMat.mat);
    outputMat.color = updateData[outPutIndex].color;
    matrixData[outPutIndex] = outputMat;
}
