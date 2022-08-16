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
	int respoineIndex = 0;
	int layerLevel = 0;
	int time = 60;
	float z = 300.0f;


#pragma region RezStage
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


	//左から戦艦-----------------------------------------------------------------
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].flame = time * 10;
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].generateData.initPos = { -1000.0f,-150.0f,400.0f };
	++respoineIndex;


	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].flame = time * 10;
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].generateData.initPos = { -100.0f,-150.0f,200.0f };
	++respoineIndex;
	//左から戦艦-----------------------------------------------------------------


	//もう一度左から戦艦二体-----------------------------------------------------------------
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].flame = time * 15;
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].generateData.initPos = { -1000.0f,-150.0f,100.0f };
	++respoineIndex;


	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].flame = time * 15;
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].generateData.initPos = { -100.0f,-150.0f,0.0f };
	++respoineIndex;
	//もう一度左から戦艦二体-----------------------------------------------------------------


	//正面から戦艦と通常敵-----------------------------------------------------------------
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 25;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -40.0f,20.0f,z };
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.speed = -3.0f;
	++respoineIndex;


	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 25;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -80.0f,20.0f,z };
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.speed = -3.0f;
	++respoineIndex;


	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 25;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 40.0f,20.0f,z };
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.speed = -3.0f;
	++respoineIndex;

	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 25;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 80.0f,20.0f,z };
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.speed = -3.0f;
	++respoineIndex;


	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].flame = time * 25;
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].generateData.initPos = { 40.0f,-150.0f,100.0f };
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].generateData.battleShipData.isShotFlag = true;
	++respoineIndex;


	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].flame = time * 25;
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].generateData.initPos = { -40.0f,-150.0f,100.0f };
	responeData[ENEMY_TYPE_BATTLESHIP][respoineIndex].generateData.battleShipData.isShotFlag = true;
	++respoineIndex;
	//正面から戦艦と通常敵-----------------------------------------------------------------

#pragma endregion
	++layerLevel;

#pragma region 敵の配置レベル1
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
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 40.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 9;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 60.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 9;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 80.0f,10.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------






	//4-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 12;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -20.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 12;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -40.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 12;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -60.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 12;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -80.0f,10.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------


	//4-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 15;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 20.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 15;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 40.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 15;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 60.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 15;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 80.0f,10.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------


	//4-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 18;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -20.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 18;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -40.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 18;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -60.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 18;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -80.0f,10.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------

#pragma endregion
	++layerLevel;

#pragma region 敵の配置レベル2
	
	//5-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 3;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -40.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 3;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -20.0f,10.0f,z };
	++respoineIndex;

	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 3;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 20.0f,10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 3;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 40.0f,10.0f,z };
	++respoineIndex;
	//5-----------------------------------------------------------------

	//2-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 6;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -20.0f,-10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 6;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -40.0f,-10.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//2-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_MISILE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].flame = time * 8;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].generateData.initPos = { 20.0f,-10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_MISILE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].flame = time * 8;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].generateData.initPos = { 40.0f,-10.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------

	//3-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 11;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -20.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 11;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -40.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 11;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -60.0f,20.0f,z };
	++respoineIndex;
	//3-----------------------------------------------------------------


	//3-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 14;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 20.0f,-10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_MISILE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].flame = time * 14;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].generateData.initPos = { 40.0f,-10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 14;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 60.0f,-10.0f,z };
	++respoineIndex;
	//3-----------------------------------------------------------------


	//2-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 17;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 10.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 17;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -10.0f,20.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//2-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 19;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 10.0f,-10.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_MISILE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].flame = time * 19;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].generateData.initPos = { -10.0f,-10.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//2-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 21;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -10.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 21;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -10.0f,0.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//2-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 23;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 10.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 23;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 10.0f,0.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------



	//4-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 26;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -20.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 26;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -20.0f,0.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_MISILE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].flame = time * 26;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].generateData.initPos = { -40.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_MISILE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].flame = time * 26;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].generateData.initPos = { -40.0f,0.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------



	//4-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 29;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 20.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 29;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 20.0f,0.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 29;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 40.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 29;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { 40.0f,0.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------


	//4-----------------------------------------------------------------
	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 32;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -20.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].flame = time * 32;
	responeData[ENEMY_TYPE_NORMAL][respoineIndex].generateData.initPos = { -20.0f,0.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_MISILE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].flame = time * 32;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].generateData.initPos = { 20.0f,20.0f,z };
	++respoineIndex;

	
	responeData[ENEMY_TYPE_MISILE][respoineIndex].layerLevel = layerLevel;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].flame = time * 32;
	responeData[ENEMY_TYPE_MISILE][respoineIndex].generateData.initPos = { 20.0f,0.0f,z };
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
	if (ControllerInputManager::Instance()->InputTrigger(XINPUT_GAMEPAD_START) || game->SceneChange() == 0)
	{
		return -2;
	}
	else if(ControllerInputManager::Instance()->InputTrigger(XINPUT_GAMEPAD_BACK))
	{
		return 1;
	}
	else
	{
		return SCENE_NONE;
	}
}
