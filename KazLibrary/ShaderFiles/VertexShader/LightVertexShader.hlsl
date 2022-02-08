#include"../ShaderHeader/BasicShaderHeader.hlsli"
#include"../ShaderHeader/LightHeader.hlsli"

LightVSOutput VSmain(float4 pos : POSITION, float3 normal : NORMAL)
{
	LightVSOutput outp;

	outp.svpos = mul(world, pos);   // ���f���̒��_�����[���h���W�n�ɕϊ�
	outp.worldPos = outp.svpos;

	outp.svpos = mul(view, outp.svpos);    // ���[���h���W�n����J�������W�n�ɕϊ�
	outp.svpos = mul(viewproj, outp.svpos);    // �J�������W�n����X�N���[�����W�n�ɕϊ�	
	outp.normal = mul(world, normal); // �@������]������

	return outp;
}