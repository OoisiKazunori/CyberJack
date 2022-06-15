//#include"../ShaderHeader/BasicShaderHeader.hlsli"

struct ColorOutPut
{
    float4 svpos : SV_POSITION;
};

cbuffer SceneConstantBuffer : register(b0)
{
    float4 velocity;
    float4 offset;
    float4 color;
    float4x4 projection;
}

ColorOutPut VSmain(float4 pos : POSITION)
{
    ColorOutPut op;
	//op.svpos = mul(world, pos);   // ���f���̒��_�����[���h���W�n�ɕϊ�
	//op.svpos = mul(view, op.svpos);    // ���[���h���W�n����J�������W�n�ɕϊ�
	//op.svpos = mul(viewproj, op.svpos);    // �J�������W�n����X�N���[�����W�n�ɕϊ�
    op.svpos = mul(pos + offset, projection);
    //op.svpos = pos;
	
    return op;
}