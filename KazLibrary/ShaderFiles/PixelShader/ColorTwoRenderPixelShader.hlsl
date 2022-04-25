#include"../ShaderHeader/BasicShaderHeader.hlsli"
#include"../ShaderHeader/MultiPassHeader.hlsli"

TwoRender PSmain(ColorOutPut input) : SV_TARGET
{
    TwoRender output;
    output.target0 = color;
    output.target1 = float4(0.0f, 0.0f, 0.0f, 0.0f);
    return output;
}