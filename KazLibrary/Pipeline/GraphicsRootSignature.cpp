#include"../Pipeline/GraphicsRootSignature.h"
#include"../DirectXCommon/DirectX12Device.h"
#include"../DirectXCommon/DirectX12CmdList.h"
#include"../KazLibrary/Helper/KazHelper.h"

#define STR(var) #var   //�����ɂ����ϐ���ϐ��������������񃊃e�����Ƃ��ĕԂ��}�N���֐�

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

#pragma region ���[�g�p�����̐ݒ�

	int param1 = 0;
	int param2 = 0;
	paramD[ROOTSIGNATURE].paramMax = DATA_MAX;
	for (int i = 0; i < DATA_MAX; i++)
	{
		switch (ROOTSIGNATURE_DATA.range[i])
		{
		case GRAPHICS_RANGE_TYPE_CBV:
			//���[�g�p�����̐ݒ�
			rootparam[i].InitAsConstantBufferView(param1, 0, D3D12_SHADER_VISIBILITY_ALL);

			//�󂯓n���p
			paramD[ROOTSIGNATURE].range[i] = GRAPHICS_RANGE_TYPE_CBV;
			paramD[ROOTSIGNATURE].paramData[i].param = ROOTSIGNATURE_DATA.paramData[i].param;
			paramD[ROOTSIGNATURE].paramData[i].type = ROOTSIGNATURE_DATA.paramData[i].type;
			paramD[ROOTSIGNATURE].type[i] = GRAPHICS_ROOTSIGNATURE_TYPE_VIEW;
			param1++;
			break;

		case GRAPHICS_RANGE_TYPE_SRV:
			//���[�g�p�����̐ݒ�
			descRangeSRV.push_back({});
			descRangeSRV[descRangeSRV.size() - 1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, param2);
			rootparam[i].InitAsDescriptorTable(1, &descRangeSRV[descRangeSRV.size() - 1], D3D12_SHADER_VISIBILITY_ALL);

			//�󂯓n���p
			paramD[ROOTSIGNATURE].range[i] = GRAPHICS_RANGE_TYPE_SRV;
			paramD[ROOTSIGNATURE].paramData[i].param = ROOTSIGNATURE_DATA.paramData[i].param;
			paramD[ROOTSIGNATURE].paramData[i].type = ROOTSIGNATURE_DATA.paramData[i].type;
			paramD[ROOTSIGNATURE].type[i] = GRAPHICS_ROOTSIGNATURE_TYPE_DESCRIPTORTABLE;
			param2++;
			break;

		case GRAPHICS_RANGE_TYPE_SRV_VIEW:
			//���[�g�p�����̐ݒ�
			rootparam[i].InitAsShaderResourceView(param1, 0, D3D12_SHADER_VISIBILITY_ALL);

			//�󂯓n���p
			paramD[ROOTSIGNATURE].range[i] = GRAPHICS_RANGE_TYPE_SRV;
			paramD[ROOTSIGNATURE].paramData[i].param = ROOTSIGNATURE_DATA.paramData[i].param;
			paramD[ROOTSIGNATURE].paramData[i].type = ROOTSIGNATURE_DATA.paramData[i].type;
			paramD[ROOTSIGNATURE].type[i] = GRAPHICS_ROOTSIGNATURE_TYPE_VIEW;
			param1++;
			break;

		case GRAPHICS_RANGE_TYPE_CBV_DESC:
			//���[�g�p�����̐ݒ�
			descRangeSRV.push_back({});
			descRangeSRV[descRangeSRV.size() - 1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, param2);
			rootparam[i].InitAsDescriptorTable(1, &descRangeSRV[descRangeSRV.size() - 1], D3D12_SHADER_VISIBILITY_ALL);

			//�󂯓n���p
			paramD[ROOTSIGNATURE].range[i] = GRAPHICS_RANGE_TYPE_CBV_DESC;
			paramD[ROOTSIGNATURE].paramData[i].param = ROOTSIGNATURE_DATA.paramData[i].param;
			paramD[ROOTSIGNATURE].paramData[i].type = ROOTSIGNATURE_DATA.paramData[i].type;
			paramD[ROOTSIGNATURE].type[i] = GRAPHICS_ROOTSIGNATURE_TYPE_DESCRIPTORTABLE;
			param2++;
			break;

		case GRAPHICS_RANGE_TYPE_UAV_VIEW:
			//���[�g�p�����̐ݒ�
			rootparam[i].InitAsUnorderedAccessView(param1, 0, D3D12_SHADER_VISIBILITY_ALL);

			//�󂯓n���p
			paramD[ROOTSIGNATURE].range[i] = GRAPHICS_RANGE_TYPE_UAV_VIEW;
			paramD[ROOTSIGNATURE].paramData[i].param = ROOTSIGNATURE_DATA.paramData[i].param;
			paramD[ROOTSIGNATURE].paramData[i].type = ROOTSIGNATURE_DATA.paramData[i].type;
			paramD[ROOTSIGNATURE].type[i] = GRAPHICS_ROOTSIGNATURE_TYPE_VIEW;
			param1++;
			break;

		case GRAPHICS_RANGE_TYPE_UAV_DESC:
			//���[�g�p�����̐ݒ�
			descRangeSRV.push_back({});
			descRangeSRV[descRangeSRV.size() - 1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, param2);
			rootparam[i].InitAsDescriptorTable(1, &descRangeSRV[descRangeSRV.size() - 1], D3D12_SHADER_VISIBILITY_ALL);

			//�󂯓n���p
			paramD[ROOTSIGNATURE].range[i] = GRAPHICS_RANGE_TYPE_UAV_VIEW;
			paramD[ROOTSIGNATURE].paramData[i].param = ROOTSIGNATURE_DATA.paramData[i].param;
			paramD[ROOTSIGNATURE].paramData[i].type = ROOTSIGNATURE_DATA.paramData[i].type;
			paramD[ROOTSIGNATURE].type[i] = GRAPHICS_ROOTSIGNATURE_TYPE_DESCRIPTORTABLE;
			param2++;
		case -1:
			break;
		}
	}
#pragma endregion

	if (ROOTSIGNATURE == ROOTSIGNATURE_DATA_SRV_UAV)
	{
		//�R���s���[�g�V�F�[�_�[�p�̃��[�g�V�O�l�`���[(�Վ��p)
		std::array <CD3DX12_DESCRIPTOR_RANGE, 3> ranges{};
		ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);
		ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 1);
		ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

		std::array<CD3DX12_ROOT_PARAMETER, 3> computeRootParameters;
		computeRootParameters[0].InitAsDescriptorTable(1, &ranges[0]);
		computeRootParameters[1].InitAsDescriptorTable(1, &ranges[1]);
		computeRootParameters[2].InitAsConstantBufferView(0, 0);
		//computeRootParameters[5].InitAsDescriptorTable(1, &ranges[4]);
		CreateMyRootSignature(ROOTSIGNATURE_DATA.sample, computeRootParameters.data(), computeRootParameters.size(), ROOTSIGNATURE);
	}
	else if (ROOTSIGNATURE == ROOTSIGNATURE_DATA_DRAW_UAV)
	{
		std::array<CD3DX12_ROOT_PARAMETER, 1> computeRootParameters;
		computeRootParameters[0].InitAsUnorderedAccessView(0, 0);
		CreateMyRootSignature(ROOTSIGNATURE_DATA.sample, computeRootParameters.data(), computeRootParameters.size(), ROOTSIGNATURE);

	}
	else if (ROOTSIGNATURE == ROOTSIGNATURE_DATA_UAB_UAB_UAB_CB)
	{
		std::array<CD3DX12_ROOT_PARAMETER, 5> computeRootParameters;
		std::array <CD3DX12_DESCRIPTOR_RANGE, 4> ranges{};
		ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);
		ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 1);
		ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 2);
		ranges[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 3);
		computeRootParameters[0].InitAsDescriptorTable(1, &ranges[0]);
		computeRootParameters[1].InitAsDescriptorTable(1, &ranges[1]);
		computeRootParameters[2].InitAsDescriptorTable(1, &ranges[2]);
		computeRootParameters[3].InitAsDescriptorTable(1, &ranges[3]);
		computeRootParameters[4].InitAsConstantBufferView(0, 0);
	
		CreateMyRootSignature(ROOTSIGNATURE_DATA.sample, computeRootParameters.data(), computeRootParameters.size(), ROOTSIGNATURE);
	}
	else
	{
		CreateMyRootSignature(ROOTSIGNATURE_DATA.sample, rootparam.data(), rootparam.size(), ROOTSIGNATURE);
	}


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

const Microsoft::WRL::ComPtr<ID3D12RootSignature> GraphicsRootSignature::GetRootSignature(RootSignatureMode ROOT_SIGNATURE)
{
	return rootSignature[ROOT_SIGNATURE];
}

void GraphicsRootSignature::CreateMyRootSignature(D3D12_STATIC_SAMPLER_DESC SAMPLER_DATA, D3D12_ROOT_PARAMETER *ROOT_PARAM_DATA, size_t DATA_MAX, RootSignatureMode ROOTSIGNATURE)
{
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(
		static_cast<UINT>(DATA_MAX),
		ROOT_PARAM_DATA,
		1,
		&SAMPLER_DATA,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//�o�[�W������������ł̃V���A���C�Y
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);

	//���[�g�V�O�l�`���̐���
	DirectX12Device::Instance()->dev->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature[ROOTSIGNATURE])
	);
}

void GraphicsRootSignature::CreateMyRootSignature(D3D12_STATIC_SAMPLER_DESC SAMPLER_DATA, D3D12_ROOT_PARAMETER1 *ROOT_PARAM_DATA, size_t DATA_MAX, RootSignatureMode ROOTSIGNATURE)
{
	SAMPLER_DATA = {};
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	if (ROOTSIGNATURE == ROOTSIGNATURE_DATA_DRAW)
	{
		rootSignatureDesc.Init_1_1(static_cast<UINT>(DATA_MAX), ROOT_PARAM_DATA, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	}
	else
	{
		rootSignatureDesc.Init_1_1(static_cast<UINT>(DATA_MAX), ROOT_PARAM_DATA);
	}
	//�o�[�W������������ł̃V���A���C�Y
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);

	//���[�g�V�O�l�`���̐���
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