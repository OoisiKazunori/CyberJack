#pragma once
#include"../DirectXCommon/Base.h"
#include<DirectXTex.h>
#include<string>
#include"../Pipeline/Shader.h"
#include<vector>
#include"../Helper/ISinglton.h"
#include"../Pipeline/GraphicsRootSignature.h"
#include"../Buffer/CreateGpuBuffer.h"

/// <summary>
/// 画像の分割をする際にどの座標からどれくらいのサイズでUV切り取りをするか記録したもの
/// </summary>
struct DivGraphData
{
	vector<XMFLOAT2> divLeftUp;
	XMFLOAT2 divSize;
	short handle;
};

/// <summary>
/// 画像の読み込み、画像の管理
/// </summary>
class TextureResourceMgr :public ISingleton<TextureResourceMgr> {
public:

	TextureResourceMgr();
	~TextureResourceMgr();

	/// <summary>
	/// ファイルパスを書いて画像の読み込みを行います
	/// </summary>
	/// <param name="RESOURCE">読み込みたい画像のファイルパス</param>
	/// <returns>ハンドル</returns>
	short LoadGraph(std::string RESOURCE);
	
	/// <summary>
	///  ファイルパスを書いて画像を読み込み、分割します
	/// </summary>
	/// <param name="RESOURCE">読み込みたい画像のファイルパス</param>
	/// <param name="ALL_NUM">分割する画像の総数</param>
	/// <param name="DIV_WIDTH_NUM">X軸に画像を分割する数</param>
	/// <param name="DIV_HEIGHT_NUM">Y軸に画像を分割する数</param>
	/// <param name="TEXSIZE_WIDTH">分割する画像のXサイズ</param>
	/// <param name="TEXSIZE_HEIGHT">分割する画像のYサイズ</param>
	/// <returns>ハンドル</returns>
	short LoadDivGraph(string RESOURCE, 
		unsigned int ALL_NUM,
		unsigned int DIV_WIDTH_NUM, unsigned int DIV_HEIGHT_NUM,
		unsigned int TEXSIZE_WIDTH, unsigned int TEXSIZE_HEIGHT
	);

	/// <summary>
	/// 読み込んだ画像を削除します
	/// </summary>
	/// <param name="HANDLE">削除したい画像のハンドル</param>
	void Release(short HANDLE);

	/// <summary>
	/// ハンドルからシェーダーリソースをコマンドリストに積ませます
	/// </summary>
	/// <param name="GRAPH_HANDLE">ハンドル</param>
	/// <param name="PARAM">ルートパラム</param>
	/// <param name="TYPE">ルートパラムの種類</param>
	void SetSRV(short GRAPH_HANDLE, GraphicsRootSignatureParameter PARAM, GraphicsRootParamType TYPE);


	/// <summary>
	/// 画像の情報を渡します
	/// バッファの取得に失敗した場合 WidthとHeightに0が返されます
	/// </summary>
	/// <param name="HANDLE"></param>
	/// <returns></returns>
	D3D12_RESOURCE_DESC GetTextureSize(short HANDLE);

	/// <summary>
	/// 分割する画像の情報を渡します
	/// バッファの取得に失敗した場合 texSizeのXとYに-1が入ります
	/// </summary>
	/// <param name="HANDLE">ハンドル</param>
	/// <returns>分割する画像の情報</returns>
	DivGraphData GetDivData(short HANDLE);

private:
	unique_ptr<CreateGpuBuffer> buffers;
	const Image *img;

	D3D12_STATIC_SAMPLER_DESC samplerDesc;

	static const int texWidth = 256;
	static const int texDataCount = texWidth * texWidth;
	const int DescriptorMaxNum = 5000;

	ScratchImage scratchImg;
	UINT handle;
	UINT setHandle;
	UINT IncreSize;
	TexMetadata metadata;

	HRESULT result;

	std::vector<std::string> handleName;

	vector<DivGraphData> divData;
	friend ISingleton<TextureResourceMgr>;
};