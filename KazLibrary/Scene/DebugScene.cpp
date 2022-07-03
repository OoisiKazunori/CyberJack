#include"DebugScene.h"
#include"../Camera/CameraMgr.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Math/KazMath.h"
#include"../Helper/KazHelper.h"
#include"../Helper/KazBufferHelper.h"
#include"../KazLibrary/Pipeline/GraphicsPipeLineMgr.h"
#include"../KazLibrary/DirectXCommon/DirectX12CmdList.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"
#include"../KazLibrary/Loader/TextureResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

DebugScene::DebugScene()
{
	//short texHandle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "");

	buffer = std::make_unique<CreateGpuBuffer>();

	int num = 1000000;
	//���͗p�̃o�b�t�@�쐬
	inputHandle = buffer->CreateBuffer(KazBufferHelper::SetStructureBuffer(sizeof(InputData) * num));
	//�o�͗p�̃o�b�t�@�쐬
	outPutHandle = buffer->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(OutPutData) * num));
	//���ʗp�̃o�b�t�@
	commonHandle = buffer->CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(CommonData)));

	//�f�[�^����͂��Ă݂�
	//inputData = 1;

	//��̃o�b�t�@���f�X�N���v�^�q�[�v�ɓo�^
	size = DescriptorHeapMgr::Instance()->GetSize(DESCRIPTORHEAP_MEMORY_TEXTURE_COMPUTEBUFFER);

	//���ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC inputDesc = {};
	inputDesc.Format = DXGI_FORMAT_UNKNOWN;
	inputDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	inputDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	inputDesc.Buffer.FirstElement = 0;
	inputDesc.Buffer.NumElements = static_cast<UINT>(1);
	inputDesc.Buffer.StructureByteStride = sizeof(InputData) * num;
	inputDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;


	//���ݒ�
	D3D12_UNORDERED_ACCESS_VIEW_DESC outPutDesc = {};
	outPutDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	outPutDesc.Format = DXGI_FORMAT_UNKNOWN;
	outPutDesc.Buffer.NumElements = static_cast<UINT>(1);
	outPutDesc.Buffer.StructureByteStride = sizeof(OutPutData) * num;
	outPutDesc.Buffer.CounterOffsetInBytes = 0;



	D3D12_CONSTANT_BUFFER_VIEW_DESC commonDesc;
	commonDesc.BufferLocation = buffer->GetBufferData(commonHandle)->GetGPUVirtualAddress();
	commonDesc.SizeInBytes = (UINT)buffer->GetBufferData(commonHandle)->GetDesc().Width;


	DescriptorHeapMgr::Instance()->CreateBufferView(size.startSize, inputDesc, buffer->GetBufferData(inputHandle).Get());
	DescriptorHeapMgr::Instance()->CreateBufferView(size.startSize + 1, outPutDesc, buffer->GetBufferData(outPutHandle).Get());


	instanceBox = std::make_unique<BoxPolygonRender>(true, INSTANCE_NUM_MAX);
	instanceBox->data.pipelineName = PIPELINE_NAME_INSTANCE_COLOR;
	instanceBox->CreateConstBuffer(KazBufferHelper::GetBufferSize<UINT>(matData.size(), sizeof(DirectX::XMMATRIX)), typeid(DirectX::XMMATRIX).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);

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

	//�R���s���[�g�p�̃p�C�v���C���ݒ�
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_TEST);

	//���͗p�̃f�[�^�]��
	buffer->TransData(inputHandle, &inputData, sizeof(InputData));

	//���͗p�̃o�b�t�@�ݒ�
	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(size.startSize));
	//�o�͗p�̃o�b�t�@�ݒ�
	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(1, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(size.startSize + 1));

	//���ʃf�[�^�̃o�b�t�@�ݒ�Ɠ]��
	{
		CommonData data;
		data.cameraMat = CameraMgr::Instance()->GetViewMatrix();
		data.projectionMat = CameraMgr::Instance()->GetPerspectiveMatProjection();
		buffer->TransData(commonHandle, &data, sizeof(CommonData));
	}
	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(2, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(size.startSize + 2));
	//�f�B�X�p�b�`
	DirectX12CmdList::Instance()->cmdList->Dispatch(100, 10, 1);
}

void DebugScene::Draw()
{
	instanceBox->Draw();
}

void DebugScene::Input()
{
	KeyBoradInputManager *input = KeyBoradInputManager::Instance();

#pragma region Camera
	debugCameraMove = { 0,0,0 };
	float debugSpeed = 1;
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
		debugCameraMove.y = debugSpeed;
	}
	if (input->InputState(DIK_S))
	{
		debugCameraMove.y = -debugSpeed;
	}

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

int DebugScene::SceneChange()
{
	return -1;
}
