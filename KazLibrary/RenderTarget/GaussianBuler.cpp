#include"../RenderTarget/GaussianBuler.h"
#include"../Math/KazMath.h"

GaussianBuler::GaussianBuler(const KazMath::Vec2<UINT> &GRAPH_SIZE, XMFLOAT3 COLOR)
{
	buffers.reset(new CreateGpuBuffer);

	//ガウシアンブラーの計算
	int table = 8;
	vector<float> gaus(table);
	KazMath::CalcWeightsTableFromGaussian(gaus.data(), table, 100.0f);


	cbvHandle = buffers->CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(gaus[0] * gaus.size())));

	//cbvHandle = renderData.constBufferMgrInstacnce->CreateBuffer(KazMath::AligmentedValue(sizeof(gaus[0] * gaus.size()), 256));
	
	//float *weight = nullptr;
	//copy(gaus.begin(), gaus.end(), weight);
	//buffers->TransData(cbvHandle, &weight, 256);

	{
		float *mappedWeight = nullptr;
		buffers->GetBufferData(cbvHandle)->Map(0, nullptr, (void **)&mappedWeight);
		copy(gaus.begin(), gaus.end(), mappedWeight);
		buffers->GetBufferData(cbvHandle)->Unmap(0, nullptr);
	}

	graphSize = GRAPH_SIZE;
	rect = CD3DX12_RECT(0, 0, static_cast<LONG>(graphSize.x), static_cast<LONG>(graphSize.y));
	viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(graphSize.x), static_cast<float>(graphSize.y));


	renderTargetHandle =
		RenderTargetStatus::Instance()->CreateRenderTarget(graphSize, BG_COLOR, DXGI_FORMAT_R8G8B8A8_UNORM);

	renderTargetHandle2 =
		RenderTargetStatus::Instance()->CreateRenderTarget(graphSize, BG_COLOR, DXGI_FORMAT_R8G8B8A8_UNORM);


	tex[0].data.pipelineName = PIPELINE_NAME_GAUSSIAN_UPBLUR;
	tex[1].data.pipelineName = PIPELINE_NAME_GAUSSIAN_SIDEBLUR;

	tex[0].data.transform.pos = { WIN_X / 2,WIN_Y / 2 };
	tex[1].data.transform.pos = { WIN_X / 2,WIN_Y / 2 };
}

GaussianBuler::~GaussianBuler()
{
	RenderTargetStatus::Instance()->DeleteRenderTarget(renderTargetHandle);
	RenderTargetStatus::Instance()->DeleteRenderTarget(renderTargetHandle2);
}

short GaussianBuler::BlurImage(const short& TEXTURE_HANDLE, const short& CURRENT_RENDERTARGET_HANDLE)
{
	renderData.pipelineMgr->SetPipeLineAndRootSignature(PIPELINE_NAME_GAUSSIAN_UPBLUR);
	
	//renderData.constBufferMgrInstacnce->SetCBV(cbvHandle, GraphicsRootSignature::Instance()->GetRootParam(renderData.pipelineMgr->GetRootSignatureName(PIPELINE_NAME_GAUSSIAN_UPBLUR)), GRAPHICS_PRAMTYPE_DATA);

	UINT num = KazRenderHelper::SetBufferOnCmdList(GraphicsRootSignature::Instance()->GetRootParam(renderData.pipelineMgr->GetRootSignatureName(PIPELINE_NAME_GAUSSIAN_UPBLUR)), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
	renderData.cmdListInstance->cmdList->SetGraphicsRootConstantBufferView(num, buffers->GetBufferData(cbvHandle).Get()->GetGPUVirtualAddress());



	//ガウシアンぼかしの縦ブラー
	RenderTargetStatus::Instance()->PrepareToChangeBarrier(renderTargetHandle, CURRENT_RENDERTARGET_HANDLE);
	RenderTargetStatus::Instance()->ClearRenderTarget(renderTargetHandle);

	//描画
	renderData.cmdListInstance->cmdList->RSSetViewports(1, &viewport);
	renderData.cmdListInstance->cmdList->RSSetScissorRects(1, &rect);
	tex[0].data.handle = TEXTURE_HANDLE;
	tex[0].Draw();

	//ガウシアンぼかしの横ブラー
	RenderTargetStatus::Instance()->PrepareToChangeBarrier(renderTargetHandle2, renderTargetHandle);
	RenderTargetStatus::Instance()->ClearRenderTarget(renderTargetHandle2);

	//描画
	renderData.cmdListInstance->cmdList->RSSetViewports(1, &viewport);
	renderData.cmdListInstance->cmdList->RSSetScissorRects(1, &rect);
	tex[1].data.handle = renderTargetHandle;
	tex[1].Draw();

	//true...RenderTarget外での実行、false...RenderTarget内での実行
	if (CURRENT_RENDERTARGET_HANDLE == -1)
	{
		RenderTargetStatus::Instance()->PrepareToCloseBarrier(renderTargetHandle2);
		RenderTargetStatus::Instance()->SetDoubleBufferFlame(BG_COLOR);
	}
	else
	{
		RenderTargetStatus::Instance()->PrepareToChangeBarrier(CURRENT_RENDERTARGET_HANDLE, renderTargetHandle2);
		RenderTargetStatus::Instance()->ClearRenderTarget(CURRENT_RENDERTARGET_HANDLE);
	}

	CD3DX12_RECT Rect(0, 0, static_cast<long>(WIN_X), static_cast<long>(WIN_Y));
	CD3DX12_VIEWPORT Viewport(0.0f, 0.0f, static_cast<float>(WIN_X), static_cast<float>(WIN_Y));
	renderData.cmdListInstance->cmdList->RSSetViewports(1, &Viewport);
	renderData.cmdListInstance->cmdList->RSSetScissorRects(1, &Rect);


	return renderTargetHandle2;
}
