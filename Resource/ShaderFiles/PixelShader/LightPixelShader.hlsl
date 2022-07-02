#include"../ShaderHeader/BasicShaderHeader.hlsli"
#include"../ShaderHeader/LightHeader.hlsli"

float4 PSmain(LightVSOutput input) : SV_TARGET
{

    // �f�B���N�V�������C�g�ɂ��Lambert�g�U���ˌ���Phong���ʔ��ˌ����v�Z����
    float3 diffDirection = CalcLambertDiffuse(dirDirection, dirColor, input.normal);
    float3 specDirection = CalcPhongSpecular(dirDirection, dirColor, input.worldPos, input.normal);

    // �|�C���g���C�g�ɂ��Lambert�g�U���ˌ���Phong���ʔ��ˌ����v�Z����
    //�T�[�t�F�C�X�ɓ��˂���|�C���g���C�g�̌��̌������v�Z����
    float3 ligDir = input.worldPos - ptPosition;
    ligDir = normalize(ligDir);

    //�����Ȃ���Lambert�g�U���ˌ����v�Z����
    float3 diffPoint = CalcLambertDiffuse 
    (
        ligDir,
        ptColor,
        input.normal
    );

    //�����Ȃ���Phong���ʔ��ˌ����v�Z����
    float3 specPoint = CalcPhongSpecular
    (
        ligDir,
        ptColor,
        input.worldPos,
        input.normal
    );



    //�����ɂ��e�������v�Z����
    float3 distance = length(input.worldPos - ptPosition); 
    float affect = 1.0f - 1.0f / ptRange * distance;

    if (affect < 0.0f)
    {
        affect = 0.0f;
    }
    affect = pow(affect, 3.0);



    //�g�U���ˌ��Ƌ��ʔ��ˌ��ɉe��������Z���ĉe������߂�    
    diffPoint *= affect;        
    specPoint *= affect;

    //�ŏI�I�Ȕ��ˌ������߂�
    //�f�B���N�V�������C�g�����������������炱���ő���
    float3 diffuseLig = diffPoint + diffDirection;
    float3 specularLig = specPoint + specDirection;
    //float3 diffuseLig = diffPoint;
    //float3 specularLig = specPoint;

    //�g�U���ˌ��Ƌ��ʔ��ˌ��𑫂��Z���āA�ŏI�I�Ȍ������߂�
    float3 lig = diffuseLig + specularLig + ambientLight;
    float4 finalColor = color;


    //color�ɋ��߂�������Z���čŏI�o�̓J���[�����߂�
    finalColor.xyz *= lig;

    return finalColor;
}



//float4 SpotLightPSmain(LightVSOutput input) : SV_TARGET
//{
//
//    // �f�B���N�V�������C�g�ɂ��Lambert�g�U���ˌ���Phong���ʔ��ˌ����v�Z����
//    float3 diffDirection = CalcLambertDiffuse(dirDirection, dirColor, input.normal);
//    float3 specDirection = CalcPhongSpecular(dirDirection, dirColor, input.worldPos, input.normal);
//
//    // �|�C���g���C�g�ɂ��Lambert�g�U���ˌ���Phong���ʔ��ˌ����v�Z����
//    //�T�[�t�F�C�X�ɓ��˂���|�C���g���C�g�̌��̌������v�Z����
//    float3 ligDir = input.worldPos - spPosition;
//    ligDir = normalize(ligDir);
//
//    //�����Ȃ���Lambert�g�U���ˌ����v�Z����
//    float3 diffPoint = CalcLambertDiffuse
//    (
//        ligDir,
//        ptColor,
//        input.normal
//    );
//
//    //�����Ȃ���Phong���ʔ��ˌ����v�Z����
//    float3 specPoint = CalcPhongSpecular
//    (
//        ligDir,
//        ptColor,
//        input.worldPos,
//        input.normal
//    );
//
//
//
//    //�����ɂ��e�������v�Z����
//    float3 distance = length(input.worldPos - spPosition);
//    float affect = 1.0f - 1.0f / spRange * distance;
//
//    if (affect < 0.0f)
//    {
//        affect = 0.0f;
//    }
//    affect = pow(affect, 3.0);
//
//
//
//    //�g�U���ˌ��Ƌ��ʔ��ˌ��ɉe��������Z���ĉe������߂�    
//    diffPoint *= affect;
//    specPoint *= affect;
//
//    //���ˌ��Ǝˏo�����̊p�x�����߂�
//    float angle = dot(ligDir, spDirection);
//    angle = abs(acos(angle));
//
//
//    //�p�x�ɂ��e���������߂�
//    affect = 1.0f - 1.0f / spAngle * angle;
//
//    if (affect < 0.0f)
//    {
//        affect = 0;
//    }
//    affect = pow(affect, 0.5f);
//
//    //�e��������Z���Ĕ��ˌ�����߂�
//    diffPoint *= affect;
//    specPoint *= affect;
//
//    //�ŏI�I�Ȕ��ˌ������߂�
//    //�f�B���N�V�������C�g�����������������炱���ő���
//    float3 diffuseLig = diffPoint;
//    float3 specularLig = specPoint;
//
//
//    //�g�U���ˌ��Ƌ��ʔ��ˌ��𑫂��Z���āA�ŏI�I�Ȍ������߂�
//    float3 lig = diffuseLig + specularLig + ambientLight;
//    float4 finalColor = color;
//
//
//    //color�ɋ��߂�������Z���čŏI�o�̓J���[�����߂�
//    finalColor.xyz *= lig;
//
//    return finalColor;
//}

