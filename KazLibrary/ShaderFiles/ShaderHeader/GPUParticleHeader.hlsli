struct ColorOutPut
{
    float4 svpos : SV_POSITION;
};

cbuffer SceneConstantBuffer : register(b0)
{
    float4 velocity;
    float4 offset;
    float4 color;
    float4x4 projection;
}