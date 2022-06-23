#include "GraphicsPipeLineMgr.h"
#include"../DirectXCommon/DirectX12Device.h"
#include"../DirectXCommon/DirectX12CmdList.h"
#include"../Helper/KazHelper.h"

GraphicsPipeLineMgr::GraphicsPipeLineMgr()
{
	InputLayOutRegisterData.resize(100);
	VertexShaderRegisterData.resize(100);
	PixcelShaderRegisterData.resize(100);
	PipeLineDataRegisterData.resize(100);
	GeometoryShaderRegisterData.resize(100);
	RootSignatureName.resize(100);
	PipeLineRegisterData.resize(100);
	computeRootSignatureName.resize(100);

	ComputeShaderRegisterData.resize(100);
	computePipeLineDataRegisterData.resize(100);
	ComputePipeLineRegisterData.resize(100);
	vshaderBin.resize(100);
	pshaderBin.resize(100);
	gshaderBin.resize(100);
	cshaderBin.resize(100);
	geo = false;
}

GraphicsPipeLineMgr::~GraphicsPipeLineMgr()
{
}

void GraphicsPipeLineMgr::RegisterInputLayOutWithData(InputLayOutData INPUT_LAY_OUT, InputLayOutNames NAME)
{
	if (IsitSafe(NAME, InputLayOutRegisterData.size()))
	{
		InputLayOutRegisterData[NAME] = INPUT_LAY_OUT;
	}
	else
	{
		FailCheck("�댯:InputLayOut���o�^�ł��܂���ł���");
	}
}

void GraphicsPipeLineMgr::RegisterVertexShaderWithData(string SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL, VertexShaderNames NAME)
{
	if (IsitSafe(NAME, VertexShaderRegisterData.size()))
	{
		Shader vsShader;
		vsShader.CompileShader2(SHADER_FILE, ENTRY_POINT, SHADER_MODEL, SHADER_TYPE_VERTEX);
		VertexShaderRegisterData[NAME] = vsShader.GetShaderData(SHADER_TYPE_VERTEX);
		vshaderBin[NAME] = vsShader.shaderBin;
	}
	else
	{
		FailCheck("�댯:VertexShaderNames���o�^�ł��܂���ł���");
	}
}

void GraphicsPipeLineMgr::RegisterPixcelShaderWithData(string SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL, PixcelShaderNames NAME)
{
	if (IsitSafe(NAME, PixcelShaderRegisterData.size()))
	{
		Shader psShader;
		psShader.CompileShader2(SHADER_FILE, ENTRY_POINT, SHADER_MODEL, SHADER_TYPE_PIXEL);
		PixcelShaderRegisterData[NAME] = psShader.GetShaderData(SHADER_TYPE_PIXEL);
		pshaderBin[NAME] = psShader.shaderBin;
	}
	else
	{
		FailCheck("�댯:PixcelShaderNames���o�^�ł��܂���ł���");
	}
}

void GraphicsPipeLineMgr::RegisterGeometoryShaderWithData(string SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL, GeometoryShaderNames NAME)
{
	if (IsitSafe(NAME, GeometoryShaderRegisterData.size()))
	{
		Shader gsShader;
		gsShader.CompileShader2(SHADER_FILE, ENTRY_POINT, SHADER_MODEL, SHADER_TYPE_GEOMETORY);
		GeometoryShaderRegisterData[NAME] = gsShader.GetShaderData(SHADER_TYPE_GEOMETORY);
		gshaderBin[NAME] = gsShader.shaderBin;
	}
	else
	{
		FailCheck("�댯:GeometryShaderNames���o�^�ł��܂���ł���");
	}
}

void GraphicsPipeLineMgr::RegisterComputeShaderWithData(string SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL, ComputeShaderNames NAME)
{
	if (IsitSafe(NAME, ComputeShaderRegisterData.size()))
	{
		Shader csShader;
		csShader.CompileShader2(SHADER_FILE, ENTRY_POINT, SHADER_MODEL, SHADER_TYPE_COMPUTE);
		ComputeShaderRegisterData[NAME] = csShader.GetShaderData(SHADER_TYPE_COMPUTE);
		cshaderBin[NAME] = csShader.shaderBin;
	}
	else
	{
		FailCheck("�댯:ComputeShaderNames���o�^�ł��܂���ł���");
	}
}

void GraphicsPipeLineMgr::RegisterPipeLineDataWithData(D3D12_GRAPHICS_PIPELINE_STATE_DESC PIPELINE_DATA, PipeLineDataNames NAME)
{
	if (IsitSafe(NAME, PipeLineDataRegisterData.size()))
	{
		PipeLineDataRegisterData[NAME] = PIPELINE_DATA;
	}
	else
	{
		FailCheck("�댯:PipeLineDataRegisterData���o�^�ł��܂���ł���");
	}
}

void GraphicsPipeLineMgr::RegisterComputePipeLineDataWithData(D3D12_COMPUTE_PIPELINE_STATE_DESC PIPELINE_DATA, ComputePipeLineDataNames NAME)
{
	if (IsitSafe(NAME, computePipeLineDataRegisterData.size()))
	{
		computePipeLineDataRegisterData[NAME] = PIPELINE_DATA;
	}
	else
	{
		FailCheck("�댯:computePipeLineDataRegisterData���o�^�ł��܂���ł���");
	}
}

void GraphicsPipeLineMgr::CreatePipeLine(InputLayOutNames INPUT_LAYOUT_NAME, VertexShaderNames VERTEX_SHADER_NAME, PixcelShaderNames PIXCEL_SHADER_NAME, PipeLineDataNames PIPELINE_DATA_NAME, RootSignatureMode ROOTSIGNATURE, PipeLineNames PIPELINE_NAME, GeometoryShaderNames GEOMETORY_SHADER_NAME)
{
	//�p�C�v���C���f�[�^�̑��
	D3D12_GRAPHICS_PIPELINE_STATE_DESC grahicsPipeLine;
	grahicsPipeLine = PipeLineDataRegisterData[PIPELINE_DATA_NAME];


	//�C���v�b�g���C�A�E�g�̑��
	grahicsPipeLine.InputLayout.pInputElementDescs = InputLayOutRegisterData[INPUT_LAYOUT_NAME].inputLayOut;
	grahicsPipeLine.InputLayout.NumElements = InputLayOutRegisterData[INPUT_LAYOUT_NAME].size;

	//Shader�̑��
	grahicsPipeLine.VS = CD3DX12_SHADER_BYTECODE(vshaderBin[VERTEX_SHADER_NAME].data(), vshaderBin[VERTEX_SHADER_NAME].size());
	grahicsPipeLine.PS = CD3DX12_SHADER_BYTECODE(pshaderBin[PIXCEL_SHADER_NAME].data(), pshaderBin[PIXCEL_SHADER_NAME].size());




	if (GEOMETORY_SHADER_NAME != SHADER_GEOMETORY_NONE)
	{
		grahicsPipeLine.GS = CD3DX12_SHADER_BYTECODE(gshaderBin[GEOMETORY_SHADER_NAME].data(), gshaderBin[GEOMETORY_SHADER_NAME].size());
	}

	//���[�g�V�O�l�`���̐ݒ�
	RootSignatureName[PIPELINE_NAME] = ROOTSIGNATURE;
	grahicsPipeLine.pRootSignature = GraphicsRootSignature::Instance()->GetRootSignature(ROOTSIGNATURE).Get();


	//�p�C�v���C���̐���
	if (IsitSafe(PIPELINE_NAME, PipeLineRegisterData.size()))
	{
		DirectX12Device::Instance()->dev->CreateGraphicsPipelineState(&grahicsPipeLine, IID_PPV_ARGS(&PipeLineRegisterData[PIPELINE_NAME]));
	}
	else
	{
		FailCheck("�댯:Pipeline���o�^�ł��܂���ł���");
	}
}

void GraphicsPipeLineMgr::CreateComputePipeLine(ComputeShaderNames COMPUTE_SHADER_NAME, ComputePipeLineDataNames PIPELINE_DATA_NAME, RootSignatureMode ROOTSIGNATURE, ComputePipeLineNames PIPELINE_NAME)
{
	//�p�C�v���C���f�[�^�̑��
	D3D12_COMPUTE_PIPELINE_STATE_DESC grahicsPipeLine;
	grahicsPipeLine = computePipeLineDataRegisterData[PIPELINE_DATA_NAME];

	//Shader�̑��
	grahicsPipeLine.CS = CD3DX12_SHADER_BYTECODE(cshaderBin[COMPUTE_SHADER_NAME].data(), cshaderBin[COMPUTE_SHADER_NAME].size());

	//���[�g�V�O�l�`���̐ݒ�
	computeRootSignatureName[PIPELINE_NAME] = ROOTSIGNATURE;
	grahicsPipeLine.pRootSignature = GraphicsRootSignature::Instance()->GetRootSignature(ROOTSIGNATURE).Get();


	//�p�C�v���C���̐���
	if (IsitSafe(PIPELINE_NAME, ComputePipeLineRegisterData.size()))
	{
		DirectX12Device::Instance()->dev->CreateComputePipelineState(&grahicsPipeLine, IID_PPV_ARGS(&ComputePipeLineRegisterData[PIPELINE_NAME]));
		ComputePipeLineRegisterData[PIPELINE_NAME]->SetName(L"Compute");
	}
	else
	{
		FailCheck("�댯:ComputePipeline���o�^�ł��܂���ł���");
	}
}

bool GraphicsPipeLineMgr::SetPipeLineAndRootSignature(PipeLineNames PIPELINE_NAME)
{
	if (IsitSafe(PIPELINE_NAME, PipeLineRegisterData.size()) && PipeLineRegisterData[PIPELINE_NAME].Get() != nullptr)
	{
		GraphicsRootSignature::Instance()->SetRootSignature(RootSignatureName[PIPELINE_NAME]);
		DirectX12CmdList::Instance()->cmdList->SetPipelineState(PipeLineRegisterData[PIPELINE_NAME].Get());
		return true;
	}
	else
	{
		FailCheck("�댯:PipeLine�����݂��Ȃ��ׁA�R�}���h���X�g�ɐς߂܂���ł���");
		return false;
	}
}

bool GraphicsPipeLineMgr::SetComputePipeLineAndRootSignature(ComputePipeLineNames PIPELINE_NAME)
{
	if (IsitSafe(PIPELINE_NAME, ComputePipeLineRegisterData.size()) && ComputePipeLineRegisterData[PIPELINE_NAME].Get() != nullptr)
	{
		GraphicsRootSignature::Instance()->SetComputeRootSignature(computeRootSignatureName[PIPELINE_NAME]);
		DirectX12CmdList::Instance()->cmdList->SetPipelineState(ComputePipeLineRegisterData[PIPELINE_NAME].Get());
		return true;
	}
	else
	{
		FailCheck("�댯:ComputePipeLine�����݂��Ȃ��ׁA�R�}���h���X�g�ɐς߂܂���ł���");
		return false;
	}
}

RootSignatureMode GraphicsPipeLineMgr::GetRootSignatureName(PipeLineNames PIPELINE_NAME)
{
	return RootSignatureName[PIPELINE_NAME];
}

template <typename T>
inline bool GraphicsPipeLineMgr::IsitSafe(T NAME,unsigned short SIZE)
{
	if (0 <= NAME && NAME < SIZE)
	{
		return true;
	}
	else
	{
		return false;
	}
}
