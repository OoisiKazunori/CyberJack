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

struct PosColorOutput
{
    float4 svpos : SV_POSITION; //システム用頂点座標
    float4 color : COLOR;
};

RWStructuredBuffer<OutputData> matrixData : register(u0);