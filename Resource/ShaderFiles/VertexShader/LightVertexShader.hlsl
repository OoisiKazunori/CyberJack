#include"../ShaderHeader/BasicShaderHeader.hlsli"
#include"../ShaderHeader/LightHeader.hlsli"

LightVSOutput VSmain(float4 pos : POSITION, float3 normal : NORMAL)
{
	LightVSOutput outp;

	outp.svpos = mul(world, pos);   // モデルの頂点をワールド座標系に変換
	outp.worldPos = outp.svpos;

	outp.svpos = mul(view, outp.svpos);    // ワールド座標系からカメラ座標系に変換
	outp.svpos = mul(viewproj, outp.svpos);    // カメラ座標系からスクリーン座標系に変換	
	outp.normal = mul(world, normal); // 法線を回転させる

	return outp;
}