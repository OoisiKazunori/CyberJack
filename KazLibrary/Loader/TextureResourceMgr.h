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
/// �摜�̕���������ۂɂǂ̍��W����ǂꂭ�炢�̃T�C�Y��UV�؂�������邩�L�^��������
/// </summary>
struct DivGraphData
{
	vector<XMFLOAT2> divLeftUp;
	XMFLOAT2 divSize;
	short handle;
};

/// <summary>
/// �摜�̓ǂݍ��݁A�摜�̊Ǘ�
/// </summary>
class TextureResourceMgr :public ISingleton<TextureResourceMgr> {
public:

	TextureResourceMgr();
	~TextureResourceMgr();

	/// <summary>
	/// �t�@�C���p�X�������ĉ摜�̓ǂݍ��݂��s���܂�
	/// </summary>
	/// <param name="RESOURCE">�ǂݍ��݂����摜�̃t�@�C���p�X</param>
	/// <returns>�n���h��</returns>
	short LoadGraph(std::string RESOURCE);
	
	/// <summary>
	///  �t�@�C���p�X�������ĉ摜��ǂݍ��݁A�������܂�
	/// </summary>
	/// <param name="RESOURCE">�ǂݍ��݂����摜�̃t�@�C���p�X</param>
	/// <param name="ALL_NUM">��������摜�̑���</param>
	/// <param name="DIV_WIDTH_NUM">X���ɉ摜�𕪊����鐔</param>
	/// <param name="DIV_HEIGHT_NUM">Y���ɉ摜�𕪊����鐔</param>
	/// <param name="TEXSIZE_WIDTH">��������摜��X�T�C�Y</param>
	/// <param name="TEXSIZE_HEIGHT">��������摜��Y�T�C�Y</param>
	/// <returns>�n���h��</returns>
	short LoadDivGraph(string RESOURCE, 
		unsigned int ALL_NUM,
		unsigned int DIV_WIDTH_NUM, unsigned int DIV_HEIGHT_NUM,
		unsigned int TEXSIZE_WIDTH, unsigned int TEXSIZE_HEIGHT
	);

	/// <summary>
	/// �ǂݍ��񂾉摜���폜���܂�
	/// </summary>
	/// <param name="HANDLE">�폜�������摜�̃n���h��</param>
	void Release(short HANDLE);

	/// <summary>
	/// �n���h������V�F�[�_�[���\�[�X���R�}���h���X�g�ɐς܂��܂�
	/// </summary>
	/// <param name="GRAPH_HANDLE">�n���h��</param>
	/// <param name="PARAM">���[�g�p����</param>
	/// <param name="TYPE">���[�g�p�����̎��</param>
	void SetSRV(short GRAPH_HANDLE, GraphicsRootSignatureParameter PARAM, GraphicsRootParamType TYPE);


	/// <summary>
	/// �摜�̏���n���܂�
	/// �o�b�t�@�̎擾�Ɏ��s�����ꍇ Width��Height��0���Ԃ���܂�
	/// </summary>
	/// <param name="HANDLE"></param>
	/// <returns></returns>
	D3D12_RESOURCE_DESC GetTextureSize(short HANDLE);

	/// <summary>
	/// ��������摜�̏���n���܂�
	/// �o�b�t�@�̎擾�Ɏ��s�����ꍇ texSize��X��Y��-1������܂�
	/// </summary>
	/// <param name="HANDLE">�n���h��</param>
	/// <returns>��������摜�̏��</returns>
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