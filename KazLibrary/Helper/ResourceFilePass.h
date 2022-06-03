#pragma once
#include"../DirectXCommon/Base.h"

/// <summary>
/// �t�H���_�܂ł̃p�X��Z�߂܂���
/// ���\�[�X�̓ǂݍ��݂Ńp�X�������ꍇ�́A�ǂݍ��݂����t�@�C��������t�H���_����₩��I��Ő擪�ɏ�������A�t�@�C�����������Ă�������
/// </summary>
namespace KazFilePathName
{
	static string RelativeResourcePath = "Resource/";

	//�V�F�[�_�[�p�X-----------------------

	static string RelativeShaderPath = "KazLibrary/ShaderFiles/";
	static string VertexShaderPath = RelativeShaderPath + "VertexShader/";
	static string PixelShaderPath = RelativeShaderPath + "PixelShader/";
	static string GeometryShaderPath = RelativeShaderPath + "GeometryShader/";
	static string ComputeShaderPath = RelativeShaderPath + "ComputeShader/";


	//�摜�p�X-----------------------

	static string TestPath = RelativeResourcePath + "Test/";
	static string CursorPath = RelativeResourcePath + "Cursor/";
	static string HpPath = RelativeResourcePath + "Hp/";
	static string GoalPath = RelativeResourcePath + "Goal/";
	static string GameOverPath = RelativeResourcePath + "GameOver/";
	static string StageUIPath = RelativeResourcePath + "StageUI/";
	static string TitlePath = RelativeResourcePath + "Title/";
	static string SceneChangePath = RelativeResourcePath + "SceneChange/";


	//�f�o�b�N�p�X-----------------------

	static string ParamPath = RelativeResourcePath + "Parameter/";
	static string StageParamPath = ParamPath + "Stage/";

}