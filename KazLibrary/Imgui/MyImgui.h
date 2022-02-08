#pragma once
#include"../DirectXCommon/Base.h"
#include"imgui.h"
#include"imgui_impl_win32.h"
#include"imgui_impl_dx12.h"

/// <summary>
/// ImGui�ׂ̈̃f�X�N���v�^�q�[�v�̐�����ImGUi�̏��������s���܂�
/// �Ȃ��AImGui�̊֐����Ăяo�����߂�h���܂Ƃ߂ď����Ă��܂�
/// </summary>
class MyImgui {
public:
	MyImgui();
	~MyImgui();

	/// <summary>
	/// ImGui�̏������ƃf�X�N���v�^�q�[�v�̏�����
	/// </summary>
	/// <param name="hwnd">�E�B���h�E�n���h��</param>
	void Create(HWND hwnd);

	/// <summary>
	/// ImGui�̕`��J�n
	/// </summary>
	void NewFlame();

	/// <summary>
	/// �R�}���h���X�g�ɐς�
	/// </summary>
	void Set();

private:
	/// <summary>
	/// �q�[�v�̐���
	/// </summary>
	/// <returns>�f�X�N���v�^�q�[�v</returns>
	ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeapForImgui();
	ComPtr<ID3D12DescriptorHeap>heapForImgui;

};

