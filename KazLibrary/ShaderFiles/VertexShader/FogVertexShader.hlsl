#include"../ShaderHeader/FogShaderHeader.hlsli"
#include"../ShaderHeader/BaseShaderHeader.hlsli"
#include"../ShaderHeader/FogBufferHeader.hlsli"

FogOutPut VSmain(float4 pos : POSITION)
{
    FogOutPut op;
    op.svpos = mul(world, pos); // ���f���̒��_�����[���h���W�n�ɕϊ�
    op.fogFactor = op.svpos;
    op.svpos = mul(view, op.svpos); // ���[���h���W�n����J�������W�n�ɕϊ�
    op.svpos = mul(viewproj, op.svpos); // �J�������W�n����X�N���[�����W�n�ɕϊ�
    return op;
}