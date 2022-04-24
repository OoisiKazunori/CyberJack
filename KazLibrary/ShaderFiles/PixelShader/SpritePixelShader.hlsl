#include"../ShaderHeader/SpriteShaderHeader.hlsli"

Texture2D<float4>tex:register(t0);
SamplerState smp :register(s0);

float4 PSmain(VSOutput input) :SV_TARGET
{
	float4 test = float4(tex.Sample(smp, input.uv));
    
    if (color.a < test.a)
    {
        test.a = color.a; 
    }
    
	return test;
}