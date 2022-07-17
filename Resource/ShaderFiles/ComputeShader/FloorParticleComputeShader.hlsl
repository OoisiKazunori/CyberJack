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
    float4 vel;
    float4 color;
    uint timer;
};

cbuffer RootConstants : register(b0)
{
    matrix view;        
    matrix projection;
    matrix billBoard;
    float4 emmittPos;
    uint increSize;     //行列と色を出力する構造体のサイズ
    uint64_t gpuAddress; //行列と色を出力するバッファのGPUアドレス
    int seed;
};

//出力
AppendStructuredBuffer<OutputData> matrixData : register(u0);
//更新
RWStructuredBuffer<UpdateData> updateData : register(u1);

static const int THREAD_MAX = 10;

[numthreads(THREAD_MAX, THREAD_MAX, THREAD_MAX)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    //uint index = (groupId.x * THREAD_MAX) + groupIndex;
    uint index = (groupThreadID.y * THREAD_MAX) + groupThreadID.x + ((THREAD_MAX * THREAD_MAX) * groupThreadID.z);
    index += (THREAD_MAX * THREAD_MAX * THREAD_MAX) * groupId.x;
    //uint index = 9000;

    float4 initPos = emmittPos;

    float maxNum = 100000;
    float indexf = index;
    float cRate = indexf / maxNum;
    //生成するパーティクルの判断-------------------------
    //移動量と生存時間の初期化
    if(updateData[index].timer <= 0)
    {
        updateData[index].timer = 1;


        float3 seeds1 = float3(
            seed * (groupId.x + groupThreadID.x),
            seed - groupThreadID.y,
            seed % groupId.x
        );
        float3 seeds2 = float3(
            index - (groupThreadID.x * groupId.x),
            index * groupId.x,
            index % (groupThreadID.z + groupId.x)
        );

        float4 generatePos = initPos + 
        float4
        (
            Rand(seeds1.x,seeds2.x,500,-500),
            Rand(seeds1.y,seeds2.y,0,-300),
            Rand(seeds1.z,seeds2.z,500,-50),
            0.0f
        );
        updateData[index].pos = generatePos;
        updateData[index].color = float4(Rand(seed,index,1,0),Rand(seed/2,index,1,0),Rand(seed*2,index,1,0),1.0f);
        updateData[index].vel = float4(0.0f,0.0f,-Rand(seed,index,1,0),0.0f);
        //updateData[index].color = float4(cRate,1.0f,1.0f,1.0f);
    }
    else
    {
        updateData[index].pos += updateData[index].vel;

        if(updateData[index].pos.z <= -100.0f)
        {
            updateData[index].pos.z = 500.0f;
        }
    }
    //生成するパーティクルの判断-------------------------





    //行列計算-------------------------
    matrix pMatTrans = Translate(updateData[index].pos.xyz);
    matrix pMatRot = Rotate(float3(0.0f,0.0f,0.0f));
    float scale = 1.0f;
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
    outputMat.color = updateData[index].color;
    matrixData.Append(outputMat);    
    //出力用-------------------------

}