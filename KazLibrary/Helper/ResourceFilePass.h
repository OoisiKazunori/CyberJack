#pragma once
#include"../DirectXCommon/Base.h"

/// <summary>
/// �t�H���_�܂ł̃p�X��Z�߂܂���
/// ���\�[�X�̓ǂݍ��݂Ńp�X�������ꍇ�́A�ǂݍ��݂����t�@�C��������t�H���_����₩��I��Ő擪�ɏ�������A�t�@�C�����������Ă�������
/// </summary>
namespace KazFilePathName
{
	static string RelativeResourcePath = "Resource/";
	static string RelativeShaderPath = "KazLibrary/ShaderFiles/";
	static string VertexShaderPath = RelativeShaderPath + "VertexShader/";
	static string PixelShaderPath = RelativeShaderPath + "PixelShader/";
	static string GeometryShaderPath = RelativeShaderPath + "GeometryShader/";
	static string ComputeShaderPath = RelativeShaderPath + "ComputeShader/";

	static string TestPath = RelativeResourcePath + "Test/";
	static string CursorPath = RelativeResourcePath + "Cursor/";
	static string GoalPath = RelativeResourcePath + "Goal/";

}