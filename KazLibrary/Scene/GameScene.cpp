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

}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	int respoineIndex = 0;
	int layerLevel = 0;
	int time = 60;
	float z = 0.0f;

#pragma region RezStage
	z = 800.0f;
	stages[layerLevel] = std::make_shared<RezStage>();

	cameraMoveArray[layerLevel][0].flame = KazMath::ConvertSecondToFlame(30);
	cameraMoveArray[layerLevel][0].dir = KazEnemyHelper::CAMERA_LEFT;
	cameraMoveArray[layerLevel][1].flame = KazMath::ConvertSecondToFlame(50);
	cameraMoveArray[layerLevel][1].dir = KazEnemyHelper::CAMERA_FRONT;

	//正面から通常敵四体-----------------------------------------------------------------
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 3;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -80.0f,10.0f,z };
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.speed = -3.0f;
	++respoineIndex;


	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 3;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -40.0f,10.0f,z };
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.speed = -3.0f;
	++respoineIndex;

	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 3;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 40.0f,10.0f,z };
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.speed = -3.0f;
	++respoineIndex;


	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 3;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 80.0f,10.0f,z };
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.speed = -3.0f;
	++respoineIndex;
	//正面から通常敵四体-----------------------------------------------------------------


	//ミサイル敵-----------------------------------------------------------------
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 10;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -40.0f,-20.0f,z };
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.speed = -3.0f;
	++respoineIndex;

	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 10;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 40.0f,-20.0f,z };
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.speed = -3.0f;
	++respoineIndex;


	responeData[ENEMY_TYPE_MISILE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].flame = time * 12;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].generateData.initPos = { -80.0f,-20.0f,400.0f };
	responeData[ENEMY_TYPE_MISILE][respoineIndex].generateData.speed = -3.0f;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].generateData.misileEnemy.isShotFlag = true;
	++respoineIndex;

	responeData[ENEMY_TYPE_MISILE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].flame = time * 12;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].generateData.initPos = { -40.0f,10.0f,400.0f };
	responeData[ENEMY_TYPE_MISILE][respoineIndex].generateData.speed = -3.0f;
	++respoineIndex;

	responeData[ENEMY_TYPE_MISILE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].flame = time * 12;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].generateData.initPos = { 40.0f,10.0f,400.0f };
	responeData[ENEMY_TYPE_MISILE][respoineIndex].generateData.speed = -3.0f;
	++respoineIndex;

	responeData[ENEMY_TYPE_MISILE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].flame = time * 12;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].generateData.initPos = { 80.0f,-20.0f,400.0f };
	responeData[ENEMY_TYPE_MISILE][respoineIndex].generateData.speed = -3.0f;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].generateData.misileEnemy.isShotFlag = true;
	++respoineIndex;
	//ミサイル敵--------------------------------------

	//左から戦艦-----------------------------------------------------------------
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].flame = time * 30;
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].generateData.initPos = { -1000.0f,-150.0f,400.0f };
	++respoineIndex;


	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].flame = time * 30;
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].generateData.initPos = { -100.0f,-150.0f,200.0f };
	++respoineIndex;
	//左から戦艦-----------------------------------------------------------------


	//バイク敵--------------------------------------
	responeData[ENEMY_TYPE_BIKE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_BIKE][respoineIndex].flame = time * 34;
	responeData[ENEMY_TYPE_BIKE][respoineIndex].generateData.initPos = { -200.0f,50.0f,50.0f };
	++respoineIndex;

	responeData[ENEMY_TYPE_BIKE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_BIKE][respoineIndex].flame = time * 34;
	responeData[ENEMY_TYPE_BIKE][respoineIndex].generateData.initPos = { -100.0f,0.0f,100.0f };
	++respoineIndex;
	//バイク敵--------------------------------------


	//召喚敵
	responeData[ENEMY_TYPE_MOTHER][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MOTHER][respoineIndex].flame = time * 50;
	responeData[ENEMY_TYPE_MOTHER][respoineIndex].generateData.initPos = { 60.0f,50.0f,300.0f };
	++respoineIndex;
	//召喚敵
	responeData[ENEMY_TYPE_MOTHER][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MOTHER][respoineIndex].flame = time * 50;
	responeData[ENEMY_TYPE_MOTHER][respoineIndex].generateData.initPos = { -60.0f,50.0f,300.0f };
	++respoineIndex;


	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].flame = time * 70;
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].generateData.initPos = { 0.0f,-150.0f,100.0f };
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].generateData.battleShipData.isShotFlag = true;
	++respoineIndex;
	//正面から戦艦と通常敵-----------------------------------------------------------------

#pragma endregion
	++layerLevel;

#pragma region 敵の配置レベル1
	z = 450.0f;
	stages[layerLevel] = std::make_shared<FirstStage>();

	//2-----------------------------------------------------------------
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 3;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 20.0f,10.0f,z };
	++respoineIndex;

	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 3;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 60.0f,10.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//respoineIndex = 0;
	//2-----------------------------------------------------------------
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 6;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -20.0f,10.0f,z };
	++respoineIndex;


	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 6;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -60.0f,10.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//4-----------------------------------------------------------------

	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 9;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 20.0f,10.0f,z };
	++respoineIndex;


	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 9;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 60.0f,10.0f,z };
	++respoineIndex;


	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 9;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 100.0f,10.0f,z };
	++respoineIndex;


	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 9;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 140.0f,10.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------






	//4-----------------------------------------------------------------

	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 12;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -20.0f,10.0f,z };
	++respoineIndex;


	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 12;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -60.0f,10.0f,z };
	++respoineIndex;


	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 12;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -100.0f,10.0f,z };
	++respoineIndex;


	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 12;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -140.0f,10.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------


	//4-----------------------------------------------------------------

	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 15;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 20.0f,10.0f,z };
	++respoineIndex;


	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 15;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 60.0f,10.0f,z };
	++respoineIndex;


	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 15;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 100.0f,10.0f,z };
	++respoineIndex;


	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 15;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 140.0f,10.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------


	//4-----------------------------------------------------------------

	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 18;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -20.0f,10.0f,z };
	++respoineIndex;


	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 18;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -60.0f,10.0f,z };
	++respoineIndex;


	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 18;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -100.0f,10.0f,z };
	++respoineIndex;


	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 18;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -140.0f,10.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------

#pragma endregion
	++layerLevel;

	stages[layerLevel] = std::make_shared<ThridStage>();


	game = std::make_unique<Game>();
	game->Init(responeData, stages, cameraMoveArray);
}

void GameScene::Finalize()
{
	game.reset();
	for (int i = 0; i < stages.size(); ++i)
	{
		stages[i].reset();
	}
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
	if (ControllerInputManager::Instance()->InputTrigger(XINPUT_GAMEPAD_START) || game->SceneChange() == 0)
	{
		return -2;
	}
	else if (ControllerInputManager::Instance()->InputTrigger(XINPUT_GAMEPAD_BACK))
	{
		return 1;
	}
	else
	{
		return SCENE_NONE;
	}
}
