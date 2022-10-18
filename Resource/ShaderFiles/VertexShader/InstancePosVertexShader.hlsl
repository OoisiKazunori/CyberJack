#include"../ShaderHeader/BasicShaderHeader.hlsli"
#include"../ShaderHeader/InstanceHeader.hlsli"

InstancePosOut VSmain(float4 pos : POSITION, uint id : SV_InstanceID)
{
    InstancePosOut op;
    op.svpos = mul(uavMatrixData[id].mat, pos);
    op.id = id;
    return op;
}