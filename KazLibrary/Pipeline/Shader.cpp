#include "Shader.h"
#include"../Helper/KazHelper.h"
#include"../DirectXCommon/DirectX12Device.h"

Shader::Shader()
{
	shaderBlobs.resize(SHADER_TYPE_MAX);
}

void Shader::CompileShader(std::string SHADER_FILE, std::string ENTRY_POINT, std::string SHADER_MODEL, ShaderType SHADER_TYPE)
{
	//コンパイルの準備-------------------------
	CComPtr<IDxcLibrary> library;
	HRESULT hr = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));

	CComPtr<IDxcCompiler> compiler;
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));

	wchar_t lShaderFile[128];
	KazHelper::ConvertStringToWchar_t(SHADER_FILE, lShaderFile, 128);

	uint32_t codePage = CP_UTF8;
	CComPtr<IDxcBlobEncoding> sourceBlob;
	hr = library->CreateBlobFromFile(lShaderFile, &codePage, &sourceBlob);
	//コンパイルの準備-------------------------


	//コンパイル-------------------------
	wchar_t lEntryPoint[128];
	KazHelper::ConvertStringToWchar_t(ENTRY_POINT, lEntryPoint, 128);

	wchar_t lShaderModel[128];
	KazHelper::ConvertStringToWchar_t(SHADER_MODEL, lShaderModel, 128);


	CComPtr<IDxcOperationResult> result;
	hr = compiler->Compile(
		sourceBlob,		// pSource
		lShaderFile,		// pSourceName
		lEntryPoint,		// pEntryPoint
		lShaderModel,	// pTargetProfile
		NULL, 0,		// pArguments, argCount
		NULL, 0,		// pDefines, defineCount
		NULL,			// pIncludeHandler
		&result);		// ppResult

	if (SUCCEEDED(hr))
	{
		result->GetStatus(&hr);
	}
	//コンパイル-------------------------

	//エラー処理-------------------------
	if (FAILED(hr))
	{
		if (result)
		{
			CComPtr<IDxcBlobEncoding> errorsBlob;
			hr = result->GetErrorBuffer(&errorsBlob);
			if (SUCCEEDED(hr) && errorsBlob)
			{
				wprintf(L"Compilation failed with errors:\n%hs\n",
					(const char *)errorsBlob->GetBufferPointer());
			}
		}
		// Handle compilation error...
	}
	//エラー処理-------------------------

	CComPtr<IDxcBlob> code;
	result->GetResult(&code);


	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(code->GetBufferPointer(), code->GetBufferSize());
	CComPtr<ID3D12PipelineState> pso;
	hr = DirectX12Device::Instance()->dev->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pso));

}

IDxcBlob* Shader::GetShaderData(ShaderType SHADER_TYPE)
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
