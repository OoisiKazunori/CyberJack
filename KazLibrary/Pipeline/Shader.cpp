#include "Shader.h"
#include"../Helper/KazHelper.h"
#include"../DirectXCommon/DirectX12Device.h"
#include<Windows.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader()
{
	shaderBlobs.resize(SHADER_TYPE_MAX);
}

void Shader::CompileShader(std::string SHADER_FILE, std::string ENTRY_POINT, std::string SHADER_MODEL, ShaderType SHADER_TYPE)
{
	//�R���p�C���̏���-------------------------
	ComPtr<IDxcLibrary> library;
	HRESULT hr = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));

	ComPtr<IDxcCompiler> compiler;
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));

	std::array<wchar_t, 128> lShaderFile;
	KazHelper::ConvertStringToWchar_t(SHADER_FILE, lShaderFile.data(), lShaderFile.size());

	uint32_t codePage = CP_UTF8;
	ComPtr<IDxcBlobEncoding> sourceBlob;
	hr = library->CreateBlobFromFile(lShaderFile.data(), &codePage, &sourceBlob);
	//�R���p�C���̏���-------------------------



	//�R���p�C��-------------------------
	std::array<wchar_t, 128> lEntryPoint;
	KazHelper::ConvertStringToWchar_t(ENTRY_POINT, lEntryPoint.data(), lEntryPoint.size());

	std::array<wchar_t, 128> lShaderModel;
	KazHelper::ConvertStringToWchar_t(SHADER_MODEL, lShaderModel.data(), lShaderModel.size());


	//Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler;
	//hr = library->includeHandler(&includeHandler);
	//DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&));


	ComPtr<IDxcOperationResult> result;
	hr = compiler->Compile(
		sourceBlob.Get(),		// pSource
		lShaderFile.data(),		// pSourceName
		lEntryPoint.data(),		// pEntryPoint
		lShaderModel.data(),	// pTargetProfile
		nullptr, 0,				// pArguments, argCount
		nullptr, 0,				// pDefines, defineCount
		nullptr,	// pIncludeHandler
		&result);				// ppResult

	if (SUCCEEDED(hr))
	{
		result->GetStatus(&hr);
	}
	//�R���p�C��-------------------------

	//�G���[����-------------------------
	if (FAILED(hr))
	{
		if (result)
		{
			CComPtr<IDxcBlobEncoding> errorsBlob;
			hr = result->GetErrorBuffer(&errorsBlob);
			if (SUCCEEDED(hr) && errorsBlob)
			{
				wprintf(L"Compilation failed with errors:\n%hs\n",
					static_cast<const char *>(errorsBlob->GetBufferPointer()));
				//assert(0);
			}
		}
		// Handle compilation error...
	}
	//�G���[����-------------------------

	CComPtr<IDxcBlob> code;
	result->GetResult(&shaderBlobs[SHADER_TYPE]);


	//D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	//psoDesc.PS = CD3DX12_SHADER_BYTECODE(code->GetBufferPointer(), code->GetBufferSize());
	//CComPtr<ID3D12PipelineState> pso;
	//hr = DirectX12Device::Instance()->dev->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pso));

}

void Shader::CompileShader2(std::string SHADER_FILE, std::string ENTRY_POINT, std::string SHADER_MODEL, ShaderType SHADER_TYPE)
{
	HRESULT hr;
	std::ifstream infile(SHADER_FILE, std::ifstream::binary);
	if (!infile) {
		throw std::runtime_error("failed shader compile.");
	}

	std::wstring fileName = StringToWString(SHADER_FILE);
	std::stringstream strstream;

	strstream << infile.rdbuf();

	std::string shaderCode = strstream.str();
	Microsoft::WRL::ComPtr<IDxcLibrary> library;
	hr = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));
	Microsoft::WRL::ComPtr<IDxcCompiler> compiler;
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
	Microsoft::WRL::ComPtr<IDxcBlobEncoding> source;

	hr = library->CreateBlobWithEncodingFromPinned(
		(LPBYTE)shaderCode.c_str(), (UINT32)shaderCode.size(), CP_UTF8, &source);
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler;

	// �C���N���[�h���g���ꍇ�ɂ͓K�؂ɐݒ肷�邱��.
	hr = library->CreateIncludeHandler(&includeHandler);
	// �R���p�C���I�v�V�����̎w��.
	std::vector<LPCWSTR> arguments;

	std::array<wchar_t, 128> lEntryPoint;
	KazHelper::ConvertStringToWchar_t(ENTRY_POINT, lEntryPoint.data(), lEntryPoint.size());

	std::array<wchar_t, 128> lShaderModel;
	KazHelper::ConvertStringToWchar_t(SHADER_MODEL, lShaderModel.data(), lShaderModel.size());


	arguments.emplace_back(L"/Od");
	const auto target = L"vs_6_4";

	Microsoft::WRL::ComPtr<IDxcOperationResult> dxcResult;
	hr = compiler->Compile(source.Get(), fileName.c_str(),
		lEntryPoint.data(), lShaderModel.data(), arguments.data(), UINT(arguments.size()),
		nullptr, 0, includeHandler.Get(), &dxcResult);

	if (FAILED(hr)) {
		throw std::runtime_error("failed shader compile.");
	}

	dxcResult->GetStatus(&hr);
	if (FAILED(hr)) {
		Microsoft::WRL::ComPtr<IDxcBlobEncoding> errBlob;
		dxcResult->GetErrorBuffer(&errBlob);

		wprintf(L"Compilation failed with errors:\n%hs\n",
			static_cast<const char *>(errBlob->GetBufferPointer()));
		// ... errBlob �̓��e���G���[���b�Z�[�W�Ƃ��ĕ\�� (�ȗ�)
		throw std::runtime_error("failed shader compile");
	}

	Microsoft::WRL::ComPtr<IDxcBlob> blob;
	dxcResult->GetResult(&blob);

	shaderBlobs[SHADER_TYPE] = blob;

	std::vector<char> result;
	auto size = blob->GetBufferSize();
	result.resize(size);
	memcpy(result.data(), blob->GetBufferPointer(), size);
	shaderBin = result;
}

IDxcBlob *Shader::GetShaderData(ShaderType SHADER_TYPE)
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
