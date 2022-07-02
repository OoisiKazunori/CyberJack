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
	struct DrawIndexedInstancedData
	{
		UINT indexCountPerInstance;
		UINT instanceCount;
		UINT startIndexLocation;
		UINT baseVertexLocation;
		UINT startInstanceLocation;
	};

	struct DrawInstancedData
	{
		UINT vertexCountPerInstance;
		UINT instanceCount;
		UINT startVertexLocation;
		UINT startInstanceLocation;
	};

	struct IASetVertexBuffersData
	{
		UINT slot;
		UINT numViews;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	};

	struct DrawIndexInstanceCommandData
	{
		D3D_PRIMITIVE_TOPOLOGY topology;
		KazRenderHelper::IASetVertexBuffersData vertexBufferDrawData;
		D3D12_INDEX_BUFFER_VIEW indexBufferView;
		KazRenderHelper::DrawIndexedInstancedData drawIndexInstancedData;
	};

	struct DrawInstanceCommandData
	{
		D3D_PRIMITIVE_TOPOLOGY topology;
		KazRenderHelper::IASetVertexBuffersData vertexBufferDrawData;
		KazRenderHelper::DrawInstancedData drawInstanceData;
	};


	DrawIndexInstanceCommandData SetDrawIndexInstanceCommandData(const D3D_PRIMITIVE_TOPOLOGY &TOPOLOGY, const D3D12_VERTEX_BUFFER_VIEW &VERTEX_VIEW, const D3D12_INDEX_BUFFER_VIEW &INDEX_VIEW, UINT INDECIES_NUM, UINT INSTANCE_NUM);
	DrawInstanceCommandData SetDrawInstanceCommandData(const D3D_PRIMITIVE_TOPOLOGY &TOPOLOGY, const D3D12_VERTEX_BUFFER_VIEW &VERTEX_VIEW, UINT VERTEX_NUM, UINT INSTANCE_NUM);

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

	array<KazMath::Vec2<float>, 4> ChangePlaneScale(const KazMath::Vec2<float> &LEFTUP_POS, const KazMath::Vec2<float> &RIGHTDOWN_POS, const KazMath::Vec2<float> &SCALE, const KazMath::Vec2<float> &ANCHOR_POINT, const KazMath::Vec2<int> &TEX_SIZE);
	array<KazMath::Vec2<float>, 4> ChangeModiPlaneScale(const KazMath::Vec2<float> &LEFTUP_POS, const KazMath::Vec2<float> &RIGHTDOWN_POS, const XMFLOAT4 &SCALE, const XMFLOAT2 &ANCHOR_POINT, const XMFLOAT2 &TEX_SIZE);

	void FlipXUv(XMFLOAT2 *UV_LEFTUP_POS, XMFLOAT2 *UV_LEFTDOWN_POS, XMFLOAT2 *UV_RIGHTUP_POS, XMFLOAT2 *UV_RIGHTDOWN_POS);
	void FlipYUv(XMFLOAT2 *UV_LEFTUP_POS, XMFLOAT2 *UV_LEFTDOWN_POS, XMFLOAT2 *UV_RIGHTUP_POS, XMFLOAT2 *UV_RIGHTDOWN_POS);


	void VerticesCut(const KazMath::Vec2<int> &DIV_SIZE, const KazMath::Vec2<int> &DIV_LEFTUP_POS, XMFLOAT3 *LEFTUP_POS, XMFLOAT3 *LEFTDOWN_POS, XMFLOAT3 *RIGHTUP_POS, XMFLOAT3 *RIGHTDOWN_POS, const KazMath::Vec2<float> &SCALE, const KazMath::Vec2<float> &ANCHOR_POINT);
	void UVCut(const KazMath::Vec2<int> &UV_DIV_LEFTUP_POS, const KazMath::Vec2<int> &DIV_SIZE, const KazMath::Vec2<int> &TEX_SIZE, XMFLOAT2 *LEFTUP_POS, XMFLOAT2 *LEFTDOWN_POS, XMFLOAT2 *RIGHTUP_POS, XMFLOAT2 *RIGHTDOWN_POS);


	void InitUvPos(XMFLOAT2 *UV_LEFTUP_POS, XMFLOAT2 *UV_LEFTDOWN_POS, XMFLOAT2 *UV_RIGHTUP_POS, XMFLOAT2 *UV_RIGHTDOWN_POS);
	void InitVerticesPos(XMFLOAT3 *LEFTUP_POS, XMFLOAT3 *LEFTDOWN_POS, XMFLOAT3 *RIGHTUP_POS, XMFLOAT3 *RIGHTDOWN_POS, const XMFLOAT2 &ANCHOR_POINT);

	array<unsigned short, 6> InitIndciesForPlanePolygon();


	int SetBufferOnCmdList(const GraphicsRootSignatureParameter &PARAM, const GraphicsRangeType &RANGE, const GraphicsRootParamType &TYPE);


	XMFLOAT4 SendColorDataToGPU(XMFLOAT4 COLOR_DATA);

};