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
/// �m�[�h����̏��
/// </summary>
struct NewNode
{
	/// <summary>
	/// ���O
	/// </summary>
	string name;
	//���[�J���X�P�[��
	XMVECTOR scaling = { 1.0f,1.0f,1.0f,0.0f };
	//���[�J����]�p
	XMVECTOR rotation = { 0.0f,0.0f,0.0f,0.0f };
	//���[�J���ړ�
	XMVECTOR translation = { 0.0f,0.0f,0.0f,1.0f };
	//���[�J���ό`�s��
	XMMATRIX transform;
	//�O���[�o���ό`�s��
	XMMATRIX globalTransform;
	//�e�m�[�h
	NewNode *parent = nullptr;
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
		XMMATRIX invInitialPose;
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
	/// �ċA�I�Ƀm�[�h�\�������
	/// </summary>
	/// <param name="MODEL">�ǂݍ��ݐ�̃��f���I�u�W�F�N�g</param>
	/// <param name="FBX_NODE">��͑Ώۂ̃m�[�h</param>
	void ParseNodeRecursive(Model *MODEL, FbxNode *FBX_NODE, NewNode *PARENT = nullptr);

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


	XMVECTOR FbxDoubleToXMVECTOR(const FbxDouble3 &DOUBLE_3);

	string ExtractFileName(const string &PATH);

	void ConvertMatrixFromFbx(XMMATRIX *DST, const FbxAMatrix &SRC);

};

