#include"../ShaderHeader/BasicShaderHeader.hlsli"
#include"../ShaderHeader/InstanceHeader.hlsli"
#include"../ShaderHeader/MultiPassHeader.hlsli"
#include"../ShaderHeader/FogShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> maskColor : register(t1);
SamplerState smp : register(s0);

cbuffer FogData : register(b0)
{
    float4 fogData;
    float2 uvOffset;
}

TwoRender PSmain(InstanceOutPut input) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, input.uv);

    TwoRender op;
    op.target0 = CaluFog(input.svpos, texColor, fogData.xyz, fogData.w); 

    if(0.8f <= texColor.x && 0.8f <= texColor.y && 0.8f <= texColor.z)
    {
        op.target1 = float4(1.0f - (input.worldPos.z / 4000.0f),0,0,1.0f - (input.worldPos.z / 4000.0f));
    }
    else
    {
        op.target1 = float4(0,0,0,0);
    }

    return op;
}