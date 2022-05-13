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
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 3;
	responeData[0][respoineIndex].initPos = { 20.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 3;
	responeData[0][respoineIndex].initPos = { 60.0f,10.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------

	//2-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 6;
	responeData[0][respoineIndex].initPos = { -20.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 6;
	responeData[0][respoineIndex].initPos = { -60.0f,10.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//4-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 9;
	responeData[0][respoineIndex].initPos = { 20.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 9;
	responeData[0][respoineIndex].initPos = { 40.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 9;
	responeData[0][respoineIndex].initPos = { 60.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 9;
	responeData[0][respoineIndex].initPos = { 80.0f,10.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------






	//4-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 12;
	responeData[0][respoineIndex].initPos = { -20.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 12;
	responeData[0][respoineIndex].initPos = { -40.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 12;
	responeData[0][respoineIndex].initPos = { -60.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 12;
	responeData[0][respoineIndex].initPos = { -80.0f,10.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------


	//4-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 15;
	responeData[0][respoineIndex].initPos = { 20.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 15;
	responeData[0][respoineIndex].initPos = { 40.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 15;
	responeData[0][respoineIndex].initPos = { 60.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 15;
	responeData[0][respoineIndex].initPos = { 80.0f,10.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------


	//4-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 18;
	responeData[0][respoineIndex].initPos = { -20.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 18;
	responeData[0][respoineIndex].initPos = { -40.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 18;
	responeData[0][respoineIndex].initPos = { -60.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 18;
	responeData[0][respoineIndex].initPos = { -80.0f,10.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------

#pragma endregion

#pragma region 敵の配置レベル2
	respoineIndex = 0;
	++layerLevel;
	//5-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 3;
	responeData[0][respoineIndex].initPos = { -40.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 3;
	responeData[0][respoineIndex].initPos = { -20.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 3;
	responeData[0][respoineIndex].initPos = { 0.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 3;
	responeData[0][respoineIndex].initPos = { 20.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 3;
	responeData[0][respoineIndex].initPos = { 40.0f,10.0f,z };
	++respoineIndex;
	//5-----------------------------------------------------------------

	//2-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 6;
	responeData[0][respoineIndex].initPos = { -20.0f,-10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 6;
	responeData[0][respoineIndex].initPos = { -40.0f,-10.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//2-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 8;
	responeData[0][respoineIndex].initPos = { 20.0f,-10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 8;
	responeData[0][respoineIndex].initPos = { 40.0f,-10.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------

	//3-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 11;
	responeData[0][respoineIndex].initPos = { -20.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 11;
	responeData[0][respoineIndex].initPos = { -40.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 11;
	responeData[0][respoineIndex].initPos = { -60.0f,20.0f,z };
	++respoineIndex;
	//3-----------------------------------------------------------------


	//3-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 14;
	responeData[0][respoineIndex].initPos = { 20.0f,-10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 14;
	responeData[0][respoineIndex].initPos = { 40.0f,-10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 14;
	responeData[0][respoineIndex].initPos = { 60.0f,-10.0f,z };
	++respoineIndex;
	//3-----------------------------------------------------------------


	//2-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 17;
	responeData[0][respoineIndex].initPos = { 10.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 17;
	responeData[0][respoineIndex].initPos = { -10.0f,20.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//2-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 19;
	responeData[0][respoineIndex].initPos = { 10.0f,-10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 19;
	responeData[0][respoineIndex].initPos = { -10.0f,-10.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//2-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 21;
	responeData[0][respoineIndex].initPos = { -10.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 21;
	responeData[0][respoineIndex].initPos = { -10.0f,0.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//2-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 23;
	responeData[0][respoineIndex].initPos = { 10.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 23;
	responeData[0][respoineIndex].initPos = { 10.0f,0.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------



	//4-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 26;
	responeData[0][respoineIndex].initPos = { -20.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 26;
	responeData[0][respoineIndex].initPos = { -20.0f,0.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 26;
	responeData[0][respoineIndex].initPos = { -40.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 26;
	responeData[0][respoineIndex].initPos = { -40.0f,0.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------



	//4-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 29;
	responeData[0][respoineIndex].initPos = { 20.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 29;
	responeData[0][respoineIndex].initPos = { 20.0f,0.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 29;
	responeData[0][respoineIndex].initPos = { 40.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 29;
	responeData[0][respoineIndex].initPos = { 40.0f,0.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------


	//4-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 32;
	responeData[0][respoineIndex].initPos = { -20.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 32;
	responeData[0][respoineIndex].initPos = { -20.0f,0.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 32;
	responeData[0][respoineIndex].initPos = { 20.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 32;
	responeData[0][respoineIndex].initPos = { 20.0f,0.0f,z };
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
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_K))
	{
		return SCENE_TITLE;
	}
	else
	{
		return SCENE_NONE;
	}
}
