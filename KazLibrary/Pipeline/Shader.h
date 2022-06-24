#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/OutPutDebugStringAndCheckResult.h"

#include<atlstr.h>

#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

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

	//�V�̐�
	void CompileShader(std::string SHADER_FILE, std::string ENTRY_POINT, std::string SHADER_MODEL, ShaderType SHADER_TYPE);
	void CompileShader2(std::string SHADER_FILE, std::string ENTRY_POINT, std::string SHADER_MODEL, ShaderType SHADER_TYPE);

	IDxcBlob *GetShaderData(ShaderType SHADER_TYPE);

	//�G���[
	ComPtr<ID3DBlob> errorBlob;

	std::vector<char> shaderBin;
private:
	HRESULT result;
	void Error();


	vector<ComPtr<IDxcBlob>> shaderBlobs;



	std::wstring StringToWString(std::string oString)
	{
		// SJIS �� wstring
		int iBufferSize = MultiByteToWideChar(CP_ACP, 0, oString.c_str()
			, -1, (wchar_t *)NULL, 0);

		// �o�b�t�@�̎擾
		wchar_t *cpUCS2 = new wchar_t[iBufferSize];

		// SJIS �� wstring
		MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, cpUCS2
			, iBufferSize);

		// string�̐���
		std::wstring oRet(cpUCS2, cpUCS2 + iBufferSize - 1);

		// �o�b�t�@�̔j��
		delete[] cpUCS2;

		// �ϊ����ʂ�Ԃ�
		return(oRet);
	}

};

//LPCWSTR
//LPCSTR
