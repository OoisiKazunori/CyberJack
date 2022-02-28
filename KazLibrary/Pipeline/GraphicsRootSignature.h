#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/ISinglton.h"
#include<vector>
using namespace std;


/// <summary>
/// ルートシグネチャーの種類
/// </summary>
enum RootSignatureMode
{
	ROOTSIGNATURE_DATA_DRAW,
	//削除予定
	ROOTSIGNATURE_DATA_DRAW_TEX,
	ROOTSIGNATURE_DATA_DRAW_TEX_DATA1,
	ROOTSIGNATURE_DATA_DRAW_TEX_DATA1_DATA2,
	ROOTSIGNATURE_DATA_DRAW_TEX_TEX2_DATA1_DATA2,
	//削除予定
	ROOTSIGNATURE_MODE_MULTIPASS,
	//削除予定
	ROOTSIGNATURE_MODE_LIGHT,
	ROOTSIGNATURE_MODE_LIGHT_BORDER,
	ROOTSIGNATURE_DATA_DRAW_TEX4,
	ROOTSIGNATURE_DATA_DRAW_TEX_SKINING,
	ROOTSIGNATURE_DATA_DRAW_DATA1,
	ROOTSIGNATURE_DATA_DRAW_TEX_SKINING_DATA1,
	ROOTSIGNATURE_DATA_DRAW_DATA1_TEX_DATA2,
	//削除予定
	ROOTSIGNATURE_MODE_LINE,
	ROOTSIGNATURE_DATA_DATA1_DATA2_TEX,
	ROOTSIGNATURE_DATA_DATA1_DATA2_DATA3_TEX,
	ROOTSIGNATURE_DATA_DATA1,
	ROOTSIGNATURE_DATA_DATA1_DATA2,
	//テスト用
	ROOTSIGNATURE_DATA_SRV_UAV_CBV,
};


/// <summary>
/// SetSRVかSetCBVの引数に値を渡した際に
///コマンドリストのSetGraphicsRootConstantBufferView()に値を渡すのか
///SetGraphicsDescriptorTable()に値を渡すのか判断する際に必要
/// </summary>
enum GraphicsRootSignatureType
{
	GRAPHICS_ROOTSIGNATURE_TYPE_NONE = -1,
	GRAPHICS_ROOTSIGNATURE_TYPE_DESCRIPTORTABLE,
	GRAPHICS_ROOTSIGNATURE_TYPE_VIEW
};

/// <summary>
/// どういった種類のデスクリプタレンジなのか判断する際に必要
/// </summary>
enum GraphicsRangeType
{
	GRAPHICS_RANGE_TYPE_NONE = -1,
	GRAPHICS_RANGE_TYPE_SRV,
	GRAPHICS_RANGE_TYPE_UAV,
	GRAPHICS_RANGE_TYPE_CBV,
	GRAPHICS_RANGE_TYPE_CBV_DESC,
	GRAPHICS_RANGE_TYPE_SAMPLER
};

/// <summary>
/// そのルートパラムはどういった役割を持っているのか
/// </summary>
enum GraphicsRootParamType
{
	GRAPHICS_PRAMTYPE_NONE = -1,
	//描画系のみ使用
	GRAPHICS_PRAMTYPE_DRAW,
	//テクスチャのみ使用
	GRAPHICS_PRAMTYPE_TEX,
	GRAPHICS_PRAMTYPE_TEX2,
	//マテリアルデータのみ使用
	GRAPHICS_PRAMTYPE_MATERIAL,
	//その他(ライトデータなど使用)
	GRAPHICS_PRAMTYPE_DATA,
	GRAPHICS_PRAMTYPE_DATA2,
	GRAPHICS_PRAMTYPE_DATA3,
	//スキニング情報のみ使用
	GRAPHICS_PRAMTYPE_SKINING
};

/// <summary>
/// ルートパラメーターの番号とルートパラメーターの役割を指定します
/// </summary>
struct RootParamData
{
	short param;
	GraphicsRootParamType type;
};


/// <summary>
/// SetSRV、もしくはSetCBVにて、 ルートパラムの番号とディスクリプタヒープとして積ませるか、ビューとして積ませるか
/// を判断する際に必要になる構造体です
/// </summary>
struct GraphicsRootSignatureParameter
{
	GraphicsRangeType range[10];			//種類
	RootParamData paramData[10];			//パラメーター
	GraphicsRootSignatureType type[10];		//どのセットするか
	short paramMax;		    				//使用したパラメーターの数

	//初期化
	GraphicsRootSignatureParameter()
	{
		for (int i = 0; i < 10; i++)
		{
			range[i] = GRAPHICS_RANGE_TYPE_NONE;
			paramData[i].param = -1;
			paramData[i].type = GRAPHICS_PRAMTYPE_NONE;
			type[i] = GRAPHICS_ROOTSIGNATURE_TYPE_NONE;
		}
		paramMax = -1;
	}
};

/// <summary>
/// ルートシグネチャを生成する際に必要になるデータです
/// </summary>
struct RootSignatureData
{
	GraphicsRangeType range[10];
	RootParamData paramData[10];
	D3D12_STATIC_SAMPLER_DESC sample;

	RootSignatureData()
	{
		sample.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sample.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sample.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sample.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sample.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		sample.MaxLOD = D3D12_FLOAT32_MAX;
		sample.MinLOD = 0.0f;
		sample.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sample.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		sample.MaxAnisotropy = 16;
		sample.MipLODBias = 0;
		sample.ShaderRegister = 0;
		sample.RegisterSpace = 0;
	}
};


/// <summary>
/// ルートシグネチャーを生成、管理します
/// </summary>
class GraphicsRootSignature :public ISingleton<GraphicsRootSignature>
{
public:
	GraphicsRootSignature();
	~GraphicsRootSignature();

	/// <summary>
	/// ルートシグネチャを生成します
	/// </summary>
	/// <param name="ROOTSIGNATURE_DATA">ルートシグネチャを生成する際に必要になるデータ</param>
	void CreateRootSignature(RootSignatureMode ROOTSIGNATURE, RootSignatureData ROOTSIGNATURE_DATA, short DATA_MAX);

	/// <summary>
	/// 指定のルートシグネチャをコマンドリストに積みます
	/// </summary>
	/// <param name="ROOTSIGNATURE_MODE">どのルートシグネチャを設定するか</param>
	void SetRootSignature(RootSignatureMode ROOTSIGNATURE_MODE);

	/// <summary>
	/// 指定のルートシグネチャをコマンドリストに積みます
	/// </summary>
	/// <param name="ROOTSIGNATURE_MODE">どのルートシグネチャを設定するか</param>
	void SetComputeRootSignature(RootSignatureMode ROOTSIGNATURE_MODE);


	/// <summary>
	/// 指定のルートシグネチャーを返り値で渡します
	/// </summary>
	/// <param name="ROOT_SIGNATURE">指定のルートシグネチャー</param>
	/// <returns>生成されたルートシグネチャー</returns>
	const ComPtr<ID3D12RootSignature> GetRootSignature(RootSignatureMode ROOT_SIGNATURE);

	/// <summary>
	/// 指定のルートシグネチャー毎に設定されたルートパラメーターを渡します
	/// </summary>
	/// <param name="ROOTSIGNATURE_MODE">指定のルートシグネチャー</param>
	/// <returns>ルートパラメーター</returns>
	const GraphicsRootSignatureParameter GetRootParam(RootSignatureMode ROOTSIGNATURE_MODE);

private:
	/*void CreateColorRootSignature();
	void CreateTextureRootSignature();
	void CreateSpriteRootSignature();
	void CreateObjRootSignature();
	void CreateMultipassRootSignature();
	void CreateLight();

	GraphicsRootSignatureParameter paramMgr[10];
	GraphicsRootSignatureParameter textureParam;
	GraphicsRootSignatureParameter spriteParam;
	GraphicsRootSignatureParameter objParam;
	GraphicsRootSignatureParameter MultiPassParam;
	GraphicsRootSignatureParameter LightParam;
	GraphicsRootSignatureParameter colorParam;*/

	vector<ComPtr<ID3D12RootSignature>> rootSignature;
	vector<GraphicsRootSignatureParameter> paramD;
	friend ISingleton<GraphicsRootSignature>;


	void CreateMyRootSignature(D3D12_STATIC_SAMPLER_DESC SAMPLER_DATA, D3D12_ROOT_PARAMETER *ROOT_PARAM_DATA, short DATA_MAX, RootSignatureMode ROOTSIGNATURE);

	RootSignatureMode CheckEnum(RootSignatureMode MODE)
	{
		RootSignatureMode tmp = MODE;
		return tmp;
	};
};


/*
メモ帳

ルートパラムを分ける理由
1.構造体っぽく
2.使い分ける単位
例.
ライトとオブジェクト
ライトは一度送っただけで値をそのまま使えるが、オブジェクトは頻繁に切り替える
テクスチャの切り替えが負担かかる...
パーティクルだと一回セットして描画する
*/