#include"../ShaderHeader/GPUParticleHeader.hlsli"

ColorOutPut VSmain(float4 pos : POSITION,uint id : SV_INSTANCEID)
{
    ColorOutPut op;
    op.svpos = mul(matrixData[id].mat, pos);
    op.id = id;
    return op;
}