#include "DebugScene.h"
#include"../Camera/CameraMgr.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Math/KazMath.h"
#include"../Helper/KazHelper.h"
#include"../KazLibrary/Pipeline/GraphicsPipeLineMgr.h"
#include"../KazLibrary/DirectXCommon/DirectX12CmdList.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"
#include"../KazLibrary/Loader/TextureResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

DebugScene::DebugScene()
{
	short texHandle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "");

	buffer = std::make_unique<CreateGpuBuffer>();

	int num = 1000000;
	//入力用のバッファ作成
	inputHandle = buffer->CreateBuffer(KazBufferHelper::SetStructureBuffer(sizeof(InputData) * num));
	//出力用のバッファ作成
	outPutHandle = buffer->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(OutPutData) * num));

	//データを入力してみる
	//inputData = 1;

	//二つのバッファをデスクリプタヒープに登録
	size = DescriptorHeapMgr::Instance()->GetSize(DESCRIPTORHEAP_MEMORY_TEXTURE_COMPUTEBUFFER);

	//仮設定
	D3D12_SHADER_RESOURCE_VIEW_DESC inputDesc = {};
	inputDesc.Format = DXGI_FORMAT_UNKNOWN;
	inputDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	inputDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	inputDesc.Buffer.FirstElement = 0;
	inputDesc.Buffer.NumElements = static_cast<UINT>(1);
	inputDesc.Buffer.StructureByteStride = sizeof(InputData) * num;
	inputDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;


	//仮設定
	D3D12_UNORDERED_ACCESS_VIEW_DESC outPutDesc = {};
	outPutDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	outPutDesc.Format = DXGI_FORMAT_UNKNOWN;
	outPutDesc.Buffer.NumElements = static_cast<UINT>(1);
	outPutDesc.Buffer.StructureByteStride = sizeof(OutPutData) * num;
	outPutDesc.Buffer.CounterOffsetInBytes = 0;

	DescriptorHeapMgr::Instance()->CreateBufferView(size.startSize, inputDesc, buffer->GetBufferData(inputHandle).Get());
	DescriptorHeapMgr::Instance()->CreateBufferView(size.startSize + 1, outPutDesc, buffer->GetBufferData(outPutHandle).Get());



	for (int i = 0; i < instanceBox.size(); ++i)
	{
		instanceBox[i] = std::make_unique<BoxPolygonRender>(true, MAX);
		instanceBox[i]->data.pipelineName = PIPELINE_NAME_INSTANCE_COLOR;
		instanceBox[i]->CreateConstBuffer(matData.size() * sizeof(XMMATRIX), typeid(XMMATRIX).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
	}
	for (int i = 0; i < matData.size(); ++i)
	{
		matData[i];
		//instanceBox[i]->TransData(, , );
	}

}

DebugScene::~DebugScene()
{
}

void DebugScene::Init()
{

}

void DebugScene::Finalize()
{
}

void DebugScene::Update()
{
	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f });

	//コンピュート用のパイプライン設定
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_TEST);

	//入力用のデータ転送
	buffer->TransData(inputHandle, &inputData, sizeof(InputData));

	//入力用のバッファ設定
	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(size.startSize));
	//出力用のバッファ設定
	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(1, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(size.startSize + 1));
	DirectX12CmdList::Instance()->cmdList->Dispatch(1, 1, 1);
	//出力結果の受け取り
	OutPutData *data = (OutPutData *)buffer->GetMapAddres(outPutHandle);
}

void DebugScene::Draw()
{
	//bg.Draw();
	for (int i = 0; i < instanceBox.size(); ++i)
	{
		instanceBox[i]->Draw();
	}
}

void DebugScene::Input()
{
	KeyBoradInputManager *input = KeyBoradInputManager::Instance();

#pragma region カメラ操作
	debugCameraMove = { 0,0,0 };
	float debugSpeed = 1;
	//�J�����ړ�
	if (input->InputState(DIK_D))
	{
		debugCameraMove.x = -debugSpeed;
	}
	if (input->InputState(DIK_A))
	{
		debugCameraMove.x = debugSpeed;
	}
	if (input->InputState(DIK_W))
	{
		//y�Ȃ̂ɑO�ɐi��....
		debugCameraMove.y = debugSpeed;
	}
	if (input->InputState(DIK_S))
	{
		debugCameraMove.y = -debugSpeed;
	}


	//�J�����p�x
	if (input->InputState(DIK_RIGHTARROW))
	{
		angle.x += debugSpeed;
	}
	if (input->InputState(DIK_LEFTARROW))
	{
		angle.x += -debugSpeed;
	}

	if (input->InputState(DIK_UPARROW))
	{
		angle.y += debugSpeed;
	}
	if (input->InputState(DIK_DOWNARROW))
	{
		angle.y += -debugSpeed;
	}
	eyePos = KazMath::CaluEyePosForDebug(eyePos, debugCameraMove, angle);
	targetPos = KazMath::CaluTargetPosForDebug(eyePos, angle.x);

#pragma endregion

}

//�R���g���[���[�Ή�
//void DebugScene::Input(ControllerInputManager *INPUT)
//{
//#pragma region �f�o�b�N
//	//�f�o�b�N���[�h
//	if (INPUT->InputTrigger(XINPUT_GAMEPAD_B))
//	{
//		debugFlag = !debugFlag;
//	}
//
//	if (debugFlag)
//	{
//		debugCameraMove = { 0,0,0 };
//		float debugSpeed = 1;
//		//�J�����ړ�
//		if (INPUT->InputStickState(LEFT_STICK, RIGHT_SIDE))
//		{
//			debugCameraMove.x = -debugSpeed;
//		}
//		if (INPUT->InputStickState(LEFT_STICK, LEFT_SIDE))
//		{
//			debugCameraMove.x = debugSpeed;
//		}
//		if (INPUT->InputStickState(LEFT_STICK, UP_SIDE))
//		{
//			debugCameraMove.y = debugSpeed;
//		}
//		if (INPUT->InputStickState(LEFT_STICK, DOWN_SIDE))
//		{
//			debugCameraMove.y = -debugSpeed;
//		}
//
//
//
//		//�J�����p�x
//		if (INPUT->InputStickState(RIGHT_STICK, RIGHT_SIDE))
//		{
//			angle.x += debugSpeed;
//		}
//		if (INPUT->InputStickState(RIGHT_STICK, LEFT_SIDE))
//		{
//			angle.x += -debugSpeed;
//		}
//
//		if (INPUT->InputStickState(RIGHT_STICK, UP_SIDE))
//		{
//			angle.y += debugSpeed;
//		}
//		if (INPUT->InputStickState(RIGHT_STICK, DOWN_SIDE))
//		{
//			angle.y += -debugSpeed;
//		}
//
//
//	}
//#pragma endregion
//
//	eyePos = KazMath::CaluEyePosForDebug(eyePos, debugCameraMove, angle);
//	targetPos = KazMath::CaluTargetPosForDebug(eyePos, angle.x);
//
//}

int DebugScene::SceneChange()
{
	return -1;
}
