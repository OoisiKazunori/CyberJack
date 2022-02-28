#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/ISinglton.h"
#include<vector>
using namespace std;


/// <summary>
/// ���[�g�V�O�l�`���[�̎��
/// </summary>
enum RootSignatureMode
{
	ROOTSIGNATURE_DATA_DRAW,
	//�폜�\��
	ROOTSIGNATURE_DATA_DRAW_TEX,
	ROOTSIGNATURE_DATA_DRAW_TEX_DATA1,
	ROOTSIGNATURE_DATA_DRAW_TEX_DATA1_DATA2,
	ROOTSIGNATURE_DATA_DRAW_TEX_TEX2_DATA1_DATA2,
	//�폜�\��
	ROOTSIGNATURE_MODE_MULTIPASS,
	//�폜�\��
	ROOTSIGNATURE_MODE_LIGHT,
	ROOTSIGNATURE_MODE_LIGHT_BORDER,
	ROOTSIGNATURE_DATA_DRAW_TEX4,
	ROOTSIGNATURE_DATA_DRAW_TEX_SKINING,
	ROOTSIGNATURE_DATA_DRAW_DATA1,
	ROOTSIGNATURE_DATA_DRAW_TEX_SKINING_DATA1,
	ROOTSIGNATURE_DATA_DRAW_DATA1_TEX_DATA2,
	//�폜�\��
	ROOTSIGNATURE_MODE_LINE,
	ROOTSIGNATURE_DATA_DATA1_DATA2_TEX,
	ROOTSIGNATURE_DATA_DATA1_DATA2_DATA3_TEX,
	ROOTSIGNATURE_DATA_DATA1,
	ROOTSIGNATURE_DATA_DATA1_DATA2,
	//�e�X�g�p
	ROOTSIGNATURE_DATA_SRV_UAV_CBV,
};


/// <summary>
/// SetSRV��SetCBV�̈����ɒl��n�����ۂ�
///�R�}���h���X�g��SetGraphicsRootConstantBufferView()�ɒl��n���̂�
///SetGraphicsDescriptorTable()�ɒl��n���̂����f����ۂɕK�v
/// </summary>
enum GraphicsRootSignatureType
{
	GRAPHICS_ROOTSIGNATURE_TYPE_NONE = -1,
	GRAPHICS_ROOTSIGNATURE_TYPE_DESCRIPTORTABLE,
	GRAPHICS_ROOTSIGNATURE_TYPE_VIEW
};

/// <summary>
/// �ǂ���������ނ̃f�X�N���v�^�����W�Ȃ̂����f����ۂɕK�v
/// </summary>
enum GraphicsRangeType
{
	GRAPHICS_RANGE_TYPE_NONE = -1,
	GRAPHICS_RANGE_TYPE_SRV,
	GRAPHICS_RANGE_TYPE_UAV,
	GRAPHICS_RANGE_TYPE_CBV,
	GRAPHICS_RANGE_TYPE_CBV_DESC,
	GRAPHICS_RANGE_TYPE_SAMPLER
};

/// <summary>
/// ���̃��[�g�p�����͂ǂ������������������Ă���̂�
/// </summary>
enum GraphicsRootParamType
{
	GRAPHICS_PRAMTYPE_NONE = -1,
	//�`��n�̂ݎg�p
	GRAPHICS_PRAMTYPE_DRAW,
	//�e�N�X�`���̂ݎg�p
	GRAPHICS_PRAMTYPE_TEX,
	GRAPHICS_PRAMTYPE_TEX2,
	//�}�e���A���f�[�^�̂ݎg�p
	GRAPHICS_PRAMTYPE_MATERIAL,
	//���̑�(���C�g�f�[�^�Ȃǎg�p)
	GRAPHICS_PRAMTYPE_DATA,
	GRAPHICS_PRAMTYPE_DATA2,
	GRAPHICS_PRAMTYPE_DATA3,
	//�X�L�j���O���̂ݎg�p
	GRAPHICS_PRAMTYPE_SKINING
};

/// <summary>
/// ���[�g�p�����[�^�[�̔ԍ��ƃ��[�g�p�����[�^�[�̖������w�肵�܂�
/// </summary>
struct RootParamData
{
	short param;
	GraphicsRootParamType type;
};


/// <summary>
/// SetSRV�A��������SetCBV�ɂāA ���[�g�p�����̔ԍ��ƃf�B�X�N���v�^�q�[�v�Ƃ��Đς܂��邩�A�r���[�Ƃ��Đς܂��邩
/// �𔻒f����ۂɕK�v�ɂȂ�\���̂ł�
/// </summary>
struct GraphicsRootSignatureParameter
{
	GraphicsRangeType range[10];			//���
	RootParamData paramData[10];			//�p�����[�^�[
	GraphicsRootSignatureType type[10];		//�ǂ̃Z�b�g���邩
	short paramMax;		    				//�g�p�����p�����[�^�[�̐�

	//������
	GraphicsRootSignatureParameter()
	{
		for (int i = 0; i < 10; i++)
		{
			range[i] = GRAPHICS_RANGE_TYPE_NONE;
			paramData[i].param = -1;
			paramData[i].type = GRAPHICS_PRAMTYPE_NONE;
			type[i] = GRAPHICS_ROOTSIGNATURE_TYPE_NONE;
		}
		paramMax = -1;
	}
};

/// <summary>
/// ���[�g�V�O�l�`���𐶐�����ۂɕK�v�ɂȂ�f�[�^�ł�
/// </summary>
struct RootSignatureData
{
	GraphicsRangeType range[10];
	RootParamData paramData[10];
	D3D12_STATIC_SAMPLER_DESC sample;

	RootSignatureData()
	{
		sample.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sample.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sample.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sample.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sample.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		sample.MaxLOD = D3D12_FLOAT32_MAX;
		sample.MinLOD = 0.0f;
		sample.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sample.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		sample.MaxAnisotropy = 16;
		sample.MipLODBias = 0;
		sample.ShaderRegister = 0;
		sample.RegisterSpace = 0;
	}
};


/// <summary>
/// ���[�g�V�O�l�`���[�𐶐��A�Ǘ����܂�
/// </summary>
class GraphicsRootSignature :public ISingleton<GraphicsRootSignature>
{
public:
	GraphicsRootSignature();
	~GraphicsRootSignature();

	/// <summary>
	/// ���[�g�V�O�l�`���𐶐����܂�
	/// </summary>
	/// <param name="ROOTSIGNATURE_DATA">���[�g�V�O�l�`���𐶐�����ۂɕK�v�ɂȂ�f�[�^</param>
	void CreateRootSignature(RootSignatureMode ROOTSIGNATURE, RootSignatureData ROOTSIGNATURE_DATA, short DATA_MAX);

	/// <summary>
	/// �w��̃��[�g�V�O�l�`�����R�}���h���X�g�ɐς݂܂�
	/// </summary>
	/// <param name="ROOTSIGNATURE_MODE">�ǂ̃��[�g�V�O�l�`����ݒ肷�邩</param>
	void SetRootSignature(RootSignatureMode ROOTSIGNATURE_MODE);

	/// <summary>
	/// �w��̃��[�g�V�O�l�`�����R�}���h���X�g�ɐς݂܂�
	/// </summary>
	/// <param name="ROOTSIGNATURE_MODE">�ǂ̃��[�g�V�O�l�`����ݒ肷�邩</param>
	void SetComputeRootSignature(RootSignatureMode ROOTSIGNATURE_MODE);


	/// <summary>
	/// �w��̃��[�g�V�O�l�`���[��Ԃ�l�œn���܂�
	/// </summary>
	/// <param name="ROOT_SIGNATURE">�w��̃��[�g�V�O�l�`���[</param>
	/// <returns>�������ꂽ���[�g�V�O�l�`���[</returns>
	const ComPtr<ID3D12RootSignature> GetRootSignature(RootSignatureMode ROOT_SIGNATURE);

	/// <summary>
	/// �w��̃��[�g�V�O�l�`���[���ɐݒ肳�ꂽ���[�g�p�����[�^�[��n���܂�
	/// </summary>
	/// <param name="ROOTSIGNATURE_MODE">�w��̃��[�g�V�O�l�`���[</param>
	/// <returns>���[�g�p�����[�^�[</returns>
	const GraphicsRootSignatureParameter GetRootParam(RootSignatureMode ROOTSIGNATURE_MODE);

private:
	/*void CreateColorRootSignature();
	void CreateTextureRootSignature();
	void CreateSpriteRootSignature();
	void CreateObjRootSignature();
	void CreateMultipassRootSignature();
	void CreateLight();

	GraphicsRootSignatureParameter paramMgr[10];
	GraphicsRootSignatureParameter textureParam;
	GraphicsRootSignatureParameter spriteParam;
	GraphicsRootSignatureParameter objParam;
	GraphicsRootSignatureParameter MultiPassParam;
	GraphicsRootSignatureParameter LightParam;
	GraphicsRootSignatureParameter colorParam;*/

	vector<ComPtr<ID3D12RootSignature>> rootSignature;
	vector<GraphicsRootSignatureParameter> paramD;
	friend ISingleton<GraphicsRootSignature>;


	void CreateMyRootSignature(D3D12_STATIC_SAMPLER_DESC SAMPLER_DATA, D3D12_ROOT_PARAMETER *ROOT_PARAM_DATA, short DATA_MAX, RootSignatureMode ROOTSIGNATURE);

	RootSignatureMode CheckEnum(RootSignatureMode MODE)
	{
		RootSignatureMode tmp = MODE;
		return tmp;
	};
};


/*
������

���[�g�p�����𕪂��闝�R
1.�\���̂��ۂ�
2.�g��������P��
��.
���C�g�ƃI�u�W�F�N�g
���C�g�͈�x�����������Œl�����̂܂܎g���邪�A�I�u�W�F�N�g�͕p�ɂɐ؂�ւ���
�e�N�X�`���̐؂�ւ������S������...
�p�[�e�B�N�����ƈ��Z�b�g���ĕ`�悷��
*/