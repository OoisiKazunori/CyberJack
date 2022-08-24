#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/ExcuteIndirectHeader.hlsli"

struct OutputData
{
    matrix mat;
    float4 color;
};

struct UpdateData
{
    float4 pos;
    float4 vertices;
    float verticesMaxNum;
};

cbuffer RootConstants : register(b0)
{
    matrix view;
    matrix projection;
    matrix billBoard;
    float4 vertices[1];
    uint indexMaxNum;
	uint indexData[1];
};

//�o��
RWStructuredBuffer<OutputData> matrixData : register(u0);
//�X�V
RWStructuredBuffer<UpdateData> updateData : register(u1);

static const int THREAD_MAX = 10;

[numthreads(THREAD_MAX, THREAD_MAX, THREAD_MAX)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = (groupThreadID.y * THREAD_MAX) + groupThreadID.x + ((THREAD_MAX * THREAD_MAX) * groupThreadID.z);
    index += (THREAD_MAX * THREAD_MAX * THREAD_MAX) * groupId.x;


    //���_���ȓ��Ȃ珈������
    if(index < 1 && index + 1 < 1)
    {
    uint nowVertIndex = indexData[index];
    uint nextVertIndex = indexData[index + 1];

    bool firstIndexFlag = index - 1 <= -1;

    //���[���h���W�̍s��v�Z-------------------------
    matrix pMatWorld = CalucurateWorldMat(updateData[0].pos.xyz,float3(0,0,0),float3(1.0f,1.0f,1.0f));
    //���[���h���W�̍s��v�Z-------------------------

    //���_���W���烏�[���h���W�ɕϊ���--------------------------------------
    matrix prevVertMatWorld = MatrixIdentity();
    float3 vertPos = float3(0,0,0);
    if(!firstIndexFlag)
    {
        vertPos = vertices[index - 1].xyz;
        prevVertMatWorld = CalucurateWorldMat(vertPos,float3(0,0,0),float3(1.0f,1.0f,1.0f));
    }
    vertPos = vertices[index].xyz;
    matrix nowVertMatWorld = CalucurateWorldMat(vertPos,float3(0,0,0),float3(1.0f,1.0f,1.0f));
    vertPos = vertices[index + 1].xyz;
    matrix nextVertMatWorld = CalucurateWorldMat(vertPos,float3(0,0,0),float3(1.0f,1.0f,1.0f));

    matrix prevWorldMat = mul(prevVertMatWorld,pMatWorld);
    matrix nowWorldMat = mul(nowVertMatWorld,pMatWorld);
    matrix nextWorldMat = mul(nextVertMatWorld,pMatWorld);

    float4 prevVertWorldPos = float4(prevWorldMat[0].w,prevWorldMat[1].w,prevWorldMat[2].w,0.0f);
    float4 nowVertWorldPos = float4(nowWorldMat[0].w,nowWorldMat[1].w,nowWorldMat[2].w,0.0f);
    float4 nextVertWorldPos = float4(nextWorldMat[0].w,nextWorldMat[1].w,nextWorldMat[2].w,0.0f);
    //���_���W���烏�[���h���W�ɕϊ�--------------------------------------


    //���݂��̋��������Ԃ�������--------------------------------------
    float4 distance = nextVertWorldPos - nowVertWorldPos;
    float4 prevDistance = nowVertWorldPos - prevVertWorldPos;
    int absoluteValueOfDistance = CaluDistacne(nowVertWorldPos.xyz, nextVertWorldPos.xyz);
    int absoluteValueOfPrevDistance = 0;
    if(!firstIndexFlag)
    {
        absoluteValueOfPrevDistance = CaluDistacne(prevVertWorldPos.xyz, nowVertWorldPos.xyz);
    }
    
    for(int i = 0; i < absoluteValueOfDistance; ++i)
    {           
        //�o�͗p-------------------------
        float4 pos = nowVertWorldPos + distance * (i / absoluteValueOfDistance);
        matrix rMatTrans = CalucurateWorldMat(pos.xyz,float3(0,0,0),float3(1.0f,1.0f,1.0f), billBoard);

        OutputData outputMat;
        outputMat.mat = CalucurateMat(rMatTrans, view, projection);
        outputMat.color = float4(1.0f,0.0f,0.0f,1.0f); 
        uint resultIndex = index * absoluteValueOfPrevDistance;
        matrixData[resultIndex] = outputMat;
        //�o�͗p-------------------------
    }
    //���݂��̋��������Ԃ�������--------------------------------------
    }
}
