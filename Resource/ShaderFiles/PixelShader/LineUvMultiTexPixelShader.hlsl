#include"../ShaderHeader/SpriteShaderHeader.hlsli"
#include"../ShaderHeader/MultiPassHeader.hlsli"

cbuffer lineData : register(b1)
{
    float4 distance;
    float4 hitColor;
}

TwoRender PSmain(VSOutput input)
{
    float4 outputColor;
    if (input.uv.y < distance.x)
    {
        outputColor = hitColor;
    }
    else
    {
        outputColor = color;
    }

    TwoRender output;
    output.target0 = outputColor;
    output.target1 = float4(0.0f, 0.0f, 0.0f, 1.0f);
    return output;
}