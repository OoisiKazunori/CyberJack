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
/// シェーダーのファイル読み込みとコンパイルをします
/// </summary>
class Shader 
{
public:
	Shader();

	/// <summary>
	/// 頂点シェーダーのコンパイル
	/// </summary>
	/// <param name="SHADER_FILE">シェーダーのファイル名</param>
	/// <param name="ENTRY_POINT">エントリーポイント</param>
	/// <param name="SHADER_MODEL">シェーダーモデル</param>
	void CompileVertexShader(LPCWSTR SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL);

	/// <summary>
	/// ピクセルシェーダーのコンパイル
	/// </summary>
	/// <param name="SHADER_FILE">シェーダーのファイル名</param>
	/// <param name="ENTRY_POINT">エントリーポイント</param>
	/// <param name="SHADER_MODEL">シェーダーモデル</param>
	void CompilePixcelShader(LPCWSTR SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL);

	/// <summary>
	/// ジオメトリーシェーダーのコンパイル
	/// </summary>
	/// <param name="SHADER_FILE">シェーダーのファイル名</param>
	/// <param name="ENTRY_POINT">エントリーポイント</param>
	/// <param name="SHADER_MODEL">シェーダーモデル</param>
	void CompileGeometoryShader(LPCWSTR SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL);


	//頂点シェーダー
	ComPtr<ID3DBlob> vsBlob;
	//ピクセルシェーダー
	ComPtr<ID3DBlob> psBlob;
	//ジオメトリーシェーダー
	ComPtr<ID3DBlob> geoBlob;
	
	//新体制
	void CompileShader(string SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL, ShaderType SHADER_TYPE);
	ID3DBlob* GetShaderData(ShaderType SHADER_TYPE);

	//エラー
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
