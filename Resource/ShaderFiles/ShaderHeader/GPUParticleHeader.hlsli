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
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float4 color : COLOR; //uv�l
};

RWStructuredBuffer<OutputData> matrixData : register(u0);