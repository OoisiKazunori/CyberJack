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
/// シェーダーのファイル読み込みとコンパイルをします
/// </summary>
class Shader
{
public:
	Shader();

	//新体制
	void CompileShader(std::string SHADER_FILE, std::string ENTRY_POINT, std::string SHADER_MODEL, ShaderType SHADER_TYPE);
	void CompileShader2(std::string SHADER_FILE, std::string ENTRY_POINT, std::string SHADER_MODEL, ShaderType SHADER_TYPE);

	IDxcBlob *GetShaderData(ShaderType SHADER_TYPE);

	//エラー
	ComPtr<ID3DBlob> errorBlob;

	std::vector<char> shaderBin;
private:
	HRESULT result;
	void Error();


	vector<ComPtr<IDxcBlob>> shaderBlobs;



	std::wstring StringToWString(std::string oString)
	{
		// SJIS → wstring
		int iBufferSize = MultiByteToWideChar(CP_ACP, 0, oString.c_str()
			, -1, (wchar_t *)NULL, 0);

		// バッファの取得
		wchar_t *cpUCS2 = new wchar_t[iBufferSize];

		// SJIS → wstring
		MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, cpUCS2
			, iBufferSize);

		// stringの生成
		std::wstring oRet(cpUCS2, cpUCS2 + iBufferSize - 1);

		// バッファの破棄
		delete[] cpUCS2;

		// 変換結果を返す
		return(oRet);
	}

};

//LPCWSTR
//LPCSTR
