#include"../ShaderHeader/GPUParticleHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

TwoRender PSmain(PosUvOutput input) : SV_TARGET
{
    TwoRender output;
    output.target0 = float4(tex.Sample(smp, input.uv));
    output.target0.xyz += matrixData[input.id].color.xyz;
    output.target1 = float4(1.0f,0.0f,0.0f,1.0f);

    return output;
}