#include "RenderTargetStatus.h"
#include"../DirectXCommon/DirectX12Device.h"
#include"../DirectXCommon/DirectX12CmdList.h"
#include"../Buffer/DescriptorHeapMgr.h"


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
	ChangeBarrier(backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET, 1);

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

	CD3DX12_RECT rect(0.0f, 0.0f, WIN_X, WIN_Y);
	CD3DX12_VIEWPORT viewport(0.0f, 0.0f, WIN_X, WIN_Y);
	DirectX12CmdList::Instance()->cmdList->RSSetViewports(1, &viewport);
	DirectX12CmdList::Instance()->cmdList->RSSetScissorRects(1, &rect);
}

void RenderTargetStatus::SwapResourceBarrier()
{
	ChangeBarrier(backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT, 1);
}

void RenderTargetStatus::PrepareToChangeBarrier(const short& OPEN_RENDERTARGET_HANDLE, const short& CLOSE_RENDERTARGET_HANDLE)
{
	unsigned int openRendertargetPass = CountPass(OPEN_RENDERTARGET_HANDLE);
	unsigned int closeRendertargetPass = CountPass(CLOSE_RENDERTARGET_HANDLE);


	if (CLOSE_RENDERTARGET_HANDLE == -1)
	{
		//�_�u���o�b�t�@�����O�p�̃o���A�����
		ChangeBarrier(backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT, 1);

	}
	else
	{
		//�w��̃����_�[�^�Q�b�g�����
		ChangeBarrier(buffers->GetBufferData(CLOSE_RENDERTARGET_HANDLE).Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, closeRendertargetPass);
	}

	//�w��̃����_�[�^�Q�b�g�Ńo���A��������
	ChangeBarrier(buffers->GetBufferData(OPEN_RENDERTARGET_HANDLE).Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET, openRendertargetPass);


	//�����_�[�^�Q�b�g�̐ݒ�
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = multiPassRTVHeap->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += DirectX12Device::Instance()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * buffers->handle->CaluNowHandle(OPEN_RENDERTARGET_HANDLE);
	DirectX12CmdList::Instance()->cmdList->OMSetRenderTargets(openRendertargetPass, &rtvH, false, &gDepth.dsvH[handle]);
}

void RenderTargetStatus::PrepareToCloseBarrier(const short& RENDERTARGET_HANDLE)
{
	ChangeBarrier(buffers->GetBufferData(RENDERTARGET_HANDLE).Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, CountPass(RENDERTARGET_HANDLE));
}

void RenderTargetStatus::PrepareToChangeBarrier(const short &RENDERTARGET, int DEPHT_HANDLE)
{
	//�_�u���o�b�t�@�����O�p�̃o���A�����
	ChangeBarrier(backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT, 1);
	gDepth.Clear(DEPHT_HANDLE);
	//�w��̃����_�[�^�Q�b�g�Ńo���A��������
	ChangeBarrier(buffers->GetBufferData(0).Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET, CountPass(RENDERTARGET));


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

void RenderTargetStatus::ClearRenderTarget(const short& RENDERTARGET_HANDLE)
{
	short clearHandle = buffers->handle->CaluNowHandle(RENDERTARGET_HANDLE);

	//�����_�[�^�Q�b�g�̃N���A
	gDepth.Clear(handle);
	//�����_�[�^�Q�b�g�̐ݒ�
	//DescriptorHeapMgr::Instance()->SetDescriptorHeap(RENDERTARGET_HANDLE);
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = multiPassRTVHeap->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += DirectX12Device::Instance()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * buffers->handle->CaluNowHandle(RENDERTARGET_HANDLE);

	float ClearColor[] = { clearColors[clearHandle].x,clearColors[clearHandle].y ,clearColors[clearHandle].z ,clearColors[clearHandle].w };
	DirectX12CmdList::Instance()->cmdList->ClearRenderTargetView(rtvH, ClearColor, 0, nullptr);
}

short RenderTargetStatus::CreateRenderTarget(const XMFLOAT2 &GRAPH_SIZE, const XMFLOAT3 &CLEAR_COLOR, const DXGI_FORMAT &FORMAT)
{
	//�r���[�̐���
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = FORMAT;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;



	D3D12_RESOURCE_DESC resource = RenderTargetStatus::Instance()->copyBuffer->GetDesc();
	resource.Height = GRAPH_SIZE.y;
	resource.Width = GRAPH_SIZE.x;
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


	//DescriptorHeapMgr::Instance()->CreateBufferView(num, rtvDesc, buffers->GetBufferData(num).Get());
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

	renderTargetData.push_back(vector<short>(handle));
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

	const int ARRAY_SIZE = MULTIRENDER_TARGET_DATA.size();
	std::vector<short> handles;

	for (int i = 0; i < MULTIRENDER_TARGET_DATA.size(); ++i)
	{
		D3D12_RESOURCE_DESC resource = RenderTargetStatus::Instance()->copyBuffer->GetDesc();
		resource.Height = MULTIRENDER_TARGET_DATA[i].graphSize.y;
		resource.Width = MULTIRENDER_TARGET_DATA[i].graphSize.x;
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
		short num = buffers->CreateBuffer(data);


		//DescriptorHeapMgr::Instance()->CreateBufferView(num, rtvDesc, buffers->GetBufferData(num).Get());
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


		handles.push_back(num);
	}

	renderTargetData.push_back(handles);
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

void RenderTargetStatus::ChangeBarrier(ID3D12Resource *RESOURCE, const D3D12_RESOURCE_STATES& BEFORE_STATE, const D3D12_RESOURCE_STATES& AFTER_STATE, unsigned int RENDER_TARGET_PASS_NUM)
{
	D3D12_RESOURCE_BARRIER barrier =
		CD3DX12_RESOURCE_BARRIER::Transition(
			RESOURCE,
			BEFORE_STATE,
			AFTER_STATE
		);
	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(RENDER_TARGET_PASS_NUM, &barrier);
}

unsigned int RenderTargetStatus::CountPass(short HANDLE)
{
	for (int handleNum = 0; handleNum < renderTargetData.size(); ++handleNum)
	{
		for (int renderTargetNum = 0; renderTargetNum < renderTargetData.size(); ++renderTargetNum)
		{
			if (renderTargetData[handleNum][renderTargetNum] == HANDLE)
			{
				return static_cast<unsigned int>(renderTargetData[handleNum].size());
			}
		}
	}
	//�p�X�����������ꍇ�A�T�[�g��f��
	assert(0);
}
