#include"../ShaderHeader/BaseShaderHeader.hlsli"

static const int MAX_BONES = 32;
cbuffer skinning : register(b1)
{
    matrix matSkinning[MAX_BONES];
}

//cbuffer fogBuffer : register(b2)
//{
//    //�t�H�O��������n�߂鋗��
//    float4 start;
//    //�t�H�O���ő�ɂȂ鋗��
//    float4 end;
//    //�J�����̈ʒu�ƒ��_�̋���
//    float4 distance;
//    //�t�H�O�J���[
//    float4 fogColor;
//}

struct VSInput
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    uint4 boneIndices : BONEINDICES;
    float4 boneWeights : BONEWEIGHTS;
};

struct SkinOutPut
{
    float4 pos;
    float3 normal;
};

//struct VSOutput
//{
//    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
//    float3 normal : NORMAL; //�@���x�N�g��
//    float2 uv : TEXCOORD; //uv�l
//};


SkinOutPut ComputeSkin(VSInput input)
{
    SkinOutPut output = (SkinOutPut) 0;
    
    //�v�Z����{�[���ԍ�
    uint iBone;
    //�{�[���E�F�C�g
    float weight;
    //�X�L�j���O�s��
    matrix m;
    
    //�{�[��0,1
    iBone = input.boneIndices.x;
    weight = input.boneWeights.x;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    iBone = input.boneIndices.y;
    weight = input.boneWeights.y;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    //�{�[��2,3
    iBone = input.boneIndices.z;
    weight = input.boneWeights.z;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    iBone = input.boneIndices.w;
    weight = input.boneWeights.w;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    return output;
}


