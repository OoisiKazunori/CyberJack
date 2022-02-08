#include"../ShaderHeader/BasicShaderHeader.hlsli"
#include"../ShaderHeader/InstanceHeader.hlsli"

InstanceOutPut VSmain(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint id : SV_InstanceID)
{
    InstanceOutPut op;
    op.svpos = mul(matrixData[id].matData, pos);
    op.normal = normal;
    op.uv = uv;
    op.id = id;
    return op;
}