#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/HandleMaker.h"
#include"../Helper/ISinglton.h"
#include<fstream>
#include<sstream>
#include"../Buffer/CreateGpuBuffer.h"

struct materia
{
	string name;//�}�e���A����
	XMFLOAT3 ambient;//�A���r�G���g�e���x
	XMFLOAT3 diffuse;//�f�B�t���[�Y�e���x
	XMFLOAT3 specular;//�X�y�L�����[�e���x
	float alpha;//��
	string textureFilename;//�e�N�X�`���t�@�C����

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
/// .Obj�̃��f����`�悷��̂ɕK�v�ȃf�[�^������������
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
/// .obj�ɕK�v�ȃf�[�^��ۊǂ��Aobj��`�悷��ۂɃf�[�^���g���񂷃N���X
/// </summary>
class ObjResourceMgr :public ISingleton<ObjResourceMgr>
{
public:
	ObjResourceMgr();
	~ObjResourceMgr();

	/// <summary>
	/// .obj�̃��f����ǂݍ��݂܂�
	/// .obj��.png��.mtl�̖��O�͓��ꂵ�ĉ�����
	/// </summary>
	/// <param name="RESOURCE">.obj�̃f�[�^������t�@�C���p�X</param>
	/// <param name="TEX_RESOURCE">.mtl�̉摜�f�[�^������t�@�C���p�X</param>
	/// <returns>�n���h��</returns>
	short LoadModel(string RESOURCE);

	/// <summary>
	/// �w��̃��f���f�[�^���폜���܂�
	/// </summary>
	/// <param name="HANDLE">�w��̃n���h��</param>
	void Release(short HANDLE);

	/// <summary>
	/// �w��̃n���h�����烊�\�[�X���擾���܂�
	/// </summary>
	/// <param name="HANDLE">�w��̃n���h��</param>
	ObjResourceData GetResourceData(short HANDLE);

private:
	HandleMaker handle;

	vector<ObjResourceData> resource;
	vector<string> name;

	unique_ptr<CreateGpuBuffer> modelDataBuffers;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="filename">�摜�̃t�@�C����</param>
	/// <param name="resource"></param>
	/// <param name="MTL_RESOURCE"></param>
	/// <returns></returns>
	short LoadMaterial(const string &filename, ObjResourceData *resource, string MTL_RESOURCE);

};
