#pragma once
#include"../DirectXCommon/Base.h"
#include"LineRender.h"
#include"../Pipeline/GraphicsPipeLineMgr.h"


/// <summary>
/// デバック用の基準線を描画します
/// </summary>
class BackGroundForDebug
{
public:
	BackGroundForDebug();

	/// <summary>
	/// X...赤軸、Y...緑軸、Z...青軸
	/// それぞれプラス方向に向いてます
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

