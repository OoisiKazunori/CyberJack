#include"../ShaderHeader/MultipassHeader.hlsli"

struct ColorOutPut
{
    float4 svpos : SV_POSITION;
    uint id : SV_INSTANCEID;
};

struct OutputData
{
    matrix mat;
    float4 color;
};

struct PosUvOutput
{
    float4 svpos : SV_POSITION; //システム用頂点座標
    float2 uv : TEXCOORD; //uv値
    uint id : SV_INSTANCEID;
};

RWStructuredBuffer<OutputData> matrixData : register(u0);