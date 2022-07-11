#include"../ShaderHeader/GPUParticleHeader.hlsli"

float4 PSmain(ColorOutPut input) : SV_TARGET
{
    uint id = 5;
    return matrixData[id].color;
}