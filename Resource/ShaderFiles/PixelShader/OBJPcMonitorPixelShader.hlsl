#include"../ShaderHeader/OBJShaderHeader.hlsli"
#include"../ShaderHeader/BaseShaderHeader.hlsli"
#include"../ShaderHeader/MultiPassHeader.hlsli"

Texture2D<float4>modelTex:register(t0);
Texture2D<float4>maskTex:register(t1);
Texture2D<float4>movieTex:register(t2);
SamplerState smp :register(s0);

static const float2 LEFT_UP_UV =    float2(0.03f,0.96f);
static const float2 LEFT_DOWN_UV =  float2(0.03f,0.38f);
static const float2 RIGHT_UP_UV =   float2(0.979f,0.963f);
static const float2 RIGHT_DOWN_UV = float2(0.979f,0.38f);

TwoRender PSmain(VSOutput input) : SV_TARGET
{
	float3 light = normalize(float3(1,-1,1));			//右下奥向きのライト
	float light_diffuse = saturate(dot(-light, input.normal));//環境光...diffuseを[0,1]の範囲にClampする
	
	float3 shade_color;
	shade_color = m_ambient + 0.0;					//アンビエント
	shade_color += m_diffuse * light_diffuse; //ディフューズ項

    float4 maskColor = maskTex.Sample(smp, input.uv);

    TwoRender output;
    //モニター描画
    if(1.0f <= maskColor.r)
    {
        //モニター用のUVに書き換えする
        float2 movieUV = (input.uv - float2(0.03,0.38)) / float2(0.97,0.59);
        movieUV.x = 1.0f - movieUV.x;
        output.target0 = movieTex.Sample(smp, movieUV);
        output.target1 = float4(1,0,0,0.4);
    }
    //モデル描画
    else
    {
        output.target0 = modelTex.Sample(smp, input.uv);
        output.target0.a = m_alpha;
        output.target1 = float4(0,0,0,0);
    }

    return output;
}