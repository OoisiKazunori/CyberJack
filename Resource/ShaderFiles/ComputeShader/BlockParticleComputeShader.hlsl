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
};

cbuffer RootConstants : register(b0)
{
    matrix view;        
    matrix projection;
    matrix billBoard;
    float4 vertices[8];
    unsigned int index0[2];
	unsigned int index1[2];
	unsigned int index2[2];
	unsigned int index3[2];
	unsigned int index4[2];
	unsigned int index5[2];
	unsigned int index6[2];
	unsigned int index7[2];
	unsigned int index8[2];
	unsigned int index9[2];
	unsigned int index10[2];
	unsigned int index11[2];
};

//出力
RWStructuredBuffer<OutputData> matrixData : register(u0);
//更新
RWStructuredBuffer<UpdateData> updateData : register(u1);

static const int THREAD_MAX = 5;
static const float BLOCK_SIZE = 1.0f;

[numthreads(THREAD_MAX, THREAD_MAX, THREAD_MAX)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = (groupThreadID.y * THREAD_MAX) + groupThreadID.x + ((THREAD_MAX * THREAD_MAX) * groupThreadID.z);
    index += (THREAD_MAX * THREAD_MAX * THREAD_MAX) * groupId.x;

    

    float4 vertWorldPos[8];

    //頂点座標の行列計算-------------------------
    matrix pMatTrans = Translate(updateData[index].pos.xyz);
    matrix pMatRot = Rotate(float3(0.0f,0.0f,0.0f));
    float scale = 1.0f;
    matrix pMatScale = Scale(float3(scale, scale, scale));
    
    matrix pMatWorld = MatrixIdentity();
    pMatWorld = mul(pMatScale, pMatWorld);
    pMatWorld = mul(pMatRot, pMatWorld);
    pMatWorld = mul(billBoard, pMatWorld);
    pMatWorld = mul(pMatTrans, pMatWorld);
    for(int i = 0;i < 8; ++i)
    {        
        vertWorldPos[i] = mul(pMatWorld,vertices[i]);
    }
    //頂点座標の行列計算-------------------------


    uint indexData[12][2];
    indexData[0][0] = 0;
    indexData[0][1] = 1;

    indexData[1][0] = 0;
    indexData[1][1] = 2;

    indexData[2][0] = 0;
    indexData[2][1] = 4;

    indexData[3][0] = 3;
    indexData[3][1] = 2;

    indexData[4][0] = 3;
    indexData[4][1] = 1;
    
    indexData[5][0] = 3;
    indexData[5][1] = 7;
    
    indexData[6][0] = 4;
    indexData[6][1] = 5;
    
    indexData[7][0] = 4;
    indexData[7][1] = 6;
    
    indexData[8][0] = 7;
    indexData[8][1] = 5;
    
    indexData[9][0] = 7;
    indexData[9][1] = 6;
    
    indexData[10][0] = 0;
    indexData[10][1] = 5;

    indexData[11][0] = 6;
    indexData[11][1] = 2;



    //現在のブロックのインデックス
    uint blockIndex = 0;
    //現在のパーティクルのインデックス
    uint particleIndex = 0;
    //現在見ている辺
    uint sideIndex = 0;
    //辺内の割合
    float sideRate = 0.0f;
    
    uint startIndex = indexData[sideIndex][0];
    uint endIndex = indexData[sideIndex][1];
    float4 distance = vertWorldPos[endIndex] - vertWorldPos[startIndex];
    float4 perParticlePos = distance / 10.0f;
    float4 particlePos = vertWorldPos[startIndex] + perParticlePos * sideRate;


    //行列計算-------------------------
    matrix pMatTrans = Translate(particlePos.xyz);
    matrix pMatRot = Rotate(float3(0.0f,0.0f,0.0f));
    float scale = 0.1f;
    matrix pMatScale = Scale(float3(scale, scale, scale));
    
    matrix pMatWorld = MatrixIdentity();
    pMatWorld = mul(pMatScale, pMatWorld);
    pMatWorld = mul(pMatRot, pMatWorld);
    pMatWorld = mul(billBoard, pMatWorld);
    pMatWorld = mul(pMatTrans, pMatWorld);
    //行列計算-------------------------
    
        
    //出力用-------------------------
    OutputData outputMat;
    matrix lView = view;
    matrix lproj = projection;

    outputMat.mat = MatrixIdentity();
    outputMat.mat = mul(pMatWorld,outputMat.mat);
    outputMat.mat = mul(lView,    outputMat.mat);
    outputMat.mat = mul(lproj,    outputMat.mat);
    outputMat.color = float4(1.0f,0.0f,0.0f,1.0f); 
    matrixData[blockIndex * particleIndex] = outputMat;
    //出力用-------------------------
}