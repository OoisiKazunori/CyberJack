#include "Shader.h"
#include"../Helper/KazHelper.h"

Shader::Shader()
{
	vsBlob = nullptr;
	psBlob = nullptr;
	errorBlob = nullptr;

	pixcelShaderWasCompiledFlag = false;
	vertexShaderWasCompiledFlag = false;

	shaderBlobs.resize(SHADER_TYPE_MAX);
}

void Shader::CompileVertexShader(LPCWSTR SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL) {

	if (!vertexShaderWasCompiledFlag && !pixcelShaderWasCompiledFlag) {
		OutPutStartDebugString("シェーダーのコンパイル");
	}


	result = D3DCompileFromFile(
		//LPCWSTR...wchar_t const *
		SHADER_FILE,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		//LPCSTR...const char*
		ENTRY_POINT,
		//LPCSTR...const char*
		SHADER_MODEL,
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob,
		&errorBlob
	);

	if (CheckResult("頂点シェーダーのコンパイル", result) != S_OK) {
		Error();
	}
	else {
		vertexShaderWasCompiledFlag = true;
	}


	if (vertexShaderWasCompiledFlag && pixcelShaderWasCompiledFlag) {
		OutPutEndDebugString("シェーダーのコンパイル");
	}

}

void Shader::CompilePixcelShader(LPCWSTR SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL) {

	if (!vertexShaderWasCompiledFlag && !pixcelShaderWasCompiledFlag) {
		OutPutStartDebugString("シェーダーのコンパイル");
	}

	result = D3DCompileFromFile(
		SHADER_FILE,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		ENTRY_POINT,
		SHADER_MODEL,
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob,
		&errorBlob
	);

	if (CheckResult("ピクセルシェーダーのコンパイル", result) != S_OK) {
		Error();
	}
	else {
		pixcelShaderWasCompiledFlag = true;
	}

	if (vertexShaderWasCompiledFlag && pixcelShaderWasCompiledFlag) {
		OutPutEndDebugString("シェーダーのコンパイル");
	}
}

void Shader::CompileGeometoryShader(LPCWSTR SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL)
{

	if (!vertexShaderWasCompiledFlag && !pixcelShaderWasCompiledFlag) {
		OutPutStartDebugString("シェーダーのコンパイル");
	}

	result = D3DCompileFromFile(
		SHADER_FILE,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		ENTRY_POINT,
		SHADER_MODEL,
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&geoBlob,
		&errorBlob
	);

	if (CheckResult("ピクセルシェーダーのコンパイル", result) != S_OK) {
		Error();
	}
	else {
		pixcelShaderWasCompiledFlag = true;
	}

	if (vertexShaderWasCompiledFlag && pixcelShaderWasCompiledFlag) {
		OutPutEndDebugString("シェーダーのコンパイル");
	}
}

void Shader::CompileShader(string SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL, ShaderType SHADER_TYPE)
{
	wchar_t shaderFile[128];
	KazHelper::ConvertStringToWchar_t(SHADER_FILE, shaderFile, 128);

	result = D3DCompileFromFile(
		shaderFile,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		ENTRY_POINT,
		SHADER_MODEL,
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&shaderBlobs[SHADER_TYPE],
		&errorBlob
	);

	if (CheckResult(SHADER_FILE + "のコンパイル", result) != S_OK)
	{
		Error();
	}
}

ID3DBlob* Shader::GetShaderData(ShaderType SHADER_TYPE)
{
	return shaderBlobs[SHADER_TYPE].Get();
}

void Shader::Error() 
{
	std::string errstr;
	errstr.resize(errorBlob->GetBufferSize());

	std::copy_n((char *)errorBlob->GetBufferPointer(),
		errorBlob->GetBufferSize(),
		errstr.begin());

	errstr += "\n";

	FailCheck(errstr.c_str());
	exit(1);
}
