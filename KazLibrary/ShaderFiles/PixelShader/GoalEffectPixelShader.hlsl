#include"../ShaderHeader/SpriteShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

cbuffer uvBuffer: register(b1)
{
    float4 uv; //色
}


float4 PSmain(VSOutput input) : SV_TARGET
{
    float2 uvValue = input.uv + uv.xy;
    float4 output = float4(tex.Sample(smp, uvValue));
    
    //テクスチャの下の部分を見えなくする
    output.a = input.uv.y;
    if (color.a < output.a)
    {
        output.a = color.a;
    }
    return output;
}