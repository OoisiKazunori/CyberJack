#include"../ShaderHeader/BaseShaderHeader.hlsli"
#include"../ShaderHeader/FogShaderHeader.hlsli"
#include"../ShaderHeader/FogBufferHeader.hlsli"
#include"../ShaderHeader/MultiPassHeader.hlsli"

TwoRender PSmain(FogOutPut input) : SV_TARGET
{
    float4 fog = CaluFog(input.svpos, color, fogData.xyz, fogData.w);
    
    TwoRender output;
    output.target0 = fog;
    output.target1 = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    return output;
}