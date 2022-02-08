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
    float fogFactor : FOG;
};

