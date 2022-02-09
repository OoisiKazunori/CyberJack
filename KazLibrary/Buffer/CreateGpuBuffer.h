#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/HandleMaker.h"
#include"../Helper/KazBufferHelper.h"

/// <summary>
/// バッファとディスクリプタヒープを生成するクラスです
/// </summary>
class CreateGpuBuffer
{
public:	
	CreateGpuBuffer();
	~CreateGpuBuffer();

	/// <summary>
	/// デスクリプタヒープを生成します
	/// </summary>
	/// <param name="DESCRIPTOR_NUM_MAX">生成するバッファの最大数</param>
	void CreateDescriptorHeap(unsigned int DESCRIPTOR_NUM_MAX);

	/// <summary>
	/// バッファを生成する
	/// </summary>
	/// <param name="HEAP_PROPERTIES">プロパティ</param>
	/// <param name="RESOURCE"></param>
	/// <param name="RESOURCE_STATE"></param>
	/// <param name="CLEAR_VALUE"></param>
	/// <returns>ハンドル</returns>
	short CreateBuffer(D3D12_HEAP_PROPERTIES HEAP_PROPERTIES, D3D12_RESOURCE_DESC RESOURCE, D3D12_RESOURCE_STATES RESOURCE_STATE, D3D12_CLEAR_VALUE *CLEAR_VALUE = nullptr);


	short CreateBuffer(const KazBufferHelper::BufferResourceData &BUFFER_OPTION);

	/// <summary>
	/// データをバッファに転送します
	/// </summary>
	/// <param name="HANDLE">バッファのハンドル</param>
	/// <param name="DATA">送りたいデータのアドレス</param>
	/// <param name="DATA_SIZE">送りたいデータのサイズ</param>
	void TransData(const short &HANDLE, void *DATA, const unsigned int &DATA_SIZE);

	/// <summary>
	/// 定数バッファビューを生成します
	/// </summary>
	/// <param name="HANDLE">生成したいバッファ</param>
	/// <param name="VIEW">ビューの設定</param>
	void CreateBufferView(short HANDLE, D3D12_CONSTANT_BUFFER_VIEW_DESC VIEW);

	/// <summary>
	/// シェーダーリソースビューを生成します
	/// </summary>
	/// <param name="HANDLE">生成したいバッファ</param>
	/// <param name="VIEW">ビューの設定</param>
	void CreateBufferView(short HANDLE, D3D12_SHADER_RESOURCE_VIEW_DESC VIEW);

	/// <summary>
	/// バッファを開放します
	/// </summary>
	/// <param name="HANDLE">開放したいバッファのハンドル</param>
	void ReleaseBuffer(short HANDLE);

	D3D12_GPU_DESCRIPTOR_HANDLE GetViewPointer(short HANDLE);

	/// <summary>
	/// バッファを受け取ります
	/// </summary>
	/// <param name="HANDLE">受け取りたいバッファのハンドル</param>
	/// <returns>生成されたバッファ</returns>
	ComPtr<ID3D12Resource>GetBufferData(short HANDLE);


	/// <summary>
	/// バッファのGPUアドレスを受け取ります
	/// </summary>
	/// <param name="HANDLE">ハンドル</param>
	/// <returns>バッファのGPUアドレス</returns>
	D3D12_GPU_VIRTUAL_ADDRESS GetGpuAddress(short HANDLE);


	void *GetMapAddres(short HANDLE);


	unique_ptr<HandleMaker> handle;
private:	
	vector<ComPtr<ID3D12Resource>> buffers;
	ComPtr<ID3D12DescriptorHeap> heap;

	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	

	UINT increSize;
};

