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
    //r...����,g...�S�̃t���b�V��
    float2 flash;
};

//�X�V
RWStructuredBuffer<UpdateData> updateData : register(u0);
//�o��
RWStructuredBuffer<OutputData> matrixData : register(u1);

static const int THREAD_MAX = 10;

static const float4 FLASH_COLOR = float4(1.0f,1.0f,1.0f,1.0f);
static const float4 NORMAL_COLOR = float4(0.6f,0.6f,0.6f,0.6f);


[numthreads(THREAD_MAX, THREAD_MAX, THREAD_MAX)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = (groupThreadID.y * THREAD_MAX) + groupThreadID.x + ((THREAD_MAX * THREAD_MAX) * groupThreadID.z);
    index += (THREAD_MAX * THREAD_MAX * THREAD_MAX) * groupId.x;

    uint outPutIndex = index;

    //�ړ�--------------------------------------------  
    updateData[outPutIndex].pos.z -= 1.0f;
    if(updateData[outPutIndex].pos.z <= -150.0f)
    {
        updateData[outPutIndex].pos.z = 1800.0f;
    }
    //�ړ�--------------------------------------------

    //�F�o��
    bool TopFlag = updateData[outPutIndex].pos.y <= flash.r + 2.0f;
    bool ButtonFlag = flash.r - 2.0f <= updateData[outPutIndex].pos.y;
    //�����ɂ��F
    if(TopFlag && ButtonFlag)
    {
        updateData[outPutIndex].color = FLASH_COLOR;
    }
    if(flash.g)
    {
        updateData[outPutIndex].color = FLASH_COLOR;
    }
    //�i�X�ʏ�̐F�ɖ߂�
    if( NORMAL_COLOR.r < updateData[outPutIndex].color.r  &&
        NORMAL_COLOR.g < updateData[outPutIndex].color.g &&
        NORMAL_COLOR.b < updateData[outPutIndex].color.b &&
        NORMAL_COLOR.a < updateData[outPutIndex].color.a)
    {
        updateData[outPutIndex].color -= 0.01f;
    }
    //�F�o��


    //�s��v�Z-------------------------
    matrix pMatTrans = Translate(updateData[outPutIndex].pos.xyz);
    matrix pMatRot = Rotate(float3(0.0f,0.0f,0.0f));
    float scale = 1.0f;
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
