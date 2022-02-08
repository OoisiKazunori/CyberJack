#include"../ShaderHeader/SpriteShaderHeader.hlsli"

Texture2D<float4>tex:register(t0);
SamplerState smp :register(s0);

float Rand(float2 coord, float seed)
{
    return frac(sin(dot(coord.xy, float2(12.9898, 78.233)) + seed) * 43758.5453);
}

cbuffer time : register(b1)
{
    float Time;
}


float4 PSmain(VSOutput input) :SV_TARGET
{
	float4 test = float4(tex.Sample(smp, input.uv));
    
    if (color.a < test.a)
    {
        test.a = color.a; 
    }
    
	return test;
}