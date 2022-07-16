#include"PreCreateBaseRootSignature.h"
#include"../Pipeline/GraphicsRootSignature.h"

PreCreateBaseRootSignature::PreCreateBaseRootSignature()
{
#pragma region 座標のみ送る
	{
		RootSignatureData colorData;
		colorData.paramData[0].param = 0;
		colorData.paramData[0].type = GRAPHICS_PRAMTYPE_DRAW;
		colorData.range[0] = GRAPHICS_RANGE_TYPE_CBV;
		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_DATA_DRAW, colorData, 1);
	}
#pragma endregion


#pragma region 座標とテクスチャを送る
	{
		RootSignatureData spriteData;
		spriteData.paramData[0].param = 0;
		spriteData.paramData[0].type = GRAPHICS_PRAMTYPE_DRAW;
		spriteData.range[0] = GRAPHICS_RANGE_TYPE_CBV;
		spriteData.paramData[1].param = 1;
		spriteData.paramData[1].type = GRAPHICS_PRAMTYPE_TEX;
		spriteData.range[1] = GRAPHICS_RANGE_TYPE_SRV;
		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_DATA_DRAW_TEX, spriteData, 2);
	}
#pragma endregion


#pragma region 座標とテクスチャと環境光を送る
	{
		RootSignatureData objData;
		objData.paramData[0].param = 0;
		objData.paramData[0].type = GRAPHICS_PRAMTYPE_DRAW;
		objData.range[0] = GRAPHICS_RANGE_TYPE_CBV;

		objData.paramData[1].param = 1;
		objData.paramData[1].type = GRAPHICS_PRAMTYPE_DATA;
		objData.range[1] = GRAPHICS_RANGE_TYPE_CBV;

		objData.paramData[2].param = 2;
		objData.paramData[2].type = GRAPHICS_PRAMTYPE_TEX;
		objData.range[2] = GRAPHICS_RANGE_TYPE_SRV;

		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_DATA_DRAW_TEX_DATA1, objData, 3);
	}
#pragma endregion

#pragma region 座標とテクスチャと環境光とライトデータを送る
	{
		RootSignatureData objData;
		objData.paramData[0].param = 0;
		objData.paramData[0].type = GRAPHICS_PRAMTYPE_DRAW;
		objData.range[0] = GRAPHICS_RANGE_TYPE_CBV;

		objData.paramData[1].param = 1;
		objData.paramData[1].type = GRAPHICS_PRAMTYPE_DATA;
		objData.range[1] = GRAPHICS_RANGE_TYPE_CBV;

		objData.paramData[2].param = 2;
		objData.paramData[2].type = GRAPHICS_PRAMTYPE_TEX;
		objData.range[2] = GRAPHICS_RANGE_TYPE_SRV;

		objData.paramData[3].param = 3;
		objData.paramData[3].type = GRAPHICS_PRAMTYPE_DATA2;
		objData.range[3] = GRAPHICS_RANGE_TYPE_CBV;

		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_DATA_DRAW_TEX_DATA1_DATA2, objData, 4);
	}
#pragma endregion

#pragma region 座標とテクスチャ2枚と環境光とライトデータを送る
	{
		RootSignatureData objData;
		objData.paramData[0].param = 0;
		objData.paramData[0].type = GRAPHICS_PRAMTYPE_DRAW;
		objData.range[0] = GRAPHICS_RANGE_TYPE_CBV;

		objData.paramData[1].param = 1;
		objData.paramData[1].type = GRAPHICS_PRAMTYPE_DATA;
		objData.range[1] = GRAPHICS_RANGE_TYPE_CBV;

		objData.paramData[2].param = 2;
		objData.paramData[2].type = GRAPHICS_PRAMTYPE_TEX;
		objData.range[2] = GRAPHICS_RANGE_TYPE_SRV;

		objData.paramData[3].param = 3;
		objData.paramData[3].type = GRAPHICS_PRAMTYPE_TEX2;
		objData.range[3] = GRAPHICS_RANGE_TYPE_SRV;

		objData.paramData[4].param = 4;
		objData.paramData[4].type = GRAPHICS_PRAMTYPE_DATA2;
		objData.range[4] = GRAPHICS_RANGE_TYPE_CBV;

		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_DATA_DRAW_TEX_TEX2_DATA1_DATA2, objData, 5);
	}
#pragma endregion


#pragma region 座標とテクスチャと環境光を送る
	{
		RootSignatureData LightData;
		LightData.paramData[0].param = 0;
		LightData.paramData[0].type = GRAPHICS_PRAMTYPE_DRAW;
		LightData.range[0] = GRAPHICS_RANGE_TYPE_CBV;

		LightData.paramData[1].param = 1;
		LightData.paramData[1].type = GRAPHICS_PRAMTYPE_TEX;
		LightData.range[1] = GRAPHICS_RANGE_TYPE_SRV;

		LightData.paramData[2].param = 2;
		LightData.paramData[2].type = GRAPHICS_PRAMTYPE_DATA;
		LightData.range[2] = GRAPHICS_RANGE_TYPE_CBV;

		LightData.sample.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		LightData.sample.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		LightData.sample.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_MODE_LIGHT, LightData, 3);
	}
#pragma endregion

#pragma region 座標とテクスチャと環境光を送る
	{
		RootSignatureData LightData;
		LightData.paramData[0].param = 0;
		LightData.paramData[0].type = GRAPHICS_PRAMTYPE_DRAW;
		LightData.range[0] = GRAPHICS_RANGE_TYPE_CBV;

		LightData.paramData[1].param = 1;
		LightData.paramData[1].type = GRAPHICS_PRAMTYPE_TEX;
		LightData.range[1] = GRAPHICS_RANGE_TYPE_SRV;

		LightData.paramData[2].param = 2;
		LightData.paramData[2].type = GRAPHICS_PRAMTYPE_DATA;
		LightData.range[2] = GRAPHICS_RANGE_TYPE_CBV;

		LightData.sample.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		LightData.sample.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		LightData.sample.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_MODE_LIGHT_BORDER, LightData, 3);
	}
#pragma endregion

#pragma region 画像7枚と座標を送る
	{
		RootSignatureData bloomData;
		bloomData.paramData[0].param = 0;
		bloomData.paramData[0].type = GRAPHICS_PRAMTYPE_DRAW;
		bloomData.range[0] = GRAPHICS_RANGE_TYPE_CBV;


		bloomData.paramData[1].param = 0;
		bloomData.paramData[1].type = GRAPHICS_PRAMTYPE_TEX;
		bloomData.range[1] = GRAPHICS_RANGE_TYPE_SRV;

		bloomData.paramData[2].param = 1;
		bloomData.paramData[2].type = GRAPHICS_PRAMTYPE_TEX;
		bloomData.range[2] = GRAPHICS_RANGE_TYPE_SRV;

		bloomData.paramData[3].param = 2;
		bloomData.paramData[3].type = GRAPHICS_PRAMTYPE_TEX;
		bloomData.range[3] = GRAPHICS_RANGE_TYPE_SRV;

		bloomData.paramData[4].param = 3;
		bloomData.paramData[4].type = GRAPHICS_PRAMTYPE_TEX;
		bloomData.range[4] = GRAPHICS_RANGE_TYPE_SRV;

		bloomData.paramData[5].param = 4;
		bloomData.paramData[5].type = GRAPHICS_PRAMTYPE_TEX;
		bloomData.range[5] = GRAPHICS_RANGE_TYPE_SRV;

		bloomData.paramData[6].param = 5;
		bloomData.paramData[6].type = GRAPHICS_PRAMTYPE_TEX;
		bloomData.range[6] = GRAPHICS_RANGE_TYPE_SRV;

		bloomData.paramData[7].param = 6;
		bloomData.paramData[7].type = GRAPHICS_PRAMTYPE_TEX;
		bloomData.range[7] = GRAPHICS_RANGE_TYPE_SRV;


		bloomData.sample.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		bloomData.sample.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		bloomData.sample.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_DATA_DRAW_TEX4, bloomData, 8);
	}
#pragma endregion


#pragma region 座標と画像とスキニング行列を送る
	{
		RootSignatureData fbxData;
		fbxData.paramData[0].param = 0;
		fbxData.paramData[0].type = GRAPHICS_PRAMTYPE_DRAW;
		fbxData.range[0] = GRAPHICS_RANGE_TYPE_CBV;

		fbxData.paramData[1].param = 1;
		fbxData.paramData[1].type = GRAPHICS_PRAMTYPE_TEX;
		fbxData.range[1] = GRAPHICS_RANGE_TYPE_SRV;

		fbxData.paramData[2].param = 2;
		fbxData.paramData[2].type = GRAPHICS_PRAMTYPE_SKINING;
		fbxData.range[2] = GRAPHICS_RANGE_TYPE_CBV;

		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_DATA_DRAW_TEX_SKINING, fbxData, 3);
	}
#pragma endregion


#pragma region 座標とフォグの値を送る
	{
		//フォグ
		RootSignatureData fogData;
		fogData.paramData[0].param = 0;
		fogData.paramData[0].type = GRAPHICS_PRAMTYPE_DRAW;
		fogData.range[0] = GRAPHICS_RANGE_TYPE_CBV;

		fogData.paramData[1].param = 1;
		fogData.paramData[1].type = GRAPHICS_PRAMTYPE_DATA;
		fogData.range[1] = GRAPHICS_RANGE_TYPE_CBV;

		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_DATA_DRAW_DATA1, fogData, 2);
	}
#pragma endregion


#pragma region 座標と環境光とテクスチャとフォグの値を送る
	//フォグ込みObjモデル
	{
		RootSignatureData fogObjData;
		fogObjData.paramData[0].param = 0;
		fogObjData.paramData[0].type = GRAPHICS_PRAMTYPE_DRAW;
		fogObjData.range[0] = GRAPHICS_RANGE_TYPE_CBV;

		fogObjData.paramData[1].param = 1;
		fogObjData.paramData[1].type = GRAPHICS_PRAMTYPE_DATA;
		fogObjData.range[1] = GRAPHICS_RANGE_TYPE_CBV;

		fogObjData.paramData[2].param = 2;
		fogObjData.paramData[2].type = GRAPHICS_PRAMTYPE_TEX;
		fogObjData.range[2] = GRAPHICS_RANGE_TYPE_SRV;

		fogObjData.paramData[3].param = 3;
		fogObjData.paramData[3].type = GRAPHICS_PRAMTYPE_DATA2;
		fogObjData.range[3] = GRAPHICS_RANGE_TYPE_CBV;
		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_DATA_DRAW_DATA1_TEX_DATA2, fogObjData, 4);
	}
#pragma endregion


#pragma region 座標とテクスチャとスキニング行列とフォグの値を送る
	{
		//フォグ込みfbxモデル
		RootSignatureData fbxData;
		fbxData.paramData[0].param = 0;
		fbxData.paramData[0].type = GRAPHICS_PRAMTYPE_DRAW;
		fbxData.range[0] = GRAPHICS_RANGE_TYPE_CBV;

		fbxData.paramData[1].param = 0;
		fbxData.paramData[1].type = GRAPHICS_PRAMTYPE_TEX;
		fbxData.range[1] = GRAPHICS_RANGE_TYPE_SRV;

		fbxData.paramData[2].param = 2;
		fbxData.paramData[2].type = GRAPHICS_PRAMTYPE_SKINING;
		fbxData.range[2] = GRAPHICS_RANGE_TYPE_CBV;

		fbxData.paramData[3].param = 3;
		fbxData.paramData[3].type = GRAPHICS_PRAMTYPE_DATA;
		fbxData.range[3] = GRAPHICS_RANGE_TYPE_CBV;

		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_DATA_DRAW_TEX_SKINING_DATA1, fbxData, 4);
	}
#pragma endregion


#pragma region 座標とフォグの値を送る
	{
		RootSignatureData lineData;
		lineData.paramData[0].param = 0;
		lineData.paramData[0].type = GRAPHICS_PRAMTYPE_DRAW;
		lineData.range[0] = GRAPHICS_RANGE_TYPE_CBV;

		lineData.paramData[1].param = 1;
		lineData.paramData[1].type = GRAPHICS_PRAMTYPE_DATA;
		lineData.range[1] = GRAPHICS_RANGE_TYPE_CBV;

		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_MODE_LINE, lineData, 2);
	}
#pragma endregion


#pragma region インスタンシング描画
	{
		RootSignatureData lineData;
		lineData.paramData[0].param = 0;
		lineData.paramData[0].type = GRAPHICS_PRAMTYPE_DATA;
		lineData.range[0] = GRAPHICS_RANGE_TYPE_CBV;

		lineData.paramData[1].param = 1;
		lineData.paramData[1].type = GRAPHICS_PRAMTYPE_DATA2;
		lineData.range[1] = GRAPHICS_RANGE_TYPE_CBV;

		lineData.paramData[2].param = 2;
		lineData.paramData[2].type = GRAPHICS_PRAMTYPE_TEX;
		lineData.range[2] = GRAPHICS_RANGE_TYPE_SRV;

		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_DATA_DATA1_DATA2_TEX, lineData, 3);
	}
#pragma endregion


	{
		RootSignatureData lineData;
		lineData.paramData[0].param = 0;
		lineData.paramData[0].type = GRAPHICS_PRAMTYPE_DATA;
		lineData.range[0] = GRAPHICS_RANGE_TYPE_CBV;

		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_DATA_DATA1, lineData, 1);
	}


	{
		RootSignatureData lineData;
		lineData.paramData[0].param = 0;
		lineData.paramData[0].type = GRAPHICS_PRAMTYPE_DATA;
		lineData.range[0] = GRAPHICS_RANGE_TYPE_CBV;

		lineData.paramData[1].param = 1;
		lineData.paramData[1].type = GRAPHICS_PRAMTYPE_DATA2;
		lineData.range[1] = GRAPHICS_RANGE_TYPE_CBV;

		lineData.paramData[2].param = 2;
		lineData.paramData[2].type = GRAPHICS_PRAMTYPE_DATA3;
		lineData.range[2] = GRAPHICS_RANGE_TYPE_CBV;

		lineData.paramData[3].param = 3;
		lineData.paramData[3].type = GRAPHICS_PRAMTYPE_TEX;
		lineData.range[3] = GRAPHICS_RANGE_TYPE_SRV;

		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_DATA_DATA1_DATA2_DATA3_TEX, lineData, 4);
	}


	{
		RootSignatureData lineData;
		lineData.paramData[0].param = 0;
		lineData.paramData[0].type = GRAPHICS_PRAMTYPE_DATA;
		lineData.range[0] = GRAPHICS_RANGE_TYPE_CBV;

		lineData.paramData[1].param = 1;
		lineData.paramData[1].type = GRAPHICS_PRAMTYPE_DATA2;
		lineData.range[1] = GRAPHICS_RANGE_TYPE_CBV;

		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_DATA_DATA1_DATA2, lineData, 2);
	}



	{
		RootSignatureData lineData;
		lineData.paramData[0].param = 0;
		lineData.paramData[0].type = GRAPHICS_PRAMTYPE_DATA;
		lineData.range[0] = GRAPHICS_RANGE_TYPE_SRV;

		lineData.paramData[1].param = 0;
		lineData.paramData[1].type = GRAPHICS_PRAMTYPE_DATA2;
		lineData.range[1] = GRAPHICS_RANGE_TYPE_UAV;

		lineData.paramData[2].param = 1;
		lineData.paramData[2].type = GRAPHICS_PRAMTYPE_DATA3;
		lineData.range[2] = GRAPHICS_RANGE_TYPE_UAV;

		lineData.paramData[3].param = 2;
		lineData.paramData[3].type = GRAPHICS_PRAMTYPE_DATA4;
		lineData.range[3] = GRAPHICS_RANGE_TYPE_UAV;

		lineData.paramData[4].param = 0;
		lineData.paramData[4].type = GRAPHICS_PRAMTYPE_DATA5;
		lineData.range[4] = GRAPHICS_RANGE_TYPE_CBV;

		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_DATA_SRV_UAV, lineData, 5);
	}

	//CBV
	{
		RootSignatureData lineData;
		lineData.paramData[0].param = 0;
		lineData.paramData[0].type = GRAPHICS_PRAMTYPE_DATA;
		lineData.range[0] = GRAPHICS_RANGE_TYPE_CBV;
		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_DATA_CBV, lineData, 1);
	}

	{
		RootSignatureData spriteData;
		spriteData.paramData[0].param = 0;
		spriteData.paramData[0].type = GRAPHICS_PRAMTYPE_DRAW;
		spriteData.range[0] = GRAPHICS_RANGE_TYPE_UAV;
		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_DATA_DRAW_UAV, spriteData, 1);
	}


#pragma region 座標とテクスチャ二枚を送る
	{
		RootSignatureData spriteData;
		spriteData.paramData[0].param = 0;
		spriteData.paramData[0].type = GRAPHICS_PRAMTYPE_DRAW;
		spriteData.range[0] = GRAPHICS_RANGE_TYPE_CBV;
		spriteData.paramData[1].param = 1;
		spriteData.paramData[1].type = GRAPHICS_PRAMTYPE_TEX;
		spriteData.range[1] = GRAPHICS_RANGE_TYPE_SRV;
		spriteData.paramData[2].param = 2;
		spriteData.paramData[2].type = GRAPHICS_PRAMTYPE_TEX2;
		spriteData.range[2] = GRAPHICS_RANGE_TYPE_SRV;
		GraphicsRootSignature::Instance()->CreateRootSignature(ROOTSIGNATURE_DATA_DRAW_TEX_TEX2, spriteData, 3);
	}
#pragma endregion

}
