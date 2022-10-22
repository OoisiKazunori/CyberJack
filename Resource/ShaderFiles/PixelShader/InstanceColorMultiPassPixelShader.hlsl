#include"../ShaderHeader/BasicShaderHeader.hlsli"
#include"../ShaderHeader/InstanceHeader.hlsli"
#include"../ShaderHeader/MultiPassHeader.hlsli"

cbuffer BloomData : register(b1)
{
    float4 luminanceColor;
}

TwoRender PSmain(InstancePosOut input) : SV_TARGET
{
    TwoRender output;
    output.target0 = uavMatColorData[input.id].color;
    output.target1 = luminanceColor;
    return output;
}