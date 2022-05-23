#include"../ShaderHeader/BasicShaderHeader.hlsli"
#include"../ShaderHeader/FogShaderHeader.hlsli"
#include"../ShaderHeader/FogBufferHeader.hlsli"

float4 PSmain(ColorOutPut input) : SV_TARGET
{
    float3 fogC = float3(0.0f, 0.0f, 0.0f);
    float4 fog = CaluFog(input.svpos, color, fogC, 0.003);
    fog.a = 1;
    return fog;
}