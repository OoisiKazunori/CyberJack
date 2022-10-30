#include"../ShaderHeader/BasicShaderHeader.hlsli"
#include"../ShaderHeader/InstanceHeader.hlsli"
#include"../ShaderHeader/MultiPassHeader.hlsli"
#include"../ShaderHeader/FogShaderHeader.hlsli"

cbuffer BloomData : register(b1)
{
    float4 luminanceColor;
    float4 fogData;
}

TwoRender PSmain(InstancePosNormalColorOut input) : SV_TARGET
{
    TwoRender output;
    float3 light = normalize(float3(-1, 1, 1));
    float diffuse = saturate(dot(-light, input.normal));
    float brightness = diffuse + 0.3f;

    float4 objColor = float4(input.color.rgb * brightness,input.color.a);
    float4 fog = CaluFog(input.svpos, objColor, fogData.xyz, fogData.w);
    fog.a = objColor.a;

    output.target0 = fog;
    output.target1 = luminanceColor;
    return output;
}