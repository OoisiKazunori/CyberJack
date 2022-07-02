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
/// �m�[�h����̏��
/// </summary>
struct Node
{
	/// <summary>
	/// ���O
	/// </summary>
	string name;

	DirectX::XMFLOAT2 pad;
	//���[�J���X�P�[��
	DirectX::XMVECTOR scaling = { 1.0f,1.0f,1.0f,0.0f };
	//���[�J����]�p
	DirectX::XMVECTOR rotation = { 0.0f,0.0f,0.0f,0.0f };
	//���[�J���ړ�
	DirectX::XMVECTOR translation = { 0.0f,0.0f,0.0f,1.0f };
	//���[�J���ό`�s��
	DirectX::XMMATRIX transform;
	//�O���[�o���ό`�s��
	DirectX::XMMATRIX globalTransform;
	//�e�m�[�h
	Node *parent = nullptr;
};

/// <summary>
/// �ǂݎ�������𗬂����ݐ�
/// </summary>
class Model
{
public:
	static const int MAX_BONE_INDICES = 4;

	/// <summary>
	/// �{�[���\����
	/// </summary>
	struct Bone
	{
		//���O
		string name;
		//�����p���̋t�s��
		DirectX::XMMATRIX invInitialPose;
		//�N���X�^�[(FBX�̃{�[�����)
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
	/// �ċA�I�Ƀm�[�h�\�������
	/// </summary>
	/// <param name="MODEL">�ǂݍ��ݐ�̃��f���I�u�W�F�N�g</param>
	/// <param name="FBX_NODE">��͑Ώۂ̃m�[�h</param>
	void ParseNodeRecursive(Model *MODEL, FbxNode *FBX_NODE, Node *PARENT = nullptr);

	/// <summary>
	/// ���b�V���ǂݎ��
	/// </summary>
	/// <param name="MODEL">�ǂݍ��ݐ惂�f���I�u�W�F�N�g</param>
	/// <param name="FBX_NODE">��͑Ώۂ̃m�[�h</param>
	void ParseMesh(Model *MODEL, FbxNode *FBX_NODE);

	//���_���W�ǂݎ��
	void ParseMeshVertices(Model *MODEL, FbxMesh *FBX_MESH);
	//�ʏ��ǂݎ��
	void ParseMeshFaces(Model *MODEL, FbxMesh *FBX_MESH);
	//�}�e���A���ǂݎ��
	void ParseMaterial(Model *MODEL, FbxNode *FBX_NODE);
	//�e�N�X�`���ǂݎ��
	void LoadTexture(Model *MODEL, const string &FULL_PATH);
	//�X�L�j���O���̓ǂݎ��
	void ParseSkin(Model *MODEL, FbxMesh *FBX_MESH);


	DirectX::XMVECTOR FbxDoubleToXMVECTOR(const FbxDouble3 &DOUBLE_3);

	string ExtractFileName(const string &PATH);

};

