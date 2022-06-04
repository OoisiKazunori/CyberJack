#pragma once
#include"../DirectXCommon/Base.h"
#include"../Pipeline/GraphicsDepthTest.h"
#include"../Helper/ISinglton.h"
#include"../Buffer/CreateGpuBuffer.h"

struct MultiRenderTargetData
{
	XMFLOAT3 backGroundColor;
	XMFLOAT2 graphSize;
	MultiRenderTargetData()
	{};
};

/// <summary>
/// レンダーターゲットの切り替え
/// ダブルバッファリングの処理がメインですが
/// RenderTargetクラスにて生成した変数を使用して、ClearRenderTargetViewしたりリソースの切り替えが可能です
/// </summary>
class RenderTargetStatus :public ISingleton<RenderTargetStatus>
{
public:
	RenderTargetStatus();
	~RenderTargetStatus();

	/// <summary>
	/// ダブルバッファリング用のバッファ作成
	/// </summary>
	/// <param name="SWAPCHAIN">スワップチェーン</param>
	void CreateDoubleBuffer(ComPtr<IDXGISwapChain4> SWAPCHAIN);

	/// <summary>
	/// ダブルバッファリング用のレンダーターゲットをクリアします
	/// </summary>
	/// <param name="COLOR">背景色</param>
	void SetDoubleBufferFlame(XMFLOAT3 COLOR);


	void ClearDoubuleBuffer(XMFLOAT3 COLOR);

	/// <summary>
	/// ダブルバッファリング用のバリア切り替え
	/// </summary>
	void SwapResourceBarrier();

	/// <summary>
	/// レンダータゲットのリソースバリアを開きます
	/// CLOSE_RENDERTARGETがnullptrの場合、ダブルバッファリング用のフレームを閉じます
	/// </summary>
	/// <param name="OPEN_RENDERTARGET">開きたいレンダーターゲットのリソースバリア</param>
	/// <param name="CLOSE_RENDERTARGET">閉じたいレンダーターゲットのリソースバリア</param>
	void PrepareToChangeBarrier(const short &OPEN_RENDERTARGET_HANDLE, const short &CLOSE_RENDERTARGET_HANDLE = -1);

	/// <summary>
	/// 指定のレンダーターゲットをリソースバリアを閉じます
	/// </summary>
	/// <param name="RENDERTARGET">閉じたいレンダーターゲット</param>
	void PrepareToCloseBarrier(const short &RENDERTARGET_HANDLE);

	/// <summary>
	/// 未完成
	/// 描画先を指定のレンダーターゲットに切り替え、深度バッファを切り替えます
	/// </summary>
	/// <param name="RENDERTARGET">指定のレンダータゲット</param>
	/// <param name="DEPHT_HANDLE">指定の深度バッファ</param>
	void PrepareToChangeBarrier(const short &RENDERTARGET, int DEPHT_HANDLE);


	/// <summary>
	/// レンダーターゲットをクリアします
	/// </summary>
	/// <param name="RENDERTARGET">クリアするレンダーターゲット</param>
	void ClearRenderTarget(const short &RENDERTARGET_HANDLE);


	short CreateRenderTarget(const XMFLOAT2 &GRAPH_SIZE, const XMFLOAT3 &CLEAR_COLOR, const DXGI_FORMAT &FORMAT);
	std::vector<short> CreateMultiRenderTarget(const std::vector<MultiRenderTargetData> &MULTIRENDER_TARGET_DATA, const DXGI_FORMAT &FORMAT);
	ID3D12Resource *GetBufferData(short HANDLE)const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetViewData(short HANDLE);


	void DeleteRenderTarget(short HANDLE);
	void DeleteMultiRenderTarget(const std::vector<short> &HANDLE);


	ComPtr<ID3D12DescriptorHeap> multiPassRTVHeap;
	ComPtr<ID3D12Resource> copyBuffer;

	GraphicsDepthTest gDepth;
	short handle, handle2;

	unique_ptr<CreateGpuBuffer> buffers;
	int bbIndex;

	//レンダーターゲットのハンドルからパス数を記録した配列
	std::array<std::vector<short>, 100>renderTargetData;
private:

	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	array<XMFLOAT4, 100> clearColors;


	ComPtr<IDXGISwapChain4> copySwapchain;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	D3D12_CPU_DESCRIPTOR_HANDLE multiPassRTVHanlde;


	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};

	void ChangeBarrier(ID3D12Resource *RESOURCE, const D3D12_RESOURCE_STATES &BEFORE_STATE, const D3D12_RESOURCE_STATES &AFTER_STATE);

	struct RenderTargetHandle
	{
		unsigned int renderTargetNum;
		std::vector<short> handles;
		RenderTargetHandle(unsigned int RENDER_TARGET_NUM, std::vector<short> FIRST_HANDLE) :renderTargetNum(RENDER_TARGET_NUM), handles(FIRST_HANDLE)
		{
		};
	};
	std::vector<short> CountPass(short HANDLE);
};
