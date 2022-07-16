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
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float2 uv : TEXCOORD; //uv�l
    uint id : SV_INSTANCEID;
};

RWStructuredBuffer<OutputData> matrixData : register(u0);