#pragma once
#pragma warning(push)
#pragma warning(disable:4996)
#include"fbxsdk.h"
#pragma warning(pop)
#include"../DirectXCommon/Base.h"
#include"../Helper/ISinglton.h"
#include"../Helper/HandleMaker.h"
#include"../Buffer/CreateGpuBuffer.h"



struct NewWeightSet
{
	UINT index;
	float weight;
};


/// <summary>
/// ノード一つ分の情報
/// </summary>
struct NewNode
{
	/// <summary>
	/// 名前
	/// </summary>
	string name;
	//ローカルスケール
	XMVECTOR scaling = { 1.0f,1.0f,1.0f,0.0f };
	//ローカル回転角
	XMVECTOR rotation = { 0.0f,0.0f,0.0f,0.0f };
	//ローカル移動
	XMVECTOR translation = { 0.0f,0.0f,0.0f,1.0f };
	//ローカル変形行列
	XMMATRIX transform;
	//グローバル変形行列
	XMMATRIX globalTransform;
	//親ノード
	NewNode *parent = nullptr;
};

/// <summary>
/// 読み取った情報を流し込み先
/// </summary>
class Model
{
public:
	static const int MAX_BONE_INDICES = 4;

	/// <summary>
	/// ボーン構造体
	/// </summary>
	struct Bone
	{
		//名前
		string name;
		//初期姿勢の逆行列
		XMMATRIX invInitialPose;
		//クラスター(FBXのボーン情報)
		FbxCluster *fbxCluster;

		Bone(const string &NAME)
		{
			this->name = NAME;
		}
	};
private:
	struct VertexPosNormalUvSkin
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT3 uv;
		UINT boneIndex[MAX_BONE_INDICES];
		float boneWeight[MAX_BONE_INDICES];
	};	

	string name;
	vector<NewNode>nodes;
	vector<Bone>bones;


	NewNode *meshNode = nullptr;
	vector<VertexPosNormalUvSkin>vertices;
	vector<unsigned short>indices;


	XMFLOAT3 ambient = { 1.0f,1.0f,1.0f };
	XMFLOAT3 diffuse = { 1.0f,1.0f,1.0f };
	vector<short> textureHandle;
public:
	friend class FbxModelResourceMgr;

	vector<Bone> &GetBone()
	{
		return bones;
	};
};


struct FbxResourceD
{
public:
	unique_ptr<CreateGpuBuffer> buffers;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	vector<short> textureHandle;
	short indicisNum;
	XMFLOAT3 ambient = { 1.0f,1.0f,1.0f };
	XMFLOAT3 diffuse = { 1.0f,1.0f,1.0f };
	vector<Model::Bone> bone;

	vector<FbxTime> startTime;
	vector<FbxTime> endTime;
};

class FbxModelResourceMgr:public ISingleton<FbxModelResourceMgr>
{
public:
	FbxModelResourceMgr();
	~FbxModelResourceMgr();

	short LoadModel(const string &MODEL_NAME);

	const FbxResourceD* GetResourceData(const short &HANDLE);

private:
	FbxManager *fbxManager;
	FbxImporter *fbxImporter;
	FbxScene *fbxScene;

	vector<FbxResourceD*>modelResource;

	unique_ptr<HandleMaker> handle;


	/// <summary>
	/// 再帰的にノード構成を解析
	/// </summary>
	/// <param name="MODEL">読み込み先のモデルオブジェクト</param>
	/// <param name="FBX_NODE">解析対象のノード</param>
	void ParseNodeRecursive(Model *MODEL, FbxNode *FBX_NODE, NewNode *PARENT = nullptr);

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name="MODEL">読み込み先モデルオブジェクト</param>
	/// <param name="FBX_NODE">解析対象のノード</param>
	void ParseMesh(Model *MODEL, FbxNode *FBX_NODE);

	//頂点座標読み取り
	void ParseMeshVertices(Model *MODEL, FbxMesh *FBX_MESH);
	//面情報読み取り
	void ParseMeshFaces(Model *MODEL, FbxMesh *FBX_MESH);
	//マテリアル読み取り
	void ParseMaterial(Model *MODEL, FbxNode *FBX_NODE);
	//テクスチャ読み取り
	void LoadTexture(Model *MODEL, const string &FULL_PATH);
	//スキニング情報の読み取り
	void ParseSkin(Model *MODEL, FbxMesh *FBX_MESH);


	XMVECTOR FbxDoubleToXMVECTOR(const FbxDouble3 &DOUBLE_3);

	string ExtractFileName(const string &PATH);

	void ConvertMatrixFromFbx(XMMATRIX *DST, const FbxAMatrix &SRC);

};

