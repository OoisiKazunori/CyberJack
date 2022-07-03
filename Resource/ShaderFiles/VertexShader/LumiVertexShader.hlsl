#include"../ShaderHeader/BasicShaderHeader.hlsli"
#include"../ShaderHeader/LightHeader.hlsli"

VSOutput VSmain(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output;

	output.svpos = mul(world, pos);   // モデルの頂点をワールド座標系に変換
	output.svpos = mul(view, output.svpos);    // ワールド座標系からカメラ座標系に変換
	output.svpos = mul(viewproj, output.svpos);    // カメラ座標系からスクリーン座標系に変換

	output.uv = uv;

	return output;
}

