struct ColorOutPut
{
    float4 svpos : SV_POSITION;
};

cbuffer SceneConstantBuffer : register(b0)
{
    matrix mat;
    float4 color;
}