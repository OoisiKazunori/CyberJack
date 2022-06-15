//#include"../ShaderHeader/BasicShaderHeader.hlsli"

struct ColorOutPut
{
    float4 svpos : SV_POSITION;
};

cbuffer SceneConstantBuffer : register(b0)
{
    float4 velocity;
    float4 offset;
    float4 color;
    float4x4 projection;
}

ColorOutPut VSmain(float4 pos : POSITION)
{
    ColorOutPut op;
	//op.svpos = mul(world, pos);   // モデルの頂点をワールド座標系に変換
	//op.svpos = mul(view, op.svpos);    // ワールド座標系からカメラ座標系に変換
	//op.svpos = mul(viewproj, op.svpos);    // カメラ座標系からスクリーン座標系に変換
    op.svpos = mul(pos + offset, projection);
    //op.svpos = pos;
	
    return op;
}