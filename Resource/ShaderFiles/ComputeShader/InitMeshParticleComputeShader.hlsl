#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/ExcuteIndirectHeader.hlsli"

cbuffer RootConstants : register(b0)
{
    float4 pos;
    uint vertMaxNum;
    uint indexMaxNum;
};

//更新
RWStructuredBuffer<float4> vertciesData : register(u0);
RWStructuredBuffer<uint> indexData : register(u1);
RWStructuredBuffer<uint> indexOffset : register(u2);
//出力
AppendStructuredBuffer<float4> worldPosData : register(u3);

float4 GetPos(float3 VERT_POS,float3 WORLD_POS)
{
    float3 defaltScale = float3(1,1,1);
    float3 defaltRota = float3(0,0,0);

    matrix pMatWorld = CalucurateWorldMat(WORLD_POS,defaltScale,defaltRota);
    matrix vertMatWorld = CalucurateWorldMat(VERT_POS,defaltScale,defaltRota);

    matrix worldMat = mul(vertMatWorld,pMatWorld);

    return float4(worldMat[0].w,worldMat[1].w,worldMat[2].w,0.0f);
};

[numthreads(1, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = (groupThreadID.y * 1204) + groupThreadID.x + groupThreadID.z;
    index += 1024 * groupId.x;

    //頂点数以内なら処理する
    if(indexMaxNum < indexOffset[0] + index + 2)
    {
        return;
    }

    //三角形を構成するインデックスの指定--------------------------------------------
    uint firstVertIndex = indexData[indexOffset[0] + index];
    uint secondVertIndex = indexData[indexOffset[0] + index + 1];
    uint thirdVertIndex = indexData[indexOffset[0] + index + 2];
    indexOffset[0] += 2;
    //三角形を構成するインデックスの指定--------------------------------------------

    //頂点座標からワールド座標に変換後----------------------------------------------
    float4 firstVertWorldPos = GetPos(vertciesData[firstVertIndex].xyz,pos.xyz);
    float4 secondVertWorldPos = GetPos(vertciesData[secondVertIndex].xyz,pos.xyz);
    float4 thirdVertWorldPos = GetPos(vertciesData[secondVertIndex].xyz,pos.xyz);
    //頂点座標からワールド座標に変換------------------------------------------------

    //三角形を構成するレイ--------------------------------------------
    const int RAY_MAX_NUM = 3;
    const int RAY_POS_MAX_NUM = 2;
    float3 triangleRay[RAY_MAX_NUM][RAY_POS_MAX_NUM];
    triangleRay[0][0] = firstVertWorldPos.xyz;
    triangleRay[0][1] = secondVertWorldPos.xyz;
    triangleRay[1][0] = secondVertWorldPos.xyz;
    triangleRay[1][1] = thirdVertWorldPos.xyz;
    triangleRay[2][0] = thirdVertWorldPos.xyz;
    triangleRay[2][1] = firstVertWorldPos.xyz;
    //三角形を構成するレイ--------------------------------------------

    //重心座標
    float3 triangleCentralPos;
    triangleCentralPos += firstVertWorldPos.xyz;
    triangleCentralPos += secondVertWorldPos.xyz;
    triangleCentralPos += thirdVertWorldPos.xyz;
    triangleCentralPos /= 3.0f;
 
    //パーティクルの配置--------------------------------------------
    const int PARTICLE_MAX_NUM = 10 / 3;
    for(int rayIndex = 0; rayIndex < RAY_MAX_NUM; ++rayIndex)
    {
        //一辺の長さ
        float3 distance = triangleRay[rayIndex][1] - triangleRay[rayIndex][0];

        for(int particleIndex = 0; particleIndex < PARTICLE_MAX_NUM; ++particleIndex)
        {
            //始点算出
            float3 startPos = triangleRay[rayIndex][0] + distance * (float(RandVec3(index,100,0).x) / 100);
            //終点算出
            float3 endPos = triangleCentralPos;
            //一辺のとある位置から重心座標までの距離
            float3 resultDistance = endPos - startPos;

            //パーティクルの配置
            float4 resultPos;
            const int PARTICLE_MAX_BIAS = 100;
            const int RANDOM_NUMBER_BIAS = 80;
            if(RandVec3(index,PARTICLE_MAX_BIAS,0).x <= RANDOM_NUMBER_BIAS)
            {
                //エッジ周辺に偏らせる
                resultPos = float4(triangleRay[rayIndex][0] + resultDistance * (float(RandVec3(index,10,0).x) / 100),0.0f);
                worldPosData.Append(resultPos);
            }
            else
            {
                //面周辺に偏らせる
                resultPos = float4(triangleRay[rayIndex][0] + resultDistance * (float(RandVec3(index,100,0).x) / 100),0.0f);
                worldPosData.Append(resultPos);
            }
        }
    }
    //パーティクルの配置--------------------------------------------
}
