#include"../ShaderHeader/FbxShaderHeader.hlsli"

struct TwoRender
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

TwoRender PSmain(VSOutput input) : SV_TARGET
{
    float3 light = normalize(float3(1, -1, 1));
    float diffuse = saturate(dot(-light, input.normal));
    float brightness = diffuse + 0.3f;

    float4 texColor = float4(tex.Sample(smp, input.uv));
    
    TwoRender outPut;
    outPut.target0 = texColor * float4(brightness, brightness, brightness, 1);
    outPut.target1 = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    return outPut;
}