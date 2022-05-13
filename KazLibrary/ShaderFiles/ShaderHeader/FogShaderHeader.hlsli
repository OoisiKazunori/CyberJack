
float3 CalucurateFogValue(float3 INPUT, float3 FOG_COLOR, float3 SUB, float3 OBJ_COLOR, float MIN, float MAX, float START_FOG_DEPTH, float END_FOG_DEPTH)
{
    //�t�H�O��������n�܂���W����O�Ȃ�t�H�O�������Ȃ�
    if (INPUT.z <= START_FOG_DEPTH)
    {
        return OBJ_COLOR;
    }
    
    //-�ɂȂ�Ɗ������v�Z�ł��Ȃ��̂ŁA+�ɂ���
    float adjustPos = -MIN + INPUT.y;

    //�O���f�[�V�����̂̏㉺�ǂ̂�����ɂ��邩���߂�
    float gradationValue = adjustPos / (MAX + -MIN);


    //�t�H�O�J���[�Ƃ��̍����Ɍ��݂̈ʒu��n���ăO���f�[�V�����̂ǂ̐F�ɓ����邩���߂�
    float3 fogColor = FOG_COLOR + SUB * (1.0 - gradationValue);
    
    
    //�[�x�̊��������߂�
    float depthRate = (INPUT.z - START_FOG_DEPTH) / END_FOG_DEPTH;
    depthRate = saturate(depthRate);
    
    //OBJ�J���[�ƃt�H�O�J���[�̍�������[�x���g���Ăǂꂭ�炢�t�H�O�����邩���߂�
    float3 sub = fogColor - OBJ_COLOR;
    float3 result = OBJ_COLOR + sub * depthRate;
    
    return saturate(result);
}

//���s
float4 CaluDistanceFog(float4 POS, float4 Color, float3 FogColor)
{
    float4 fogFactor;
    float start = POS.w * 200;
    float end = POS.w * 600;
    float d = POS.z * POS.w;
    fogFactor = (end - d) / (end - start);
    
    float4 fc = float4(FogColor.x, FogColor.y, FogColor.z, 1);
    float4 fog = fogFactor * Color + (1 - fogFactor) * fc;
    
    return fog;
}

struct FogOutPut
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float3 fogFactor : FOG;
};

float4 CaluFog(float4 POS, float4 Color, float3 FogColor, float Density)
{
    //�w���t�H�O    
    float d = POS.z * POS.w;
    //���x
    float density = Density;
    float e = 2.71828f;
    float fogFactor = pow(e, -d * density);
    //�t�H�O��
    fogFactor *= 1;
    fogFactor = saturate(fogFactor);
    
    //�t�H�O�̐F  
    //color...�t�H�O��K������O�̃J���[
    float4 fc = float4(FogColor.x, FogColor.y, FogColor.z, 1);
    float4 fog = fogFactor * Color + (1 - fogFactor) * fc;
    
    //�������Đ^�����ɂȂ�Ȃ��悤�ɂ���
    //if (fc.x <= fog.x &&
    //    fc.y <= fog.y &&
    //    fc.z <= fog.z)
    //{
    //    return fc;
    //}
    
    return fog;
}