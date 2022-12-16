#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Pipeline/GraphicsPipeLineMgr.h"
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"

struct InitExcuteIndirect
{
	std::vector<D3D12_INDIRECT_ARGUMENT_DESC> argument;
	D3D12_GPU_VIRTUAL_ADDRESS updateView;
	UINT elementNum;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	UINT indexNum;
	RootSignatureMode rootsignatureName;
};

/// <summary>
/// ExcuteIndirectを簡単に使用する為のクラス
/// </summary>
class DrawExcuteIndirect
{
public:
	DrawExcuteIndirect(const InitExcuteIndirect &INIT_DATA);
	void Draw(PipeLineNames PIPELINE_NAME);

private:

	struct IndirectCommand
	{
		D3D12_GPU_VIRTUAL_ADDRESS view;
		D3D12_DRAW_INDEXED_ARGUMENTS drawArguments;
	};

	Microsoft::WRL::ComPtr<ID3D12CommandSignature> commandSig;
	CreateGpuBuffer buffers;
	RESOURCE_HANDLE drawCommandHandle;

	InitExcuteIndirect initData;

	D3D12_INDIRECT_ARGUMENT_TYPE drawType;
};

