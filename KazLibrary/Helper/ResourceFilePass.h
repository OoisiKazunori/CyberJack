#pragma once
#include"../DirectXCommon/Base.h"

/// <summary>
/// フォルダまでのパスを纏めました
/// リソースの読み込みでパスを書く場合は、読み込みたいファイルがあるフォルダを候補から選んで先頭に書いた後、ファイル名を書いてください
/// </summary>
namespace KazFilePathName
{
	static string RelativeResourcePath = "Resource/";

	//シェーダーパス-----------------------

	static string RelativeShaderPath = RelativeResourcePath + "ShaderFiles/";
	static string VertexShaderPath = RelativeShaderPath + "VertexShader/";
	static string PixelShaderPath = RelativeShaderPath + "PixelShader/";
	static string GeometryShaderPath = RelativeShaderPath + "GeometryShader/";
	static string ComputeShaderPath = RelativeShaderPath + "ComputeShader/";


	//画像パス-----------------------

	static string TestPath = RelativeResourcePath + "Test/";
	static string CursorPath = RelativeResourcePath + "Cursor/";
	static string HpPath = RelativeResourcePath + "Hp/";
	static string GoalPath = RelativeResourcePath + "Goal/";
	static string GameOverPath = RelativeResourcePath + "GameOver/";
	static string StageUIPath = RelativeResourcePath + "StageUI/";
	static string TitlePath = RelativeResourcePath + "Title/";
	static string SceneChangePath = RelativeResourcePath + "SceneChange/";
	static string TutorialPath = RelativeResourcePath + "Tutorial/";
	static string EnemyPath = RelativeResourcePath + "Enemy/";
	static string LinePath = RelativeResourcePath + "Line/";
	static string SoundPath = RelativeResourcePath + "Sound/";


	//デバックパス-----------------------

	static string ParamPath = RelativeResourcePath + "Parameter/";
	static string StageParamPath = ParamPath + "Stage/";

}