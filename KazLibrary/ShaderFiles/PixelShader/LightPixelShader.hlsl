#include"../ShaderHeader/BasicShaderHeader.hlsli"
#include"../ShaderHeader/LightHeader.hlsli"

float4 PSmain(LightVSOutput input) : SV_TARGET
{

    // ディレクションライトによるLambert拡散反射光とPhong鏡面反射光を計算する
    float3 diffDirection = CalcLambertDiffuse(dirDirection, dirColor, input.normal);
    float3 specDirection = CalcPhongSpecular(dirDirection, dirColor, input.worldPos, input.normal);

    // ポイントライトによるLambert拡散反射光とPhong鏡面反射光を計算する
    //サーフェイスに入射するポイントライトの光の向きを計算する
    float3 ligDir = input.worldPos - ptPosition;
    ligDir = normalize(ligDir);

    //減衰なしのLambert拡散反射光を計算する
    float3 diffPoint = CalcLambertDiffuse 
    (
        ligDir,
        ptColor,
        input.normal
    );

    //減衰なしのPhong鏡面反射光を計算する
    float3 specPoint = CalcPhongSpecular
    (
        ligDir,
        ptColor,
        input.worldPos,
        input.normal
    );



    //距離による影響率を計算する
    float3 distance = length(input.worldPos - ptPosition); 
    float affect = 1.0f - 1.0f / ptRange * distance;

    if (affect < 0.0f)
    {
        affect = 0.0f;
    }
    affect = pow(affect, 3.0);



    //拡散反射光と鏡面反射光に影響率を乗算して影響を弱める    
    diffPoint *= affect;        
    specPoint *= affect;

    //最終的な反射光を求める
    //ディレクションライトを合成したかったらここで足す
    float3 diffuseLig = diffPoint + diffDirection;
    float3 specularLig = specPoint + specDirection;
    //float3 diffuseLig = diffPoint;
    //float3 specularLig = specPoint;

    //拡散反射光と鏡面反射光を足し算して、最終的な光を求める
    float3 lig = diffuseLig + specularLig + ambientLight;
    float4 finalColor = color;


    //colorに求めた光を乗算して最終出力カラーを求める
    finalColor.xyz *= lig;

    return finalColor;
}



//float4 SpotLightPSmain(LightVSOutput input) : SV_TARGET
//{
//
//    // ディレクションライトによるLambert拡散反射光とPhong鏡面反射光を計算する
//    float3 diffDirection = CalcLambertDiffuse(dirDirection, dirColor, input.normal);
//    float3 specDirection = CalcPhongSpecular(dirDirection, dirColor, input.worldPos, input.normal);
//
//    // ポイントライトによるLambert拡散反射光とPhong鏡面反射光を計算する
//    //サーフェイスに入射するポイントライトの光の向きを計算する
//    float3 ligDir = input.worldPos - spPosition;
//    ligDir = normalize(ligDir);
//
//    //減衰なしのLambert拡散反射光を計算する
//    float3 diffPoint = CalcLambertDiffuse
//    (
//        ligDir,
//        ptColor,
//        input.normal
//    );
//
//    //減衰なしのPhong鏡面反射光を計算する
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
//    //距離による影響率を計算する
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
//    //拡散反射光と鏡面反射光に影響率を乗算して影響を弱める    
//    diffPoint *= affect;
//    specPoint *= affect;
//
//    //入射光と射出方向の角度を求める
//    float angle = dot(ligDir, spDirection);
//    angle = abs(acos(angle));
//
//
//    //角度による影響率を求める
//    affect = 1.0f - 1.0f / spAngle * angle;
//
//    if (affect < 0.0f)
//    {
//        affect = 0;
//    }
//    affect = pow(affect, 0.5f);
//
//    //影響率を乗算して反射光を弱める
//    diffPoint *= affect;
//    specPoint *= affect;
//
//    //最終的な反射光を求める
//    //ディレクションライトを合成したかったらここで足す
//    float3 diffuseLig = diffPoint;
//    float3 specularLig = specPoint;
//
//
//    //拡散反射光と鏡面反射光を足し算して、最終的な光を求める
//    float3 lig = diffuseLig + specularLig + ambientLight;
//    float4 finalColor = color;
//
//
//    //colorに求めた光を乗算して最終出力カラーを求める
//    finalColor.xyz *= lig;
//
//    return finalColor;
//}

