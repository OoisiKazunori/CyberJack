#include"../Pipeline/GraphicsRootSignature.h"
#include"../DirectXCommon/DirectX12Device.h"
#include"../DirectXCommon/DirectX12CmdList.h"
#include"../KazLibrary/Helper/KazHelper.h"

#define STR(var) #var   //引数にした変数を変数名を示す文字列リテラルとして返すマクロ関数

GraphicsRootSignature::GraphicsRootSignature()
{
	/*CreateColorRootSignature();
	CreateTextureRootSignature();
	CreateSpriteRootSignature();
	CreateObjRootSignature();
	CreateMultipassRootSignature();
	CreateLight();

	paramMgr[ROOTSIGNATURE_MODE_COLOR] = colorParam;
	paramMgr[ROOTSIGNATURE_MODE_TEXTURE] = textureParam;
	paramMgr[ROOTSIGNATURE_MODE_SPRITE] = spriteParam;
	paramMgr[ROOTSIGNATURE_MODE_OBJ] = objParam;
	paramMgr[ROOTSIGNATURE_MODE_MULTIPASS] = MultiPassParam;
	paramMgr[ROOTSIGNATURE_MODE_LIGHT] = LightParam;*/

	paramD.resize(50);
	rootSignature.resize(50);
}

GraphicsRootSignature::~GraphicsRootSignature()
{
	for (int i = 0; i < rootSignature.size(); i++)
	{
		rootSignature[i].ReleaseAndGetAddressOf();
	}
}

void GraphicsRootSignature::CreateRootSignature(RootSignatureMode ROOTSIGNATURE, RootSignatureData ROOTSIGNATURE_DATA, short DATA_MAX)
{
	vector<CD3DX12_ROOT_PARAMETER> rootparam(DATA_MAX);
	vector<CD3DX12_DESCRIPTOR_RANGE> descRangeSRV;
	descRangeSRV.reserve(DATA_MAX);

#pragma region ルートパラムの設定

	int param1 = 0;
	int param2 = 0;
	paramD[ROOTSIGNATURE].paramMax = DATA_MAX;
	for (int i = 0; i < DATA_MAX; i++)
	{
		switch (ROOTSIGNATURE_DATA.range[i])
		{
		case GRAPHICS_RANGE_TYPE_CBV:
			//ルートパラムの設定
			rootparam[i].InitAsConstantBufferView(param1, 0, D3D12_SHADER_VISIBILITY_ALL);

			//受け渡し用
			paramD[ROOTSIGNATURE].range[i] = GRAPHICS_RANGE_TYPE_CBV;
			paramD[ROOTSIGNATURE].paramData[i].param = ROOTSIGNATURE_DATA.paramData[i].param;
			paramD[ROOTSIGNATURE].paramData[i].type = ROOTSIGNATURE_DATA.paramData[i].type;
			paramD[ROOTSIGNATURE].type[i] = GRAPHICS_ROOTSIGNATURE_TYPE_VIEW;
			param1++;
			break;

		case GRAPHICS_RANGE_TYPE_SRV:
			//ルートパラムの設定
			descRangeSRV.push_back({});
			descRangeSRV[descRangeSRV.size() - 1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, param2);
			rootparam[i].InitAsDescriptorTable(1, &descRangeSRV[descRangeSRV.size() - 1], D3D12_SHADER_VISIBILITY_ALL);

			//受け渡し用
			paramD[ROOTSIGNATURE].range[i] = GRAPHICS_RANGE_TYPE_SRV;
			paramD[ROOTSIGNATURE].paramData[i].param = ROOTSIGNATURE_DATA.paramData[i].param;
			paramD[ROOTSIGNATURE].paramData[i].type = ROOTSIGNATURE_DATA.paramData[i].type;
			paramD[ROOTSIGNATURE].type[i] = GRAPHICS_ROOTSIGNATURE_TYPE_DESCRIPTORTABLE;
			param2++;
			break;

		case GRAPHICS_RANGE_TYPE_UAV:
			//ルートパラムの設定
			descRangeSRV.push_back({});
			descRangeSRV[descRangeSRV.size() - 1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, param2);
			rootparam[i].InitAsDescriptorTable(1, &descRangeSRV[descRangeSRV.size() - 1], D3D12_SHADER_VISIBILITY_ALL);

			//受け渡し用
			paramD[ROOTSIGNATURE].range[i] = GRAPHICS_RANGE_TYPE_UAV;
			paramD[ROOTSIGNATURE].paramData[i].param = ROOTSIGNATURE_DATA.paramData[i].param;
			paramD[ROOTSIGNATURE].paramData[i].type = ROOTSIGNATURE_DATA.paramData[i].type;
			paramD[ROOTSIGNATURE].type[i] = GRAPHICS_ROOTSIGNATURE_TYPE_DESCRIPTORTABLE;
		case -1:
			break;
		}
	}
#pragma endregion


	CreateMyRootSignature(ROOTSIGNATURE_DATA.sample, rootparam.data(), rootparam.size(), ROOTSIGNATURE);

	if (ROOTSIGNATURE == ROOTSIGNATURE_DATA_SRV_UAV)
	{
		rootSignature[ROOTSIGNATURE]->SetName(L"ComputeRootSignature");
	}
	else
	{
		rootSignature[ROOTSIGNATURE]->SetName(L"RootSignature");
	}
}

void GraphicsRootSignature::SetRootSignature(RootSignatureMode ROOTSIGNATURE_MODE)
{
	DirectX12CmdList::Instance()->cmdList->SetGraphicsRootSignature(
		rootSignature[ROOTSIGNATURE_MODE].Get()
	);
}

void GraphicsRootSignature::SetComputeRootSignature(RootSignatureMode ROOTSIGNATURE_MODE)
{
	DirectX12CmdList::Instance()->cmdList->SetComputeRootSignature(
		rootSignature[ROOTSIGNATURE_MODE].Get()
	);
}

const ComPtr<ID3D12RootSignature> GraphicsRootSignature::GetRootSignature(RootSignatureMode ROOT_SIGNATURE)
{
	return rootSignature[ROOT_SIGNATURE];
}

void GraphicsRootSignature::CreateMyRootSignature(D3D12_STATIC_SAMPLER_DESC SAMPLER_DATA, D3D12_ROOT_PARAMETER *ROOT_PARAM_DATA, short DATA_MAX, RootSignatureMode ROOTSIGNATURE)
{
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(
		DATA_MAX,
		ROOT_PARAM_DATA,
		1,
		&SAMPLER_DATA,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//バージョン自動判定でのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);

	//ルートシグネチャの生成
	DirectX12Device::Instance()->dev->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature[ROOTSIGNATURE])
	);
}

const GraphicsRootSignatureParameter GraphicsRootSignature::GetRootParam(RootSignatureMode ROOTSIGNATURE_MODE)
{
	return paramD[ROOTSIGNATURE_MODE];
}


#pragma region data
/*
void GraphicsRootSignature::CreateColorRootSignature()
{
	rootSignature.push_back({});

	//サンプラーの生成
	CD3DX12_STATIC_SAMPLER_DESC desc(0);
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = desc;

	CD3DX12_ROOT_PARAMETER rootparam;
	rootparam.InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);


	//SetSRVやSetCBVに渡す際に必要
	colorParam.range[0] = GRAPHICS_RANGE_TYPE_CBV;
	colorParam.paramData[0].param = 0;
	colorParam.paramData[0].type = GRAPHICS_PRAMTYPE_DRAW;
	colorParam.type[0] = GRAPHICS_ROOTSIGNATURE_TYPE_VIEW;
	colorParam.paramMax = 1;


	//ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(
		1,
		&rootparam,
		1,
		&samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);

	//バージョン自動判定でのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);

	//ルートシグネチャの生成
	DirectX12Device::Instance()->dev->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature[rootSignature.size() - 1])
	);


	rootSignature[0]->SetName(L"ColorRootSig");

}

void GraphicsRootSignature::CreateTextureRootSignature()
{

	rootSignature.push_back({});

	//サンプラーの生成
	CD3DX12_STATIC_SAMPLER_DESC desc(0);
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = desc;

	CD3DX12_DESCRIPTOR_RANGE descRangeCBV, descRangeSRV;
	descRangeCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER rootparam[2];
	rootparam[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparam[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);


	//SetSRVやSetCBVに渡す際に必要
	textureParam.range[0] = GRAPHICS_RANGE_TYPE_CBV;
	textureParam.paramData[0].param = 0;
	textureParam.paramData[0].type = GRAPHICS_PRAMTYPE_DRAW;
	textureParam.type[0] = GRAPHICS_ROOTSIGNATURE_TYPE_VIEW;

	textureParam.range[1] = GRAPHICS_RANGE_TYPE_SRV;
	textureParam.paramData[1].param = 1;
	textureParam.paramData[1].type = GRAPHICS_PRAMTYPE_TEX;
	textureParam.type[1] = GRAPHICS_ROOTSIGNATURE_TYPE_DESCRIPTORTABLE;
	textureParam.paramMax = 2;


	//ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(
		sizeof(rootparam) / sizeof(rootparam[0]),
		rootparam,
		1,
		&samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//バージョン自動判定でのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);

	//ルートシグネチャの生成
	DirectX12Device::Instance()->dev->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature[rootSignature.size() - 1])
	);


	rootSignature[0]->SetName(L"TextureRootSig");



}

void GraphicsRootSignature::CreateSpriteRootSignature()
{

	rootSignature.push_back({});

	//サンプラーの生成
	CD3DX12_STATIC_SAMPLER_DESC desc(0);
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = desc;

	//ディスクリプタレンジは後で理解しよう
	CD3DX12_DESCRIPTOR_RANGE descRangeCBV, descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);


	CD3DX12_ROOT_PARAMETER rootparam[2];
	rootparam[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparam[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);


	spriteParam.range[0] = GRAPHICS_RANGE_TYPE_CBV;
	spriteParam.paramData[0].param = 0;
	spriteParam.paramData[0].type = GRAPHICS_PRAMTYPE_DRAW;
	spriteParam.type[0] = GRAPHICS_ROOTSIGNATURE_TYPE_VIEW;

	spriteParam.range[1] = GRAPHICS_RANGE_TYPE_SRV;
	spriteParam.paramData[1].param = 1;
	spriteParam.paramData[1].type = GRAPHICS_PRAMTYPE_TEX;
	spriteParam.type[1] = GRAPHICS_ROOTSIGNATURE_TYPE_DESCRIPTORTABLE;
	spriteParam.paramMax = 2;

	//ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(
		sizeof(rootparam) / sizeof(rootparam[0]),
		rootparam,
		1,
		&samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//バージョン自動判定でのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);

	//ルートシグネチャの生成
	DirectX12Device::Instance()->dev->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature[rootSignature.size() - 1])
	);

	rootSignature[1]->SetName(L"SpriteRootSig");

}

void GraphicsRootSignature::CreateObjRootSignature() {
	rootSignature.push_back({});

	//サンプラーの生成
	CD3DX12_STATIC_SAMPLER_DESC desc(0);
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = desc;

	//ディスクリプタレンジは後で理解しよう
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);


	CD3DX12_ROOT_PARAMETER rootparam[3];
	rootparam[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparam[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparam[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);


	//SetSRVやSetCBVに渡す際に必要
	objParam.range[0] = GRAPHICS_RANGE_TYPE_CBV;
	objParam.paramData[0].param = 0;
	objParam.paramData[0].type = GRAPHICS_PRAMTYPE_DRAW;
	objParam.type[0] = GRAPHICS_ROOTSIGNATURE_TYPE_VIEW;

	objParam.range[1] = GRAPHICS_RANGE_TYPE_CBV;
	objParam.paramData[1].param = 1;
	objParam.paramData[1].type = GRAPHICS_PRAMTYPE_DATA;			//？？
	objParam.type[1] = GRAPHICS_ROOTSIGNATURE_TYPE_VIEW;

	objParam.range[2] = GRAPHICS_RANGE_TYPE_SRV;
	objParam.paramData[2].param = 2;
	objParam.paramData[2].type = GRAPHICS_PRAMTYPE_TEX;
	objParam.type[2] = GRAPHICS_ROOTSIGNATURE_TYPE_DESCRIPTORTABLE;

	objParam.paramMax = 3;


	//ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(
		sizeof(rootparam) / sizeof(rootparam[0]),
		rootparam,
		1,
		&samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//バージョン自動判定でのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);

	//ルートシグネチャの生成
	DirectX12Device::Instance()->dev->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature[rootSignature.size() - 1])
	);

	rootSignature[1]->SetName(L"ObjRootSig");
}

void GraphicsRootSignature::CreateMultipassRootSignature()
{

	//マルチパスレンダリング

	//マルチパスレンダリング用のルートシグネチャー
	rootSignature.push_back({});

	//正しい
	D3D12_DESCRIPTOR_RANGE range[3] = {};
	range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	range[0].BaseShaderRegister = 0;
	range[0].NumDescriptors = 1;

	range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	range[1].BaseShaderRegister = 0;
	range[1].NumDescriptors = 1;

	range[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	range[2].BaseShaderRegister = 1;
	range[2].NumDescriptors = 1;


	D3D12_ROOT_PARAMETER rp[3] = {};
	rp[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rp[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rp[0].DescriptorTable.pDescriptorRanges = &range[0];
	rp[0].DescriptorTable.NumDescriptorRanges = 1;

	rp[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rp[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rp[1].DescriptorTable.pDescriptorRanges = &range[1];
	rp[1].DescriptorTable.NumDescriptorRanges = 1;

	rp[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rp[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rp[2].DescriptorTable.pDescriptorRanges = &range[2];
	rp[2].DescriptorTable.NumDescriptorRanges = 1;


	//SetSRVやSetCBVに渡す際に必要
	MultiPassParam.range[0] = GRAPHICS_RANGE_TYPE_SRV;
	MultiPassParam.paramData[0].param = 0;
	MultiPassParam.paramData[0].type = GRAPHICS_PRAMTYPE_TEX;			//??
	MultiPassParam.type[0] = GRAPHICS_ROOTSIGNATURE_TYPE_DESCRIPTORTABLE;

	MultiPassParam.range[1] = GRAPHICS_RANGE_TYPE_CBV;
	MultiPassParam.paramData[1].param = 0;
	MultiPassParam.paramData[1].type = GRAPHICS_PRAMTYPE_DRAW;
	MultiPassParam.type[1] = GRAPHICS_ROOTSIGNATURE_TYPE_DESCRIPTORTABLE;

	MultiPassParam.range[2] = GRAPHICS_RANGE_TYPE_SRV;
	MultiPassParam.paramData[2].param = 1;
	MultiPassParam.paramData[2].type = GRAPHICS_PRAMTYPE_TEX;			//??
	MultiPassParam.type[2] = GRAPHICS_ROOTSIGNATURE_TYPE_DESCRIPTORTABLE;

	MultiPassParam.paramMax = 3;


	////新規
	/*D3D12_DESCRIPTOR_RANGE range[2] = {};
	range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	range[0].BaseShaderRegister = 1;
	range[0].NumDescriptors = 1;
	range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	range[1].BaseShaderRegister = 0;
	range[1].NumDescriptors = 1;

	D3D12_ROOT_PARAMETER rp[2] = {};
	rp[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rp[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rp[0].DescriptorTable.pDescriptorRanges = &range[0];
	rp[0].DescriptorTable.NumDescriptorRanges = 1;
	rp[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rp[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rp[1].DescriptorTable.pDescriptorRanges = &range[1];
	rp[1].DescriptorTable.NumDescriptorRanges = 1;*/
	/*


		D3D12_STATIC_SAMPLER_DESC sampler = CD3DX12_STATIC_SAMPLER_DESC(0);


		D3D12_ROOT_SIGNATURE_DESC rsDesc{};
		rsDesc.NumParameters = 3;
		rsDesc.pParameters = rp;
		rsDesc.NumStaticSamplers = 1;
		rsDesc.pStaticSamplers = &sampler;
		rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;



		//バージョン自動判定でのシリアライズ
		ComPtr<ID3DBlob> rootSigBlob = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;

		HRESULT re;
		/*
		re = D3D12SerializeRootSignature(
			&rsDesc,
			D3D_ROOT_SIGNATURE_VERSION_1,
			rootSigBlob.ReleaseAndGetAddressOf(),
			errorBlob.ReleaseAndGetAddressOf()
		);


		//ルートシグネチャの生成
		re = DirectX12Device::Instance()->dev->CreateRootSignature(
			0,
			rootSigBlob->GetBufferPointer(),
			rootSigBlob->GetBufferSize(),
			IID_PPV_ARGS(&rootSignature[rootSignature.size() - 1])
		);
	}

	void GraphicsRootSignature::CreateLight()
	{
		rootSignature.push_back({});

		//サンプラーの生成
		CD3DX12_STATIC_SAMPLER_DESC desc(0);
		CD3DX12_STATIC_SAMPLER_DESC samplerDesc = desc;

		CD3DX12_DESCRIPTOR_RANGE descRangeCBV, descRangeCBV2, descRangeSRV;
		descRangeCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
		descRangeCBV2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
		descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);


		CD3DX12_ROOT_PARAMETER rootparam[3];
		//rootparam[0].InitAsDescriptorTable(1, &descRangeCBV, D3D12_SHADER_VISIBILITY_ALL);
		//rootparam[2].InitAsDescriptorTable(1, &descRangeCBV2, D3D12_SHADER_VISIBILITY_ALL);
		rootparam[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
		rootparam[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
		rootparam[2].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);


		//SetSRVやSetCBVに渡す際に必要
		LightParam.range[0] = GRAPHICS_RANGE_TYPE_CBV;
		LightParam.paramData[0].param = 0;
		LightParam.paramData[0].type = GRAPHICS_PRAMTYPE_DRAW;
		LightParam.type[0] = GRAPHICS_ROOTSIGNATURE_TYPE_VIEW;

		LightParam.range[1] = GRAPHICS_RANGE_TYPE_SRV;
		LightParam.paramData[1].param = 0;
		LightParam.paramData[1].type = GRAPHICS_PRAMTYPE_TEX;
		LightParam.type[1] = GRAPHICS_ROOTSIGNATURE_TYPE_DESCRIPTORTABLE;

		LightParam.range[2] = GRAPHICS_RANGE_TYPE_CBV;
		LightParam.paramData[2].param = 2;
		LightParam.paramData[2].type = GRAPHICS_PRAMTYPE_DATA;
		LightParam.type[2] = GRAPHICS_ROOTSIGNATURE_TYPE_VIEW;
		LightParam.paramMax = 3;


		//ルートシグネチャの設定
		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init_1_0(
			sizeof(rootparam) / sizeof(rootparam[0]),
			rootparam,
			1,
			&samplerDesc,
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		//バージョン自動判定でのシリアライズ
		ComPtr<ID3DBlob> rootSigBlob = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
		D3DX12SerializeVersionedRootSignature(
			&rootSignatureDesc,
			D3D_ROOT_SIGNATURE_VERSION_1_0,
			&rootSigBlob,
			&errorBlob);

		//ルートシグネチャの生成
		DirectX12Device::Instance()->dev->CreateRootSignature(
			0,
			rootSigBlob->GetBufferPointer(),
			rootSigBlob->GetBufferSize(),
			IID_PPV_ARGS(&rootSignature[rootSignature.size() - 1])
		);


		rootSignature[0]->SetName(L"TextureRootSig");

	}
	*/
#pragma endregion 