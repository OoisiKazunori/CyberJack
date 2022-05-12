#include"../ShaderHeader/SpriteShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 PSmain(VSOutput input) : SV_TARGET
{
    float4 output = float4(tex.Sample(smp, input.uv));
    if (color.a < output.a)
    {
        //output.a = color.a;
    }
    
    float3 blue = float3(0.24f, 0.09f, 0.62f);
    float3 oragnge = float3(0.93f, 0.65f, 0.53f);
    float3 sub = oragnge - blue;
    sub *= float3(input.uv.y, input.uv.y, input.uv.y);
    return float4(blue.x + sub.x, blue.y + sub.y, blue.z + sub.z, 1.0f);
}