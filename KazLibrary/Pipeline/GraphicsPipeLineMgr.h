#pragma once
#include"../DirectXCommon/Base.h"
#include"../Pipeline/Shader.h"
#include"../Helper/RegisterNameWithData.h"
#include"../Helper/ISinglton.h"
#include"../Pipeline/GraphicsRootSignature.h"

struct InputLayOutData
{
	D3D12_INPUT_ELEMENT_DESC *inputLayOut;
	UINT size;
};

/*
������Base�p��enum
�`�[�������Q�[���p�ɂǂ�ǂ�ǉ����鎞�͕ʂ�enum�ōs���̂͂ǂ���
*/
enum PipeLineDataNames
{
	PIPELINE_DATA_BACKCARING_ALPHABLEND,
	PIPELINE_DATA_BACKCARING_ALPHABLEND_RNEDERTARGET_SECOND,
	PIPELINE_DATA_BACKCARING_ALPHABLEND_WIREFLAME,
	PIPELINE_DATA_BACKCARING_ALPHABLEND_WIREFLAME_MUTITEX,
	PIPELINE_DATA_BACKCARING_ALPHABLEND_WIREFLAME_HDR,
	PIPELINE_DATA_NOCARING_NOBLEND,
	PIPELINE_DATA_NOCARING_NOBLEND_R32,				// DXGIFORMAT��R32�ɂ��邽�߂ɗՎ��Ŏ������܂����B�f�v�X�V���h�E�p�ł��B
	PIPELINE_DATA_NOCARING_ALPHABLEND_LINE,
	PIPELINE_DATA_NOCARING_ALPHABLEND_LINE_MULTITEX,
	PIPELINE_DATA_NOCARING_BLENDALPHA,
	PIPELINE_DATA_NOCARING_BLENDALPHA_DEPTH_ALWAYS,
	PIPELINE_DATA_NOCARING_BLENDALPHA_MULTIPASS_TWO,
	PIPELINE_DATA_NOCARING_BLENDALPHA_DEPTHOFF,
	PIPELINE_DATA_NOCARING_BLENDALPHA_CUT,
	PIPELINE_DATA_NOCARING_BLENDALPHA_CUT_MULTITEX,
	PIPELINE_DATA_NOCARING_ADDBLEND_CUTALPHA,
	PIPELINE_DATA_NOCARING_ADDBLEND,
	PIPELINE_DATA_NOCARING_ADDBLEND_HDR,
	PIPELINE_DATA_NOCARING_NOBLEND_HDR,
	PIPELINE_DATA_BACKCARING_ALPHABLEND_POINT_HDR,
	PIPELINE_DATA_NOCARING_ALPHABLEND_LINE_HDR,
	PIPELINE_DATA_NOCARING_BLENDALPHA_HDR,
	PIPELINE_DATA_EXPANTION_VERTEX,					// ���_�g���p�ɑO�ʃJ�����O�ݒ�Œǉ����܂����B
	PIPELINE_DATA_BACKCARING_ALPHABLEND_HDR,
	PIPELINE_DATA_MAX
};

enum ComputePipeLineDataNames
{
	PIPELINE_COMPUTE_DATA_NONE = -1,
	PIPELINE_COMPUTE_DATA_TEST
};

enum InputLayOutNames
{
	LAYOUT_POS_NORMAL_TEX,
	LAYOUT_POS_TEX,
	LAYOUT_POS,
	LAYOUT_POS_NORMAL_COLOR_TEX,
	LAYOUT_POS_NORMAL_TEX_BONE_WEIGHT,
	LAYOUT_POS_TICK,
	LAYOUT_MAX
};

enum VertexShaderNames
{
	SHADER_VERTEX_COLOR,
	SHADER_VERTEX_TEXTURE,
	SHADER_VERTEX_SPRITE,
	SHADER_VERTEX_OBJ,
	SHADER_VERTEX_EXPANTION_OBJ,	// ���_�g���p���_�V�F�[�_�[
	SHADER_VERTEX_GET_SHADOWMAP,	// �V���h�E�}�b�v�擾�p���_�V�F�[�_�[
	SHADER_VERTEX_DRAW_SHADOWMAP,	// �V���h�E�}�b�v�����ɉe�`��p���_�V�F�[�_�[
	SHADER_VERTEX_LIGHT,
	SHADER_VERTEX_LUMINAS,
	SHADER_VERTEX_FBX,
	SHADER_VERTEX_MULTIPASS,
	SHADER_VERTEX_FOG_COLOR,
	SHADER_VERTEX_LINE,
	SHADER_VERTEX_OBJ_LIGHT,
	SHADER_VERTEX_INSTANCE_OBJ,
	SHADER_VERTEX_INSTANCE_COLOR,
	SHADER_VERTEX_INSTANCE_GET_SHADOWMAP,
	SHADER_VERTEX_INSTANCE_COLOR_GET_SHADOWMAP,
	SHADER_VERTEX_GPUPARTICLE,
	SHADER_VERTEX_MAX
};

enum PixcelShaderNames
{
	SHADER_PIXCEL_COLOR,
	SHADER_PIXCEL_COLOR_MULTITEX,
	SHADER_PIXCEL_TEXTURE,
	SHADER_PIXCEL_SPRITE,
	SHADER_PIXCEL_SPRITE_GRADATION,
	SHADER_PIXCEL_SPRITE_MULTITEX,
	SHADER_PIXCEL_SPRITE_GOAL_EFFECT,
	SHADER_PIXCEL_OBJ,
	SHADER_PIXCEL_OBJ_MULTITEX,
	SHADER_PIXCEL_EXPANTION_OBJ,
	SHADER_PIXCEL_GET_SHADOWMAP,	// �V���h�E�}�b�v�擾�p�s�N�Z���V�F�[�_�[
	SHADER_PIXCEL_DRAW_SHADOWMAP,	// �V���h�E�}�b�v�����ɉe�`��p���_�V�F�[�_�[
	SHADER_PIXCEL_LIGHT,
	SHADER_PIXCEL_LUMINAS,
	SHADER_PIXCEL_LUMINANCE_MULTI,
	SHADER_PIXCEL_GAUSSIAN_UPBLUR,
	SHADER_PIXCEL_GAUSSIAN_SIDEBLUR,
	SHADER_PIXCEL_FBX,
	SHADER_PIXCEL_BLOOM,
	SHADER_PIXCEL_GOALLIGHT,
	SHADER_PIXCEL_GOALLIGHT_MULTITEX,
	SHADER_PIXCEL_FOG_COLOR,
	SHADER_PIXCEL_FOG_COLOR_MULTITEX,
	SHADER_PIXCEL_FOG_FBX,
	SHADER_PIXCEL_FOG_OBJ,
	SHADER_PIXCEL_LINE,
	SHADER_PIXCEL_TICK_LINE_FOG,
	SHADER_PIXCEL_FOG_SPRITE,
	SHADER_PIXCEL_LINE_FOG,
	SHADER_PIXCEL_LINE_FOG_FORBG,
	SHADER_PIXEL_LUMINANCE,
	SHADER_PIXCEL_OBJ_LIGHT,
	SHADER_PIXCEL_INSTANCE_OBJ,
	SHADER_PIXCEL_INSTANCE_COLOR,
	SHADER_PIXCEL_INSTANCE_COLOR_GET_SHADOW,
	SHADER_PIXCEL_LINE_UV,
	SHADER_PIXCEL_LINE_UV_MULTITEX,
	SHADER_PIXCEL_LINE_FLASHEFFECT,
	SHADER_PIXCEL_INSTANCE_GET_SHADOWMAP,
	SHADER_PIXCEL_INSTANCE_OBJ_EXPANTION,
	SHADER_PIXCEL_WIHITENOISE,
	SHADER_PIXCEL_FBX_RENDER_TWO,
	SHADER_PIXEL_GPUPARTICLE,
	SHADER_PIXCEL_SPRITE_NOISE,
	SHADER_PIXCEL_MAX
};

enum GeometoryShaderNames
{
	SHADER_GEOMETORY_NONE = -1,
	SHADER_GEOMETORY_LINE
};

enum ComputeShaderNames
{
	SHADER_COMPUTE_NONE = -1,
	SHADER_COMPUTE_TEST
};

enum PipeLineNames
{
	PIPELINE_NAME_COLOR,
	PIPELINE_NAME_COLOR_NOCARING,
	PIPELINE_NAME_COLOR_MULTITEX,
	PIPELINE_NAME_INSTANCE_COLOR,
	PIPELINE_NAME_COLOR_WIREFLAME,
	PIPELINE_NAME_COLOR_WIREFLAME_MULTITEX,
	PIPELINE_NAME_COLOR_WIREFLAME_HDR,
	PIPELINE_NAME_TEXTURE,
	PIPELINE_NAME_SPRITE,
	PIPELINE_NAME_SPRITE_NOISE,
	PIPELINE_NAME_SPRITE_LUMI,
	PIPELINE_NAME_SPRITE_MULTITEX,
	PIPELINE_NAME_SPRITE_Z_ALWAYS,
	PIPELINE_NAME_SPRITE_NOBLEND,
	PIPELINE_NAME_SPRITE_GOAL_EFFECT,
	PIPELINE_NAME_SPRITE_GOAL_EFFECT_MULTITEX,
	PIPELINE_NAME_SPRITE_GOAL_EFFECT_DEPTHOFF,
	PIPELINE_NAME_OBJ,
	PIPELINE_NAME_OBJ_MULTITEX,
	PIPELINE_NAME_OBJ_WIREFLAME,
	PIPELINE_NAME_OBJ_LIGHT,
	PIPELINE_NAME_OBJ_EXPANSION_VERTEX,
	PIPELINE_NAME_OBJ_GET_SHADOWMAP,		// �V���h�E�}�b�v�擾�p�`��p�C�v���C��
	PIPELINE_NAME_OBJ_DRAW_SHADOWMAP,		// �V���h�E�}�b�v�����ɉe�`��p�p�C�v���C��
	PIPELINE_NAME_LINE,
	PIPELINE_NAME_LINE_MULTITEX,
	PIPELINE_NAME_LINE_TICK,
	PIPELINE_NAME_LINE_TICK_HDR,
	PIPELINE_NAME_LIGHT,
	PIPELINE_NAME_LUMINANCE,
	PIPELINE_NAME_GAUSSIAN_UPBLUR,
	PIPELINE_NAME_GAUSSIAN_SIDEBLUR,
	PIPELINE_NAME_FBX,
	PIPELINE_NAME_FBX_RENDERTARGET_TWO,
	PIPELINE_NAME_ADDBLEND,
	PIPELINE_NAME_BLOOM,
	PIPELINE_NAME_MULTIPASS,
	PIPELINE_NAME_GOALLIGHT,
	PIPELINE_NAME_FOG_COLOR,
	PIPELINE_NAME_FOG_COLOR_MULTITEX,
	PIPELINE_NAME_FOG_FBX,
	PIPELINE_NAME_FOG_OBJ,
	PIPELINE_NAME_FOG_LINE_TICK,
	PIPELINE_NAME_FOG_LINE,
	PIPELINE_NAME_FOG_LINE_FORBG,
	PIPELINE_NAME_FOG_LINE_FORBG_HDR,
	PIPELINE_NAME_FOG_SPRITE,
	PIPELINE_NAME_SPRITE_CUTALPHA,
	PIPELINE_NAME_SPRITE_CUTALPHA_MULTITEX,
	PIPELINE_NAME_OBJ_HDR,
	PIPELINE_NAME_OBJ_COLOR_HDR,
	PIPELINE_NAME_LINE_HDR,
	PIPELINE_NAME_SPRITE_ADDBLEND_HDR,
	PIPELINE_NAME_SPRITE_ALPHABLEND_HDR,
	PIPELINE_NAME_SPRITE_ALPHABLEND_ADDCOLOR_HDR,
	PIPELINE_NAME_LUMINANCE_HDR,
	PIPELINE_NAME_FOG_LINE_HDR,
	PIPELINE_NAME_COLOR_HDR,
	PIPELINE_NAME_SPRITE_HDR,
	PIPELINE_NAME_SPRITE_VHS,
	PIPELINE_NAME_SPRITE_GRADATION,
	PIPELINE_NAME_INSTANCE_OBJ,
	PIPELINE_NAME_INSTANCE_OBJ_GET_SHADOWMAP,
	PIPELINE_NAME_INSTANCE_OBJ_EXPANTION_VERTEX,
	PIPELINE_NAME_INSTANCE_COLOR_GET_SHADOWMAP,
	PIPELINE_NAME_LINE_UV,
	PIPELINE_NAME_LINE_FLASHEFFECT,
	PIPELINE_NAME_LINE_UV_MULTITEX,
	PIPELINE_NAME_GPUPARTICLE,
	PIPELINE_MAX
};

enum ComputePipeLineNames
{
	PIPELINE_COMPUTE_NAME_NONE = -1,
	PIPELINE_COMPUTE_NAME_TEST
};

/// <summary>
/// �p�C�v���C���̊Ǘ��A�����̖��������N���X�ł�
/// InputLayOut,Shader,D3D12_GRAPHICS_PIPELINE_STATE_DESC��\�ߓo�^���A
/// ���̌�A���ꂼ��̓o�^�����g�p���p�C�v���C���𐶐����܂�
/// </summary>
class GraphicsPipeLineMgr :public ISingleton<GraphicsPipeLineMgr>
{
public:
	GraphicsPipeLineMgr();
	~GraphicsPipeLineMgr();

	/// <summary>
	/// InputLayOut�̏��Ɩ��O��o�^���܂�
	/// </summary>
	/// <param name="INPUT_LAYOUT">�o�^������INPUTLAYOUT�̕ϐ�</param>
	/// <param name="NAME">�o�^��</param>
	void RegisterInputLayOutWithData(InputLayOutData INPUT_LAYOUT, InputLayOutNames NAME);

	/// <summary>
	/// ���_�V�F�[�_�[�̃f�[�^�Ɩ��O��o�^���܂�
	/// </summary>
	/// <param name="VS_BLOB">�o�^������Shader�N���X�ŃR���p�C���ς݂̒��_�V�F�[�_�[</param>
	/// <param name="NAME">�o�^��</param>
	void RegisterVertexShaderWithData(string SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL, VertexShaderNames NAME);

	/// <summary>
	/// �s�N�Z���V�F�[�_�[�̃f�[�^�Ɩ��O��o�^���܂�
	/// </summary>
	/// <param name="PS_BLOB">�o�^������Shader�N���X�ŃR���p�C���ς݂̃s�N�Z���V�F�[�_�[</param>
	/// <param name="NAME">�o�^��</param>
	void RegisterPixcelShaderWithData(string SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL, PixcelShaderNames NAME);

	/// <summary>
	/// �W�I���g���[�V�F�[�_�[�̃f�[�^�Ɩ��O��o�^���܂�
	/// </summary>
	/// <param name="GS_BLOB">�o�^������Shader�N���X�ŃR���p�C���ς݂̃W�I���g���[�V�F�[�_�[</param>
	/// <param name="NAME">�o�^��</param>
	void RegisterGeometoryShaderWithData(string SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL, GeometoryShaderNames NAME);

	/// <summary>
	/// �R���s���[�g�V�F�[�_�[�̃f�[�^�Ɩ��O��o�^���܂�
	/// </summary>
	/// <param name="GS_BLOB">�o�^������Shader�N���X�ŃR���p�C���ς݂̃R���s���[�g�V�F�[�_�[</param>
	/// <param name="NAME">�o�^��</param>
	void RegisterComputeShaderWithData(string SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL, ComputeShaderNames NAME);

	/// <summary>
	/// PipeLine�̐ݒ�Ɩ��O��o�^���܂�
	/// </summary>
	/// <param name="PIPELINE_DATA">�o�^�������p�C�v���C���̐ݒ�</param>
	/// <param name="NAME">�o�^��</param>
	void RegisterPipeLineDataWithData(D3D12_GRAPHICS_PIPELINE_STATE_DESC PIPELINE_DATA, PipeLineDataNames NAME);

	/// <summary>
	/// ComputePipeLine�̐ݒ�Ɩ��O��o�^���܂�
	/// </summary>
	/// <param name="PIPELINE_DATA">�o�^�������p�C�v���C���̐ݒ�</param>
	/// <param name="NAME">�o�^��</param>
	void RegisterComputePipeLineDataWithData(D3D12_COMPUTE_PIPELINE_STATE_DESC PIPELINE_DATA, ComputePipeLineDataNames NAME);

	/// <summary>
	/// �o�^���ꂽ�f�[�^�ƃ��[�g�V�O�l�`����g�݂킹�āA�p�C�v���C���𐶐����܂�
	/// </summary>
	/// <param name="INPUT_LAYOUT_NAME">�o�^����INPUT_LAYOUT�̖��O</param>
	/// <param name="SHADER_NAME">�o�^����Shader�̖��O</param>
	/// <param name="PIPELINE_DATA_NAME">�o�^����PipeLineData�̖��O</param>
	/// <param name="ROOT_SIGNATURE">���[�g�V�O�l�`���̐ݒ�</param>
	/// <param name="NAME">�p�C�v���C���̓o�^��</param>
	void CreatePipeLine(InputLayOutNames INPUT_LAYOUT_NAME, VertexShaderNames VERTEX_SHADER_NAME, PixcelShaderNames PIXCEL_SHADER_NAME, PipeLineDataNames PIPELINE_DATA_NAME, RootSignatureMode ROOTSIGNATURE, PipeLineNames PIPELINE_NAME, GeometoryShaderNames GEOMETORY_SHADER_NAME = SHADER_GEOMETORY_NONE);

	/// <summary>
	/// �o�^���ꂽ�f�[�^�ƃ��[�g�V�O�l�`����g�݂킹�āA�p�C�v���C���𐶐����܂�
	/// </summary>
	/// <param name="SHADER_NAME">�o�^����Shader�̖��O</param>
	/// <param name="PIPELINE_DATA_NAME">�o�^����PipeLineData�̖��O</param>
	/// <param name="ROOT_SIGNATURE">���[�g�V�O�l�`���̐ݒ�</param>
	/// <param name="NAME">�p�C�v���C���̓o�^��</param>
	void CreateComputePipeLine(ComputeShaderNames COMPUTE_SHADER_NAME, ComputePipeLineDataNames PIPELINE_DATA_NAME, RootSignatureMode ROOTSIGNATURE, ComputePipeLineNames PIPELINE_NAME);

	/// <summary>
	/// �o�^���ꂽ�p�C�v���C���̒�����w��̃p�C�v���C�����R�}���h���X�g�ɐς݂܂�
	/// </summary>
	/// <param name="PIPELINE_NAME">�o�^�����p�C�v���C���̖��O</param>
	bool SetPipeLineAndRootSignature(PipeLineNames PIPELINE_NAME);


	/// <summary>
	/// �o�^���ꂽ�R���s���[�g�p�C�v���C���̒�����w��̃p�C�v���C�����R�}���h���X�g�ɐς݂܂�
	/// </summary>
	/// <param name="PIPELINE_NAME">�o�^�����p�C�v���C���̖��O</param>
	bool SetComputePipeLineAndRootSignature(ComputePipeLineNames PIPELINE_NAME);


	/// <summary>
	/// �p�C�v���C���Ɗ֘A�t���������[�g�V�O�l�`���[���Ăяo���܂�
	/// </summary>
	/// <param name="PIPELINE_NAME">�p�C�v���C���̖��O</param>
	/// <returns>�p�C�v���C���Ɗ֘A�t���������[�g�V�O�l�`���[</returns>
	RootSignatureMode GetRootSignatureName(PipeLineNames PIPELINE_NAME);

private:

	static const int PIPELINE_MAX_NUM = 100;
	//�o�^�f�[�^

	std::array<InputLayOutData, PIPELINE_MAX_NUM> InputLayOutRegisterData;
	std::array<Microsoft::WRL::ComPtr<IDxcBlob>, PIPELINE_MAX_NUM> VertexShaderRegisterData;
	std::array<Microsoft::WRL::ComPtr<IDxcBlob>, PIPELINE_MAX_NUM> PixcelShaderRegisterData;
	std::array<Microsoft::WRL::ComPtr<IDxcBlob>, PIPELINE_MAX_NUM> GeometoryShaderRegisterData;
	std::array<RootSignatureMode, PIPELINE_MAX_NUM> RootSignatureName;
	std::array<D3D12_GRAPHICS_PIPELINE_STATE_DESC, PIPELINE_MAX_NUM> PipeLineDataRegisterData;
	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, PIPELINE_MAX_NUM> PipeLineRegisterData;

	std::array<Microsoft::WRL::ComPtr<IDxcBlob>, PIPELINE_MAX_NUM> ComputeShaderRegisterData;
	std::array<D3D12_COMPUTE_PIPELINE_STATE_DESC, PIPELINE_MAX_NUM> computePipeLineDataRegisterData;
	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, PIPELINE_MAX_NUM> ComputePipeLineRegisterData;
	std::array<RootSignatureMode, PIPELINE_MAX_NUM> computeRootSignatureName;

	bool geo;
	template <typename T>
	bool IsitSafe(T NAME, size_t SIZE);
};


/*
�^��_
�p�C�v���C���N���X�̎��������[�X�@�\...����H
�g�ݍ��킹�����o�^���āA1F�ň��ł��g���Ă����琶���A�g���Ă��Ȃ���Δj��
*/

