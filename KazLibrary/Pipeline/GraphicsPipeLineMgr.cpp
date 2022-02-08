#include "GraphicsPipeLineMgr.h"
#include"../DirectXCommon/DirectX12Device.h"
#include"../DirectXCommon/DirectX12CmdList.h"
#include"../Helper/KazHelper.h"

GraphicsPipeLineMgr::GraphicsPipeLineMgr()
{
	InputLayOutRegisterData.resize(50);
	VertexShaderRegisterData.resize(50);
	PixcelShaderRegisterData.resize(50);
	PipeLineDataRegisterData.resize(50);
	GeometoryShaderRegisterData.resize(50);
	RootSignatureName.resize(50);
	PipeLineRegisterData.resize(50);

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
		FailCheck("危険:InputLayOutが登録できませんでした");
	}
}

void GraphicsPipeLineMgr::RegisterVertexShaderWithData(string SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL, VertexShaderNames NAME)
{
	if (IsitSafe(NAME, VertexShaderRegisterData.size()))
	{
		Shader vsShader;
		vsShader.CompileShader(SHADER_FILE, ENTRY_POINT, SHADER_MODEL, SHADER_TYPE_VERTEX);
		VertexShaderRegisterData[NAME] = vsShader.GetShaderData(SHADER_TYPE_VERTEX);
	}
	else
	{
		FailCheck("危険:VertexShaderNamesが登録できませんでした");
	}
}

void GraphicsPipeLineMgr::RegisterPixcelShaderWithData(string SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL, PixcelShaderNames NAME)
{
	if (IsitSafe(NAME, PixcelShaderRegisterData.size()))
	{
		Shader psShader;
		psShader.CompileShader(SHADER_FILE, ENTRY_POINT, SHADER_MODEL, SHADER_TYPE_PIXEL);
		PixcelShaderRegisterData[NAME] = psShader.GetShaderData(SHADER_TYPE_PIXEL);
	}
	else
	{
		FailCheck("危険:PixcelShaderNamesが登録できませんでした");
	}
}

void GraphicsPipeLineMgr::RegisterGeometoryShaderWithData(string SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL, GeometoryShaderNames NAME)
{
	if (IsitSafe(NAME, GeometoryShaderRegisterData.size()))
	{
		Shader gsShader;
		gsShader.CompileShader(SHADER_FILE, ENTRY_POINT, SHADER_MODEL, SHADER_TYPE_GEOMETORY);
		GeometoryShaderRegisterData[NAME] = gsShader.GetShaderData(SHADER_TYPE_GEOMETORY);
	}
	else
	{
		FailCheck("危険:PixcelShaderNamesが登録できませんでした");
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
		FailCheck("危険:PipeLineDataRegisterDataが登録できませんでした");
	}
}

void GraphicsPipeLineMgr::CreatePipeLine(InputLayOutNames INPUT_LAYOUT_NAME, VertexShaderNames VERTEX_SHADER_NAME, PixcelShaderNames PIXCEL_SHADER_NAME, PipeLineDataNames PIPELINE_DATA_NAME, RootSignatureMode ROOTSIGNATURE, PipeLineNames PIPELINE_NAME, GeometoryShaderNames GEOMETORY_SHADER_NAME)
{
	//パイプラインデータの代入
	D3D12_GRAPHICS_PIPELINE_STATE_DESC grahicsPipeLine;
	grahicsPipeLine = PipeLineDataRegisterData[PIPELINE_DATA_NAME];


	//インプットレイアウトの代入
	grahicsPipeLine.InputLayout.pInputElementDescs = InputLayOutRegisterData[INPUT_LAYOUT_NAME].inputLayOut;
	grahicsPipeLine.InputLayout.NumElements = InputLayOutRegisterData[INPUT_LAYOUT_NAME].size;


	//Shaderの代入
	grahicsPipeLine.VS = CD3DX12_SHADER_BYTECODE(VertexShaderRegisterData[VERTEX_SHADER_NAME].Get());
	grahicsPipeLine.PS = CD3DX12_SHADER_BYTECODE(PixcelShaderRegisterData[PIXCEL_SHADER_NAME].Get());
	if (GEOMETORY_SHADER_NAME != SHADER_GEOMETORY_NONE)
	{
		grahicsPipeLine.GS = CD3DX12_SHADER_BYTECODE(GeometoryShaderRegisterData[GEOMETORY_SHADER_NAME].Get());
	}

	//ルートシグネチャの設定
	RootSignatureName[PIPELINE_NAME] = ROOTSIGNATURE;
	grahicsPipeLine.pRootSignature = GraphicsRootSignature::Instance()->GetRootSignature(ROOTSIGNATURE).Get();


	//パイプラインの生成
	if (IsitSafe(PIPELINE_NAME, PipeLineRegisterData.size()))
	{
		DirectX12Device::Instance()->dev->CreateGraphicsPipelineState(&grahicsPipeLine, IID_PPV_ARGS(&PipeLineRegisterData[PIPELINE_NAME]));
	}
	else
	{
		FailCheck("危険:Pipelineが登録できませんでした");
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
		FailCheck("危険:PipeLineが存在しない為、コマンドリストに積めませんでした");
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
