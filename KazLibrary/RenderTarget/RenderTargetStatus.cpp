#include "RenderTargetStatus.h"
#include"../DirectXCommon/DirectX12Device.h"
#include"../DirectXCommon/DirectX12CmdList.h"
#include"../Buffer/DescriptorHeapMgr.h"
#include<algorithm>

RenderTargetStatus::RenderTargetStatus()
{
	buffers = std::make_unique<CreateGpuBuffer>();

	buffers->handle->SetHandleSize(DescriptorHeapMgr::Instance()->GetSize(DESCRIPTORHEAP_MEMORY_TEXTURE_RENDERTARGET));
}

RenderTargetStatus::~RenderTargetStatus()
{
}

void RenderTargetStatus::CreateDoubleBuffer(ComPtr<IDXGISwapChain4> SWAPCHAIN)
{
	//�_�u���o�b�t�@�����O�p
	HRESULT result;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;	//�����_�[�^�[�Q�b�g�r���[
	heapDesc.NumDescriptors = 2;					//�ő��
	DirectX12Device::Instance()->dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));


	//���\�̓��
	std::vector<ComPtr<ID3D12Resource>> backBuffer(2);
	for (int i = 0; i < 2; i++)
	{
		//�X���b�v�`�F�[������o�b�t�@���擾
		result = SWAPCHAIN->GetBuffer(i, IID_PPV_ARGS(&backBuffer[i]));
		backBuffer[i]->SetName(L"RenderTarget");

		//�f�B�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		backBuffers.push_back(backBuffer[i]);

		//�����\���ŃA�h���X�������
		handle.ptr += i * DirectX12Device::Instance()->dev->GetDescriptorHandleIncrementSize(heapDesc.Type);

		//�����_�[�^�[�Q�b�g�r���[�̐���
		DirectX12Device::Instance()->dev->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			handle
		);
	}

	copySwapchain = SWAPCHAIN.Get();
	copyBuffer = backBuffers[0].Get();

	handle = gDepth.CreateBuffer();
	handle2 = gDepth.CreateBuffer();



	//RenderTarget�p�̃q�[�v
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;	//�����_�[�^�[�Q�b�g�r���[
	heapDesc.NumDescriptors = 100;
	//�}���`�p�X�����_�����O�p�̃f�X�N���v�^�q�[�v����
	DirectX12Device::Instance()->dev->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(multiPassRTVHeap.ReleaseAndGetAddressOf())
	);
}

void RenderTargetStatus::SetDoubleBufferFlame(XMFLOAT3 COLOR)
{
	gDepth.Clear(handle);

	bbIndex = copySwapchain->GetCurrentBackBufferIndex();
	//�o���A�؂�ւ�
	ChangeBarrier(backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	//�����_�[�^�Q�b�g�̐ݒ�
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH;
	rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIndex * DirectX12Device::Instance()->dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
	DirectX12CmdList::Instance()->cmdList->OMSetRenderTargets(1, &rtvH, false, &gDepth.dsvH[handle]);

}

void RenderTargetStatus::ClearDoubuleBuffer(XMFLOAT3 COLOR)
{
	//�����_�[�^�Q�b�g�̃N���A
	float ClearColor[] = { COLOR.x / 255.0f,COLOR.y / 255.0f,COLOR.z / 255.0f, 1.0f };
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH;
	rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIndex * DirectX12Device::Instance()->dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
	DirectX12CmdList::Instance()->cmdList->ClearRenderTargetView(rtvH, ClearColor, 0, nullptr);

	CD3DX12_RECT rect(0, 0, static_cast<long>(WIN_X), static_cast<long>(WIN_Y));
	CD3DX12_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(WIN_X), static_cast<float>(WIN_Y));
	DirectX12CmdList::Instance()->cmdList->RSSetViewports(1, &viewport);
	DirectX12CmdList::Instance()->cmdList->RSSetScissorRects(1, &rect);
}

void RenderTargetStatus::SwapResourceBarrier()
{
	ChangeBarrier(backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
}

void RenderTargetStatus::PrepareToChangeBarrier(const short &OPEN_RENDERTARGET_HANDLE, const short &CLOSE_RENDERTARGET_HANDLE)
{
	std::vector<short> openRendertargetPass = CountPass(OPEN_RENDERTARGET_HANDLE);


	if (CLOSE_RENDERTARGET_HANDLE == -1)
	{
		//�_�u���o�b�t�@�����O�p�̃o���A�����
		ChangeBarrier(backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	}
	else
	{
		std::vector<short>  closeRendertargetPass = CountPass(CLOSE_RENDERTARGET_HANDLE);

		for (int i = 0; i < closeRendertargetPass.size(); ++i)
		{
			//�w��̃����_�[�^�Q�b�g�����
			ChangeBarrier(buffers->GetBufferData(closeRendertargetPass[i]).Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		}
	}


	for (int i = 0; i < openRendertargetPass.size(); ++i)
	{
		//�w��̃����_�[�^�Q�b�g�Ńo���A��������
		ChangeBarrier(buffers->GetBufferData(openRendertargetPass[i]).Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	}


	//�����_�[�^�Q�b�g�̐ݒ�
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>rtvHs;
	for (int i = 0; i < openRendertargetPass.size(); ++i)
	{
		unsigned int handle = buffers->handle->CaluNowHandle(openRendertargetPass[i]);
		D3D12_CPU_DESCRIPTOR_HANDLE rtvH = multiPassRTVHeap->GetCPUDescriptorHandleForHeapStart();
		rtvH.ptr += DirectX12Device::Instance()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * handle;
		rtvHs.push_back(rtvH);
	}

	DirectX12CmdList::Instance()->cmdList->OMSetRenderTargets(static_cast<UINT>(openRendertargetPass.size()), rtvHs.data(), false, &gDepth.dsvH[handle]);
}

void RenderTargetStatus::PrepareToCloseBarrier(const short &RENDERTARGET_HANDLE)
{
	std::vector<short>  closeRendertargetPass = CountPass(RENDERTARGET_HANDLE);
	for (int i = 0; i < closeRendertargetPass.size(); ++i)
	{
		ChangeBarrier(buffers->GetBufferData(closeRendertargetPass[i]).Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	}
}

void RenderTargetStatus::PrepareToChangeBarrier(const short &RENDERTARGET, int DEPHT_HANDLE)
{
	//�_�u���o�b�t�@�����O�p�̃o���A�����
	ChangeBarrier(backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	gDepth.Clear(DEPHT_HANDLE);

	//�w��̃����_�[�^�Q�b�g�Ńo���A��������
	ChangeBarrier(buffers->GetBufferData(0).Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);


	//�����_�[�^�Q�b�g�̐ݒ�ƃN���A
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH;
	rtvH = multiPassRTVHeap->GetCPUDescriptorHandleForHeapStart();
	for (int i = 0; i < 0; i++)
	{
		rtvH.ptr += DirectX12Device::Instance()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	DirectX12CmdList::Instance()->cmdList->OMSetRenderTargets(1, &rtvH, false, &gDepth.dsvH[DEPHT_HANDLE]);
	float ClearColor[] = { clearColors[0].x,clearColors[0].y ,clearColors[0].z ,clearColors[0].w };
	DirectX12CmdList::Instance()->cmdList->ClearRenderTargetView(rtvH, ClearColor, 0, nullptr);
}

void RenderTargetStatus::ClearRenderTarget(const short &RENDERTARGET_HANDLE)
{
	//�����_�[�^�Q�b�g�̃N���A
	gDepth.Clear(handle);
	//�����_�[�^�Q�b�g�̐ݒ�

	std::vector<short>  openHandle = CountPass(RENDERTARGET_HANDLE);
	for (int i = 0; i < openHandle.size(); ++i)
	{
		unsigned int handle = static_cast<unsigned int>(buffers->handle->CaluNowHandle(openHandle[i]));
		D3D12_CPU_DESCRIPTOR_HANDLE rtvH = multiPassRTVHeap->GetCPUDescriptorHandleForHeapStart();
		rtvH.ptr += DirectX12Device::Instance()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * handle;

		short clearHandle = buffers->handle->CaluNowHandle(openHandle[i]);
		float ClearColor[] = { clearColors[clearHandle].x,clearColors[clearHandle].y ,clearColors[clearHandle].z ,clearColors[clearHandle].w };
		DirectX12CmdList::Instance()->cmdList->ClearRenderTargetView(rtvH, ClearColor, 0, nullptr);

		XMUINT2 graphSize =
		{
			static_cast<uint32_t>(buffers->GetBufferData(openHandle[i])->GetDesc().Width),
			static_cast<uint32_t>(buffers->GetBufferData(openHandle[i])->GetDesc().Height)
		};
		CD3DX12_RECT rect(0, 0, graphSize.x, graphSize.y);
		CD3DX12_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(graphSize.x), static_cast<float>(graphSize.y));
		DirectX12CmdList::Instance()->cmdList->RSSetViewports(static_cast<UINT>(openHandle.size()), &viewport);
		DirectX12CmdList::Instance()->cmdList->RSSetScissorRects(static_cast<UINT>(openHandle.size()), &rect);
	}
}

short RenderTargetStatus::CreateRenderTarget(const KazMath::Vec2<UINT> &GRAPH_SIZE, const XMFLOAT3 &CLEAR_COLOR, const DXGI_FORMAT &FORMAT)
{
	//�r���[�̐���
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = FORMAT;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;



	D3D12_RESOURCE_DESC resource = RenderTargetStatus::Instance()->copyBuffer->GetDesc();
	resource.Height = GRAPH_SIZE.y;
	resource.Width = static_cast<UINT64>(GRAPH_SIZE.x);
	resource.Format = FORMAT;
	float clearValue[] = { CLEAR_COLOR.x / 255.0f,CLEAR_COLOR.y / 255.0f ,CLEAR_COLOR.z / 255.0f,1.0f };
	D3D12_CLEAR_VALUE clearColor = CD3DX12_CLEAR_VALUE(FORMAT, clearValue);



	KazBufferHelper::BufferResourceData data
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		resource,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearColor,
		"ShaderResourceRenderTarget"
	);
	short num = buffers->CreateBuffer(data);


	DescriptorHeapMgr::Instance()->CreateBufferView(num, srvDesc, buffers->GetBufferData(num).Get());
	clearColors[buffers->handle->CaluNowHandle(num)] = { clearValue[0],clearValue[1],clearValue[2],clearValue[3] };


	multiPassRTVHanlde = multiPassRTVHeap->GetCPUDescriptorHandleForHeapStart();
	for (int i = 0; i < buffers->handle->CaluNowHandle(num); i++)
	{
		multiPassRTVHanlde.ptr
			+= DirectX12Device::Instance()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	//RTV�̍쐬
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = FORMAT;
	//SR���g����RTV�̐���
	DirectX12Device::Instance()->dev->CreateRenderTargetView(
		buffers->GetBufferData(num).Get(),
		&rtvDesc,
		multiPassRTVHanlde
	);

	vector<short> handle;
	handle.push_back(num);
	renderTargetData[buffers->handle->CaluNowHandle(num)] = handle;
	return num;
}

std::vector<short> RenderTargetStatus::CreateMultiRenderTarget(const std::vector<MultiRenderTargetData> &MULTIRENDER_TARGET_DATA, const DXGI_FORMAT &FORMAT)
{
	//�r���[�̐���
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = FORMAT;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	const int ARRAY_SIZE = static_cast<int>(MULTIRENDER_TARGET_DATA.size());
	std::vector<short> handles;

	short buffNum = -1;
	for (int i = 0; i < MULTIRENDER_TARGET_DATA.size(); ++i)
	{
		D3D12_RESOURCE_DESC resource = RenderTargetStatus::Instance()->copyBuffer->GetDesc();
		resource.Height = MULTIRENDER_TARGET_DATA[i].graphSize.y;
		resource.Width = static_cast<UINT64>(MULTIRENDER_TARGET_DATA[i].graphSize.x);
		resource.Format = FORMAT;
		float clearValue[] = { MULTIRENDER_TARGET_DATA[i].backGroundColor.x / 255.0f,MULTIRENDER_TARGET_DATA[i].backGroundColor.y / 255.0f ,MULTIRENDER_TARGET_DATA[i].backGroundColor.z / 255.0f,1.0f };
		D3D12_CLEAR_VALUE clearColor = CD3DX12_CLEAR_VALUE(FORMAT, clearValue);



		KazBufferHelper::BufferResourceData data
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			resource,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearColor,
			"ShaderResourceRenderTarget"
		);
		buffNum = buffers->CreateBuffer(data);


		//DescriptorHeapMgr::Instance()->CreateBufferView(num, rtvDesc, buffers->GetBufferData(num).Get());
		DescriptorHeapMgr::Instance()->CreateBufferView(buffNum, srvDesc, buffers->GetBufferData(buffNum).Get());
		clearColors[buffers->handle->CaluNowHandle(buffNum)] = { clearValue[0],clearValue[1],clearValue[2],clearValue[3] };


		multiPassRTVHanlde = multiPassRTVHeap->GetCPUDescriptorHandleForHeapStart();
		for (int bufferNum = 0; bufferNum < buffers->handle->CaluNowHandle(buffNum); bufferNum++)
		{
			multiPassRTVHanlde.ptr
				+= DirectX12Device::Instance()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		}

		//RTV�̍쐬
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Format = FORMAT;
		//SR���g����RTV�̐���
		DirectX12Device::Instance()->dev->CreateRenderTargetView(
			buffers->GetBufferData(buffNum).Get(),
			&rtvDesc,
			multiPassRTVHanlde
		);


		handles.push_back(buffNum);
	}

	renderTargetData[buffers->handle->CaluNowHandle(buffNum)] = handles;
	return handles;
}

ID3D12Resource *RenderTargetStatus::GetBufferData(short HANDLE)const
{
	return buffers->GetBufferData(HANDLE).Get();
}

D3D12_GPU_DESCRIPTOR_HANDLE RenderTargetStatus::GetViewData(short HANDLE)
{
	return DescriptorHeapMgr::Instance()->GetGpuDescriptorView(HANDLE);
}

void RenderTargetStatus::DeleteRenderTarget(short HANDLE)
{
	buffers->ReleaseBuffer(HANDLE);
	DescriptorHeapMgr::Instance()->Release(HANDLE);

	for (int i = 0; i < renderTargetData.size(); ++i)
	{
		for (int renderTargetNum = 0; renderTargetNum < renderTargetData[i].size(); ++renderTargetNum)
		{
			if (renderTargetData[i][renderTargetNum] == HANDLE)
			{
				renderTargetData[i].clear();
				renderTargetData[i].shrink_to_fit();
				return;
			}
		}
	}
	
}

void RenderTargetStatus::DeleteMultiRenderTarget(const std::vector<short> &HANDLE)
{
	for (int i = 0; i < HANDLE.size(); ++i)
	{
		buffers->ReleaseBuffer(HANDLE[i]);
		DescriptorHeapMgr::Instance()->Release(HANDLE[i]);
	}


	for (int i = 0; i < renderTargetData.size(); ++i)
	{
		for (int renderTargetNum = 0; renderTargetNum < renderTargetData[i].size(); ++renderTargetNum)
		{
			if (renderTargetData[i][renderTargetNum] == HANDLE[0])
			{
				renderTargetData[i].clear();
				renderTargetData[i].shrink_to_fit();
				return;
			}
		}
	}
}

void RenderTargetStatus::ChangeBarrier(ID3D12Resource *RESOURCE, const D3D12_RESOURCE_STATES &BEFORE_STATE, const D3D12_RESOURCE_STATES &AFTER_STATE)
{
	D3D12_RESOURCE_BARRIER barrier =
		CD3DX12_RESOURCE_BARRIER::Transition(
			RESOURCE,
			BEFORE_STATE,
			AFTER_STATE
		);
	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(1, &barrier);
}

std::vector<short> RenderTargetStatus::CountPass(short HANDLE)
{
	for (int handleNum = 0; handleNum < renderTargetData.size(); ++handleNum)
	{
		for (int renderTargetNum = 0; renderTargetNum < renderTargetData[handleNum].size(); ++renderTargetNum)
		{
			if (renderTargetData[handleNum][renderTargetNum] == HANDLE)
			{
				return renderTargetData[handleNum];
			}
		}
	}
	assert(0);
	return std::vector<short>();
}
