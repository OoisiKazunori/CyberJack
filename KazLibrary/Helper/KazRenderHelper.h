#pragma once
#include"../DirectXCommon/Base.h"
#include"../Pipeline/GraphicsPipeLineMgr.h"
#include"../Loader/TextureResourceMgr.h"
#include"../Loader/TextureResourceMgr.h"
#include"../Camera/CameraMgr.h"
#include"../DirectXCommon/DirectX12Device.h"
#include"../DirectXCommon/DirectX12CmdList.h"
#include"../Math/KazMath.h"

namespace KazRenderHelper
{
	/// <summary>
	/// 描画に必要なクラスのポインタ
	/// </summary>
	struct RenderInstancesData
	{
		TextureResourceMgr *shaderResourceMgrInstance;
		CameraMgr *cameraMgrInstance;
		DirectX12Device *deviceInstance;
		DirectX12CmdList *cmdListInstance;
		GraphicsPipeLineMgr *pipelineMgr;

		RenderInstancesData() :
			shaderResourceMgrInstance(TextureResourceMgr::Instance()),
			cameraMgrInstance(CameraMgr::Instance()),
			deviceInstance(DirectX12Device::Instance()),
			cmdListInstance(DirectX12CmdList::Instance()),
			pipelineMgr(GraphicsPipeLineMgr::Instance())
		{
		}
	};

	array<XMFLOAT2, 4> ChangePlaneScale(const XMFLOAT2 &LEFTUP_POS, const XMFLOAT2 &RIGHTDOWN_POS, const XMFLOAT2 &SCALE, const XMFLOAT2 &ANCHOR_POINT, const XMFLOAT2 &TEX_SIZE);
	array<XMFLOAT2, 4> ChangeModiPlaneScale(const XMFLOAT2 &LEFTUP_POS, const XMFLOAT2 &RIGHTDOWN_POS, const XMFLOAT4 &SCALE, const XMFLOAT2 &ANCHOR_POINT, const XMFLOAT2 &TEX_SIZE);

	void FlipXUv(XMFLOAT2 *UV_LEFTUP_POS, XMFLOAT2 *UV_LEFTDOWN_POS, XMFLOAT2 *UV_RIGHTUP_POS, XMFLOAT2 *UV_RIGHTDOWN_POS);
	void FlipYUv(XMFLOAT2 *UV_LEFTUP_POS, XMFLOAT2 *UV_LEFTDOWN_POS, XMFLOAT2 *UV_RIGHTUP_POS, XMFLOAT2 *UV_RIGHTDOWN_POS);


	void VerticesCut(XMFLOAT2 DIV_SIZE, XMFLOAT2 DIV_LEFTUP_POS, XMFLOAT3 *LEFTUP_POS, XMFLOAT3 *LEFTDOWN_POS, XMFLOAT3 *RIGHTUP_POS, XMFLOAT3 *RIGHTDOWN_POS, const XMFLOAT2 &SCALE, const XMFLOAT2 &ANCHOR_POINT);
	void UVCut(XMFLOAT2 UV_DIV_LEFTUP_POS, XMFLOAT2 DIV_SIZE, XMFLOAT2 TEX_SIZE, XMFLOAT2 *LEFTUP_POS, XMFLOAT2 *LEFTDOWN_POS, XMFLOAT2 *RIGHTUP_POS, XMFLOAT2 *RIGHTDOWN_POS);


	void InitUvPos(XMFLOAT2 *UV_LEFTUP_POS, XMFLOAT2 *UV_LEFTDOWN_POS, XMFLOAT2 *UV_RIGHTUP_POS, XMFLOAT2 *UV_RIGHTDOWN_POS);
	void InitVerticesPos(XMFLOAT3 *LEFTUP_POS, XMFLOAT3 *LEFTDOWN_POS, XMFLOAT3 *RIGHTUP_POS, XMFLOAT3 *RIGHTDOWN_POS, const XMFLOAT2 &ANCHOR_POINT);

	array<unsigned short, 6> InitIndciesForPlanePolygon();


	int SetBufferOnCmdList(const GraphicsRootSignatureParameter &PARAM, const GraphicsRangeType &RANGE, const GraphicsRootParamType &TYPE);


	XMFLOAT4 SendColorDataToGPU(XMFLOAT4 COLOR_DATA);

};