#include"../ShaderHeader/BaseShaderHeader.hlsli"

static const int MAX_BONES = 32;
cbuffer skinning : register(b1)
{
    matrix matSkinning[MAX_BONES];
}

//cbuffer fogBuffer : register(b2)
//{
//    //フォグがかかり始める距離
//    float4 start;
//    //フォグが最大になる距離
//    float4 end;
//    //カメラの位置と頂点の距離
//    float4 distance;
//    //フォグカラー
//    float4 fogColor;
//}

struct VSInput
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    uint4 boneIndices : BONEINDICES;
    float4 boneWeights : BONEWEIGHTS;
};

struct SkinOutPut
{
    float4 pos;
    float3 normal;
};

//struct VSOutput
//{
//    float4 svpos : SV_POSITION; //システム用頂点座標
//    float3 normal : NORMAL; //法線ベクトル
//    float2 uv : TEXCOORD; //uv値
//};


SkinOutPut ComputeSkin(VSInput input)
{
    SkinOutPut output = (SkinOutPut) 0;
    
    //計算するボーン番号
    uint iBone;
    //ボーンウェイト
    float weight;
    //スキニング行列
    matrix m;
    
    //ボーン0,1
    iBone = input.boneIndices.x;
    weight = input.boneWeights.x;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    iBone = input.boneIndices.y;
    weight = input.boneWeights.y;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    //ボーン2,3
    iBone = input.boneIndices.z;
    weight = input.boneWeights.z;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    iBone = input.boneIndices.w;
    weight = input.boneWeights.w;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    return output;
}


