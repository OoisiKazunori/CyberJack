#include"../ShaderHeader/GPUParticleHeader.hlsli"

TwoRender PSmain(PosUvOutput input) : SV_TARGET
{
    TwoRender output;
    output.target0 = matrixData[input.id].color;
    output.target1 = float4(1.0f,0.0f,0.0f,1.0f);

    output.target0.rgb = float3(0.5f,0.5f,0.5f);
    output.target0.a = 0.6f;
    return output;
}