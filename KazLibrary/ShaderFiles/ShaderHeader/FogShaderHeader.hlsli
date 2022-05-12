
float3 CalucurateFogValue(float3 INPUT, float3 FOG_COLOR, float3 SUB, float3 OBJ_COLOR, float MIN, float MAX, float START_FOG_DEPTH, float END_FOG_DEPTH)
{
    //フォグがかかる始まる座標より手前ならフォグをかけない
    if (INPUT.z <= START_FOG_DEPTH)
    {
        return OBJ_COLOR;
    }
    
    //-の部分をプラスに修正する
    float adjustPos = INPUT.y;
    
    //-なら最小値を現在地から引いた差分を割合を決める要素にする
    //+なら最小値を現在地に足した物をを割合を決める要素にする
    if (INPUT.y < 0.0f)
    {
        adjustPos = -MIN + INPUT.y;
    }
    else
    {
        adjustPos = MIN + INPUT.y;
    }
    
    
    //グラデーションのの上下どのあたりにいるか決める
    float gradationValue = adjustPos / (MAX + -MIN);

    //フォグカラーとその差分に現在の位置を渡してグラデーションのどの色に当たるか決める
    float3 fogColor = FOG_COLOR + SUB * gradationValue;
    
    //深度の割合を決める
    float depthRate = INPUT.z / END_FOG_DEPTH;
    
    //OBJカラーとフォグカラーの差分から深度を使ってどれくらいフォグかけるか決める
    float3 sub = OBJ_COLOR - fogColor;
    float3 result = OBJ_COLOR + sub * depthRate;
    return result;
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
    float3 fogFactor : FOG;
};

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