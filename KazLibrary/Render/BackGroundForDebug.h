#pragma once
#include"../DirectXCommon/Base.h"
#include"LineRender.h"
#include"../Pipeline/GraphicsPipeLineMgr.h"


/// <summary>
/// �f�o�b�N�p�̊����`�悵�܂�
/// </summary>
class BackGroundForDebug
{
public:
	BackGroundForDebug();

	/// <summary>
	/// X...�Ԏ��AY...�Ύ��AZ...��
	/// ���ꂼ��v���X�����Ɍ����Ă܂�
	/// </summary>
	void Draw();

private:
	//LineRender verticalLine[100], horizontalLine[100], rayDir;
	DirectX::XMFLOAT3 verticalLinePos[2][100], horizontalLinePos[2][100];

	DirectX::XMFLOAT3 xLinePos, yLinePos, zLinePos;
	//LineRender xLine, yLine, zLine;

	array<LineRender,100> verticalLine, horizontalLine;
	LineRender xLine, yLine, zLine, rayDir;

	KazMath::Color color;
};

