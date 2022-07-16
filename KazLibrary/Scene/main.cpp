#include"../DirectXCommon/Base.h"
#include"../WinApi/WinApi.h"
#include"../WinApi/Msg.h"
#include"../DirectXCommon/DirectX12.h"
#include"../Scene/SceneManager.h"
#include"../Pipeline/PreCreateBasePipeLine.h"
#include"../Pipeline/PreCreateBaseRootSignature.h"
#include"../DirectXCommon/DirectX12Device.h"
#include"../RenderTarget/RenderTargetStatus.h"
#include"../Imgui/MyImgui.h"
#include"../Helper/OutPutDebugStringAndCheckResult.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Input/ControllerInputManager.h"
#include<time.h>

/*
以下の言葉を、ソリューションの全体で検索すると特定の場所まで飛びます
「メモ帳」...そのファイルに関係する内容で分かった事、疑問点まで飛びます
「問題」.....解決しなければならない問題点まで飛びます
*/

//ターゲットがデバックの時のみコンソール用のmain文を参照する
#ifdef _DEBUG
int main()
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif // DEBUG
{
#ifdef _DEBUG
	//DebugLayer
	Microsoft::WRL::ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		//debugController->SetEnableGPUBasedValidation(TRUE);
	}
	debugController.ReleaseAndGetAddressOf();
#endif
	//CheckDirectXError
	int CheckWinError = 0;
	bool CheckMessageFlag = true;
	HRESULT CheckDirectXError = 0;
	

	WinApi winApi;
	Msg msg;
	DirectX12 directX;
	CheckWinError = winApi.CreateMyWindow(WIN_X, WIN_Y);
	if (CheckResult("ウィンドウの生成", CheckWinError)) 
	{
		return 0;
	}
	CheckDirectXError = directX.Create(WIN_X, WIN_Y, winApi.hwnd);
	//CheckDirectXError = S_FALSE;
	if (CheckResult("DirextX12の生成", CheckDirectXError)) 
	{
		return 0;
	}

	

	MyImgui imgui;
	imgui.Create(winApi.hwnd);

	RenderTargetStatus::Instance()->CreateDoubleBuffer(directX.swapchain);

	//もう一度繰り返さないようシングルトンにする
	PreCreateBaseRootSignature prepareR;
	PreCreateBasePipeLine prepareP;

	SceneManager sm;	
	KeyBoradInputManager::Instance()->CreateDevice(&winApi.hwnd, &winApi.window.hInstance);


	OutputDebugStringA("ゲームのメインループを開始します\n");
	srand(static_cast<UINT>(time(NULL)));

	while (CheckMessageFlag)
	{
		CheckMessageFlag = msg.CheckMessage();
		imgui.NewFlame();
		KeyBoradInputManager::Instance()->InputLog();
		ControllerInputManager::Instance()->InputLog();
#ifdef _DEBUG
		winApi.FPS();
#endif

		sm.Update();
		sm.Draw();
		imgui.Set();
		CameraMgr::Instance()->Record();
		RenderTargetStatus::Instance()->SwapResourceBarrier();
		directX.ActCommand();
	}
	winApi.UnregisterWindowClass();



#ifdef _DEBUG
	ID3D12DebugDevice *debugInterface;
	HRESULT result = DirectX12Device::Instance()->dev.Get()->QueryInterface(&debugInterface);
	if (SUCCEEDED(result))
	{
		//debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
		debugInterface->Release();
	}
#endif

	return 0;
}

