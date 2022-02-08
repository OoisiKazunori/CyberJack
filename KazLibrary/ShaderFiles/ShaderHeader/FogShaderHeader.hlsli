float4 CaluFog(float4 POS, float4 Color, float3 FogColor, float Density)
{
    //指数フォグ    
    float d = POS.z * POS.w;
    //密度
    float density = Density;
    float e = 2.71828f;
    float fogFactor = pow(e, -d * density);
    //フォグ量
    fogFactor *= 1;
    fogFactor = saturate(fogFactor);
    
    //フォグの色  
    //color...フォグを適応する前のカラー
    float4 fc = float4(FogColor.x, FogColor.y, FogColor.z, 1);
    float4 fog = fogFactor * Color + (1 - fogFactor) * fc;
    
    //遠すぎて真っ黒にならないようにした
    //if (fc.x <= fog.x &&
    //    fc.y <= fog.y &&
    //    fc.z <= fog.z)
    //{
    //    return fc;
    //}
    
    return fog;
}

//失敗
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
    float4 svpos : SV_POSITION; //システム用頂点座標
    float fogFactor : FOG;
};

