#include"../ShaderHeader/BaseShaderHeader.hlsli"
#include"../ShaderHeader/FogShaderHeader.hlsli"
#include"../ShaderHeader/FogBufferHeader.hlsli"

float4 PSmain(FogOutPut input) : SV_TARGET
{
    ////�w���t�H�O    
    //float d = input.svpos.z * input.svpos.w;
    ////���x
    //float density = 0.01;
    //float e = 2.71828f;
    //float fogFactor = pow(e, -d * density);
    ////�t�H�O��
    //fogFactor *= 1;
    //fogFactor = saturate(fogFactor);
    
    ////�t�H�O�̐F  
    ////color...�t�H�O��K������O�̃J���[
    //float4 FogColor = float4(fogColor.x, fogColor.y, fogColor.z, 1.0);    
    ////float4 fog = input.fogFactor * color + (1 - input.fogFactor) * FogColor;
    //float4 fog = fogFactor * color + (1 - fogFactor) * FogColor;
    
    
    ////�������Đ^�����ɂȂ�Ȃ��悤�ɂ���
    //if (FogColor.x <= fog.x&&
    //    FogColor.y <= fog.y&&
    //    FogColor.z <= fog.z)
    //{
    //    return FogColor;
    //} 
    float4 fog = CaluFog(input.svpos, color, fogData.xyz, fogData.w);
    
    
    return fog;    
}