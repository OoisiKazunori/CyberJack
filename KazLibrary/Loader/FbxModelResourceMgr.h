#pragma once
#pragma warning(push)
#pragma warning(disable:4023)
#include"fbxsdk.h"
#pragma warning(pop)
#include"../DirectXCommon/Base.h"
#include"../Helper/ISinglton.h"
#include"../Helper/HandleMaker.h"
#include"../Buffer/CreateGpuBuffer.h"



struct WeightSet
{
	UINT index;
	float weight;
};


/// <summary>
/// ノード一つ分の情報
/// </summary>
struct Node
{
	/// <summary>
	/// 名前
	/// </summary>
	string name;

	DirectX::XMFLOAT2 pad;
	//ローカルスケール
	DirectX::XMVECTOR scaling = { 1.0f,1.0f,1.0f,0.0f };
	//ローカル回転角
	DirectX::XMVECTOR rotation = { 0.0f,0.0f,0.0f,0.0f };
	//ローカル移動
	DirectX::XMVECTOR translation = { 0.0f,0.0f,0.0f,1.0f };
	//ローカル変形行列
	DirectX::XMMATRIX transform;
	//グローバル変形行列
	DirectX::XMMATRIX globalTransform;
	//親ノード
	Node *parent = nullptr;
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
		DirectX::XMMATRIX invInitialPose;
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
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 uv;
		UINT boneIndex[MAX_BONE_INDICES];
		float boneWeight[MAX_BONE_INDICES];
	};	

	string name;
	vector<Node>nodes;
	vector<Bone>bones;


	Node *meshNode = nullptr;
	vector<VertexPosNormalUvSkin>vertices;
	vector<unsigned short>indices;


	DirectX::XMFLOAT3 ambient = { 1.0f,1.0f,1.0f };
	DirectX::XMFLOAT3 diffuse = { 1.0f,1.0f,1.0f };
	vector<RESOURCE_HANDLE> textureHandle;
public:
	friend class FbxModelResourceMgr;

	vector<Bone> &GetBone()
	{
		return bones;
	};
};


struct FbxResourceData
{
public:
	std::unique_ptr<CreateGpuBuffer> buffers;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	std::vector<RESOURCE_HANDLE> textureHandle;
	UINT indicisNum;
	DirectX::XMFLOAT3 ambient;
	DirectX::XMFLOAT3 diffuse;
	std::vector<Model::Bone> bone;

	std::vector<FbxTime> startTime;
	std::vector<FbxTime> endTime;

	FbxResourceData() :indicisNum(0), ambient({ -1.0f,-1.0f,-1.0f }), diffuse({ -1.0f,-1.0f,-1.0f })
	{
	}
};

class FbxModelResourceMgr:public ISingleton<FbxModelResourceMgr>
{
public:
	FbxModelResourceMgr();
	~FbxModelResourceMgr();

	RESOURCE_HANDLE LoadModel(const string &MODEL_NAME);

	const std::shared_ptr<FbxResourceData> &GetResourceData(RESOURCE_HANDLE HANDLE);

private:
	FbxManager *fbxManager;
	FbxImporter *fbxImporter;
	FbxScene *fbxScene;

	std::vector<std::shared_ptr<FbxResourceData>>modelResource;
	std::shared_ptr<FbxResourceData> errorResource;
	std::unique_ptr<HandleMaker> handle;


	/// <summary>
	/// 再帰的にノード構成を解析
	/// </summary>
	/// <param name="MODEL">読み込み先のモデルオブジェクト</param>
	/// <param name="FBX_NODE">解析対象のノード</param>
	void ParseNodeRecursive(Model *MODEL, FbxNode *FBX_NODE, Node *PARENT = nullptr);

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


	DirectX::XMVECTOR FbxDoubleToXMVECTOR(const FbxDouble3 &DOUBLE_3);

	string ExtractFileName(const string &PATH);

};

