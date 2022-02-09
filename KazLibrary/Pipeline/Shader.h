#pragma once
#include"../DirectXCommon/Base.h"
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include<atlstr.h>
#include"../Helper/OutPutDebugStringAndCheckResult.h"

enum ShaderType
{
	SHADER_TYPE_NONE = -1,
	SHADER_TYPE_VERTEX,
	SHADER_TYPE_PIXEL,
	SHADER_TYPE_GEOMETORY,
	SHADER_TYPE_COMPUTE,
	SHADER_TYPE_MAX,
};

/// <summary>
/// �V�F�[�_�[�̃t�@�C���ǂݍ��݂ƃR���p�C�������܂�
/// </summary>
class Shader 
{
public:
	Shader();

	/// <summary>
	/// ���_�V�F�[�_�[�̃R���p�C��
	/// </summary>
	/// <param name="SHADER_FILE">�V�F�[�_�[�̃t�@�C����</param>
	/// <param name="ENTRY_POINT">�G���g���[�|�C���g</param>
	/// <param name="SHADER_MODEL">�V�F�[�_�[���f��</param>
	void CompileVertexShader(LPCWSTR SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL);

	/// <summary>
	/// �s�N�Z���V�F�[�_�[�̃R���p�C��
	/// </summary>
	/// <param name="SHADER_FILE">�V�F�[�_�[�̃t�@�C����</param>
	/// <param name="ENTRY_POINT">�G���g���[�|�C���g</param>
	/// <param name="SHADER_MODEL">�V�F�[�_�[���f��</param>
	void CompilePixcelShader(LPCWSTR SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL);

	/// <summary>
	/// �W�I���g���[�V�F�[�_�[�̃R���p�C��
	/// </summary>
	/// <param name="SHADER_FILE">�V�F�[�_�[�̃t�@�C����</param>
	/// <param name="ENTRY_POINT">�G���g���[�|�C���g</param>
	/// <param name="SHADER_MODEL">�V�F�[�_�[���f��</param>
	void CompileGeometoryShader(LPCWSTR SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL);


	//���_�V�F�[�_�[
	ComPtr<ID3DBlob> vsBlob;
	//�s�N�Z���V�F�[�_�[
	ComPtr<ID3DBlob> psBlob;
	//�W�I���g���[�V�F�[�_�[
	ComPtr<ID3DBlob> geoBlob;
	
	//�V�̐�
	void CompileShader(string SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL, ShaderType SHADER_TYPE);
	ID3DBlob* GetShaderData(ShaderType SHADER_TYPE);

	//�G���[
	ComPtr<ID3DBlob> errorBlob;
private:
	HRESULT result;
	void Error();

	bool vertexShaderWasCompiledFlag;
	bool pixcelShaderWasCompiledFlag;

	vector< ComPtr<ID3DBlob>> shaderBlobs;

	
};

//LPCWSTR
//LPCSTR
