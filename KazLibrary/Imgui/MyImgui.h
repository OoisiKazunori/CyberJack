#pragma once
#include"../DirectXCommon/Base.h"
#include"imgui.h"
#include"imgui_impl_win32.h"
#include"imgui_impl_dx12.h"

/// <summary>
/// ImGuiの為のデスクリプタヒープの生成とImGUiの初期化を行います
/// なお、ImGuiの関数を呼び出すためのhもまとめて書いています
/// </summary>
class MyImgui {
public:
	MyImgui();
	~MyImgui();

	/// <summary>
	/// ImGuiの初期化とデスクリプタヒープの初期化
	/// </summary>
	/// <param name="hwnd">ウィンドウハンドル</param>
	void Create(HWND hwnd);

	/// <summary>
	/// ImGuiの描画開始
	/// </summary>
	void NewFlame();

	/// <summary>
	/// コマンドリストに積む
	/// </summary>
	void Set();

private:
	/// <summary>
	/// ヒープの生成
	/// </summary>
	/// <returns>デスクリプタヒープ</returns>
	ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeapForImgui();
	ComPtr<ID3D12DescriptorHeap>heapForImgui;

};

