#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/OutPutDebugStringAndCheckResult.h"

#include<atlstr.h>

#include <dxcapi.h>
#pragma comment(lib, "lib/shader/dxcompiler.lib")
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
	IDxcBlob* GetShaderData(ShaderType SHADER_TYPE);

	//エラー
	ComPtr<ID3DBlob> errorBlob;
private:
	HRESULT result;
	void Error();


	vector<ComPtr<IDxcBlob>> shaderBlobs;

	
};

//LPCWSTR
//LPCSTR
