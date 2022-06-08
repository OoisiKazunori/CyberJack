#include "GameScene.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Input/ControllerInputManager.h"
#include"../Imgui/MyImgui.h"
#include"../Loader/ObjResourceMgr.h"
#include"../Helper/ResourceFilePass.h"
#include"../Game/Debug/ParameterMgr.h"
#include"../Math/KazMath.h"

GameScene::GameScene()
{
#pragma region 敵の配置レベル1
	int respoineIndex = 0;
	int layerLevel = 0;
	int time = 60;
	float z = 300.0f;

	//2-----------------------------------------------------------------
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = 0;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 3;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 20.0f,10.0f,z };
	++respoineIndex;

	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = 0;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 3;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 60.0f,10.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//respoineIndex = 0;
	//2-----------------------------------------------------------------
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = 0;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 6;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -20.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = 0;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 6;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -60.0f,10.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//4-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = 0;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 9;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 20.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = 0;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 9;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 40.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = 0;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 9;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 60.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = 0;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 9;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 80.0f,10.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------






	//4-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = 0;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 12;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -20.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = 0;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 12;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -40.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = 0;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 12;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -60.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = 0;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 12;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -80.0f,10.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------


	//4-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = 0;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 15;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 20.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = 0;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 15;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 40.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = 0;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 15;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 60.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = 0;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 15;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 80.0f,10.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------


	//4-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = 0;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 18;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -20.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = 0;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 18;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -40.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = 0;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 18;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -60.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = 0;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 18;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -80.0f,10.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------

#pragma endregion

#pragma region 敵の配置レベル2
	++layerLevel;
	//5-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 3;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -40.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 3;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -20.0f,10.0f,z };
	++respoineIndex;

	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 3;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 20.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 3;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 40.0f,10.0f,z };
	++respoineIndex;
	//5-----------------------------------------------------------------

	//2-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 6;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -20.0f,-10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 6;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -40.0f,-10.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//2-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_MISILE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].flame = time * 8;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].initPos = { 20.0f,-10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_MISILE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].flame = time * 8;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].initPos = { 40.0f,-10.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------

	//3-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 11;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -20.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 11;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -40.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 11;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -60.0f,20.0f,z };
	++respoineIndex;
	//3-----------------------------------------------------------------


	//3-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 14;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 20.0f,-10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_MISILE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].flame = time * 14;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].initPos = { 40.0f,-10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 14;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 60.0f,-10.0f,z };
	++respoineIndex;
	//3-----------------------------------------------------------------


	//2-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 17;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 10.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 17;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -10.0f,20.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//2-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 19;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 10.0f,-10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_MISILE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].flame = time * 19;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].initPos = { -10.0f,-10.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//2-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 21;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -10.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 21;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -10.0f,0.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//2-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 23;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 10.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 23;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 10.0f,0.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------



	//4-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 26;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -20.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 26;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -20.0f,0.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_MISILE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].flame = time * 26;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].initPos = { -40.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_MISILE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].flame = time * 26;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].initPos = { -40.0f,0.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------



	//4-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 29;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 20.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 29;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 20.0f,0.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 29;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 40.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 29;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { 40.0f,0.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------


	//4-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 32;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -20.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 32;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].initPos = { -20.0f,0.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_MISILE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].flame = time * 32;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].initPos = { 20.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_MISILE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].flame = time * 32;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].initPos = { 20.0f,0.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------
#pragma endregion
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	game = std::make_unique<Game>();
	game->Init(responeData);
}

void GameScene::Finalize()
{
	game.reset();
}

void GameScene::Input()
{
	game->Input();
}

void GameScene::Update()
{
	game->Update();
}

void GameScene::Draw()
{
	game->Draw();
}

int GameScene::SceneChange()
{
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_K) || game->SceneChange() == 0)
	{
		return -2;
	}
	else
	{
		return SCENE_NONE;
	}
}
