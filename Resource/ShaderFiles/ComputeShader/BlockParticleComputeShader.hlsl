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
};

//出力
RWStructuredBuffer<OutputData> matrixData : register(u0);
//更新
RWStructuredBuffer<UpdateData> updateData : register(u1);

static const int THREAD_MAX = 1;
static const float BLOCK_SIZE = 1.0f;

static const uint indexData[12][2]=
{
    {0,1},
    {0,2},
    {0,4},
    {3,2},
    {3,1},
    {3,7},
    {4,5},
    {4,6},
    {7,5},
    {7,6},
    {1,5},
    {6,2}
};


[numthreads(THREAD_MAX, THREAD_MAX, THREAD_MAX)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = (groupThreadID.y * THREAD_MAX) + groupThreadID.x + ((THREAD_MAX * THREAD_MAX) * groupThreadID.z);
    index += (THREAD_MAX * THREAD_MAX * THREAD_MAX) * groupId.x;

    float4 vertWorldPos[8];

{
    //頂点座標の行列計算-------------------------
    float4 pos = updateData[groupId.x].pos;
    matrix pMatTrans = Translate(pos.xyz);
    matrix pMatRot = Rotate(float3(0.0f,0.0f,0.0f));
    float scale = 1.0f;
    matrix pMatScale = Scale(float3(scale, scale, scale));
    
    matrix pMatWorld = MatrixIdentity();
    pMatWorld = mul(pMatScale, pMatWorld);
    pMatWorld = mul(pMatRot, pMatWorld);
    pMatWorld = mul(pMatTrans, pMatWorld);

    //頂点とワールド座標を一度行列に変換してから計算する
    for(int i = 0;i < 8; ++i)
    {
        matrix vMatWorld = MatrixIdentity();
        vMatWorld = mul(pMatScale, vMatWorld);
        vMatWorld = mul(pMatRot, vMatWorld);
        matrix vMatTrans = Translate(vertices[i].xyz);
        vMatWorld = mul(vMatTrans, vMatWorld);
        matrix tmp = mul(vMatWorld,pMatWorld);        
        vertWorldPos[i] = float4(tmp[0].w,tmp[1].w,tmp[2].w,0.0f);
    }
    //頂点座標の行列計算-------------------------
}
    
    float4 outputPos[12][10];
    for(int indexArrayNum = 0; indexArrayNum < 12; ++indexArrayNum)
    {
        uint startIndex = indexData[indexArrayNum][0];
        uint endIndex = indexData[indexArrayNum][1];
        float4 distance = vertWorldPos[endIndex] - vertWorldPos[startIndex];

        float4 perParticlePos = distance / 10.0f;
        for(int i = 0; i < 10; ++i)
        {
            //パーティクル単位の座標の書き込み
            outputPos[indexArrayNum][i] = vertWorldPos[startIndex] + perParticlePos * i;
            
            //行列計算-------------------------
            matrix pMatTrans = Translate(outputPos[indexArrayNum][i].xyz);
            matrix pMatRot = Rotate(float3(0.0f,0.0f,0.0f));
            float scale = 0.05f;
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

            uint outPutIndex = (indexArrayNum * 10 + i) + index * 120;
            matrixData[outPutIndex] = outputMat;
            //出力用-------------------------            
        }
    }
}
