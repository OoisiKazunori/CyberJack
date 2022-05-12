#include"../ShaderHeader/BaseShaderHeader.hlsli"
#include"../ShaderHeader/FogShaderHeader.hlsli"
//#include"../ShaderHeader/FogBufferHeader.hlsli"
#include"../ShaderHeader/MultiPassHeader.hlsli"

cbuffer fogBuffer : register(b1)
{
    //フォグカラー
    float3 endColor;
    float3 subValue;
    //x...RataMin,y...RateMax,z...StartFogDepth,w...EndFogDepth
    float4 rateAndFogLine;
}

TwoRender PSmain(FogOutPut input)
{
    float3 fog = CalucurateFogValue(input.fogFactor, endColor, subValue, color.xyz, rateAndFogLine.x, rateAndFogLine.y, rateAndFogLine.z, rateAndFogLine.w);
    
    float4 result = float4(fog.xyz, color.a);
    TwoRender output;
    output.target0 = result;
    output.target1 = float4(0.0f, 0.0f, 0.0f, 0.0f);
    return output;
}