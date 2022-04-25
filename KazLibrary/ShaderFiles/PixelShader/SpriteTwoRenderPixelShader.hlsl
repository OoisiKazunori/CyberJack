#include"../ShaderHeader/SpriteShaderHeader.hlsli"
#include"../ShaderHeader/MultiPassHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

TwoRender PSmain(VSOutput input) : SV_TARGET
{
    TwoRender output;
    
    output.target0 = float4(tex.Sample(smp, input.uv));
    if (color.a < output.target0.a)
    {
        output.target0.a = color.a;
    }
    output.target1 = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    return output;
}