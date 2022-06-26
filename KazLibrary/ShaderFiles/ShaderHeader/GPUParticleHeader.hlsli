struct ColorOutPut
{
    float4 svpos : SV_POSITION;
};

cbuffer OutputData : register(b0)
{
    matrix mat;
    float4 color;
}