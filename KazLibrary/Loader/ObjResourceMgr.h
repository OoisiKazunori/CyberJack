#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/HandleMaker.h"
#include"../Helper/ISinglton.h"
#include<fstream>
#include<sstream>
#include"../Buffer/CreateGpuBuffer.h"

struct materia
{
	string name;//マテリアル名
	XMFLOAT3 ambient;//アンビエント影響度
	XMFLOAT3 diffuse;//ディフューズ影響度
	XMFLOAT3 specular;//スペキュラー影響度
	float alpha;//α
	string textureFilename;//テクスチャファイル名

	materia()
	{
		ambient = { 0.3f,0.3f,0.3f };
		diffuse = { 0.0f,0.0f,0.0f };
		specular = { 0.0f,0.0f,0.0f };
		alpha = 1.0;
	}

	void Delete()
	{
		ambient = { 0.0f,0.0f,0.0f };
		diffuse = { 0.0f,0.0f,0.0f };
		specular = { 0.0f,0.0f,0.0f };
		alpha = 0.0;
	}
};


/// <summary>
/// .Objのモデルを描画するのに必要なデータが入ったもの
/// </summary>
struct ObjResourceData 
{
	//vector<Vertex> vert;
	//vector<unsigned short> indexNum;

	materia material;
	short mtlHanlde;
	
	vector<unsigned short> posKey;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	UINT indexNum;
};



/// <summary>
/// .objに必要なデータを保管し、objを描画する際にデータを使い回すクラス
/// </summary>
class ObjResourceMgr :public ISingleton<ObjResourceMgr>
{
public:
	ObjResourceMgr();
	~ObjResourceMgr();

	/// <summary>
	/// .objのモデルを読み込みます
	/// .objと.pngと.mtlの名前は統一して下さい
	/// </summary>
	/// <param name="RESOURCE">.objのデータがあるファイルパス</param>
	/// <param name="TEX_RESOURCE">.mtlの画像データがあるファイルパス</param>
	/// <returns>ハンドル</returns>
	short LoadModel(string RESOURCE);

	/// <summary>
	/// 指定のモデルデータを削除します
	/// </summary>
	/// <param name="HANDLE">指定のハンドル</param>
	void Release(short HANDLE);

	/// <summary>
	/// 指定のハンドルからリソースを取得します
	/// </summary>
	/// <param name="HANDLE">指定のハンドル</param>
	ObjResourceData GetResourceData(short HANDLE);

private:
	HandleMaker handle;

	vector<ObjResourceData> resource;
	vector<string> name;

	unique_ptr<CreateGpuBuffer> modelDataBuffers;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="filename">画像のファイル名</param>
	/// <param name="resource"></param>
	/// <param name="MTL_RESOURCE"></param>
	/// <returns></returns>
	short LoadMaterial(const string &filename, ObjResourceData *resource, string MTL_RESOURCE);

};
