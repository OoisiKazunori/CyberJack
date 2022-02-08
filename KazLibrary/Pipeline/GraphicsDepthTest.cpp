#include "GraphicsDepthTest.h"

#include"../DirectXCommon/DirectX12Device.h"
#include"../DirectXCommon/DirectX12CmdList.h"

GraphicsDepthTest::GraphicsDepthTest()
{

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; //デプスステンシルビュー

	DirectX12Device::Instance()->dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	//dsvHeap->SetName(L"GraphicsDepthTest");
	//depthBuffer->SetName(L"GraphicsDepthTest");
}

GraphicsDepthTest::~GraphicsDepthTest()
{
}

UINT GraphicsDepthTest::CreateBuffer()
{	
	UINT handleNum = handle.GetHandle();
	////深度バッファの設定
	//D3D12_RESOURCE_DESC depthResDesc{};
	//depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//depthResDesc.Width = WIN_X;//レンダーターゲットに合わせる
	//depthResDesc.Height = WIN_Y;//レンダーターゲットに合わせる
	//depthResDesc.DepthOrArraySize = 1;//深度値フォーマット
	//depthResDesc.Format = DXGI_FORMAT_D32_FLOAT;
	//depthResDesc.SampleDesc.Count = 1;
	//depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//デプスステンシル

	////深度値用ヒーププロパティ
	//D3D12_HEAP_PROPERTIES depthHeapProp{};
	//depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	////深度クリア設定
	//D3D12_CLEAR_VALUE depthClearValue{};
	//depthClearValue.DepthStencil.Depth = 1.0f;		//深度値1.0fでクリア
	//depthClearValue.Format = DXGI_FORMAT_D32_FLOAT; //深度値フォーマット

	////リソース設定
	//DirectX12Device::dev->CreateCommittedResource(
	//	&depthHeapProp,
	//	D3D12_HEAP_FLAG_NONE,
	//	&depthResDesc,
	//	D3D12_RESOURCE_STATE_DEPTH_WRITE,
	//	&depthClearValue,
	//	IID_PPV_ARGS(&depthBuffer)
	//);

	////深度ビュー作成
	//D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	//dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	//dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	//DirectX12Device::dev->CreateDepthStencilView(
	//	depthBuffer.Get(),
	//	&dsvDesc,
	//	dsvHeap->GetCPUDescriptorHandleForHeapStart()
	//);

	//dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();

	depthBuffer.push_back({});
	dsvH.push_back({});

	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		WIN_X,
		WIN_Y,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);

	CD3DX12_HEAP_PROPERTIES propertices(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_CLEAR_VALUE clearValue(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	DirectX12Device::Instance()->dev->CreateCommittedResource(
		&propertices,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(&depthBuffer[handleNum])
	);

	depthBuffer[handleNum]->SetName(L"GraphicsDepthTest");


	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DirectX12Device::Instance()->dev->CreateDepthStencilView(
		depthBuffer[handleNum].Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);

	dsvH[handleNum] = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	dsvHeap->SetName(L"GraphicsDepthTest");

	return handleNum;
}

void GraphicsDepthTest::Clear(short HANDLE)
{
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	DirectX12CmdList::Instance()->cmdList->ClearDepthStencilView(dsvH[HANDLE], D3D12_CLEAR_FLAG_DEPTH, 1.0, 0, 0, nullptr);
}
