#include "Game.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"
#include"../KazLibrary/Input/ControllerInputManager.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../Game/Debug/ParameterMgr.h"
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Easing/easing.h"
#include<cmath>
#include<iostream>
#include"../KazLibrary/Sound/SoundManager.h"

Game::Game() :LOG_FONT_SIZE(1.0f)
{
	emitters[0] = std::make_unique<HitEffectPattern1Emitter>();
	emitters[1] = std::make_unique<HitEffectPattern2Emitter>();
	emitters[2] = std::make_unique<HitEffectPattern3Emitter>();

	for (int emitterTypeIndex = 0; emitterTypeIndex < deadEffectEmitter.size(); ++emitterTypeIndex)
	{
		for (int stackIndex = 0; stackIndex < deadEffectEmitter[emitterTypeIndex].size(); ++stackIndex)
		{
			switch (emitterTypeIndex)
			{
			case 0:
				deadEffectEmitter[emitterTypeIndex][stackIndex] = std::make_unique<HitEffectPattern1Emitter>();
				break;
			case 1:
				deadEffectEmitter[emitterTypeIndex][stackIndex] = std::make_unique<HitEffectPattern2Emitter>();
				break;
			case 2:
				deadEffectEmitter[emitterTypeIndex][stackIndex] = std::make_unique<HitEffectPattern3Emitter>();
				break;
			default:
				break;
			}
		}
	}



	for (int i = 0; i < smokeR.size(); ++i)
	{
		smokeR[i].data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::RelativeResourcePath + "Effect/Smoke/smoke9.png");
		smokeR[i].data.pipelineName = PIPELINE_NAME_SPRITE_Z_OFF;

		smokeR[i].data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	}

	model = std::make_unique<ObjModelRender>();
	model->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
	model->data.transform.pos = { 0.0f,0.0f,20.0f };
	model->data.transform.scale = { 5.0f,5.0f,5.0f };
	model->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::TestPath + "hamster.obj");
	model->data.colorData = { 255,0,0,255 };


	lineStartPoly[0].data.transform.pos = { 0.5f,0.15f,0.0f };
	lineStartPoly[1].data.transform.pos = { -0.5f,0.0f,-0.5f };
	lineStartPoly[2].data.transform.pos = { 0.5f,0.6f,0.5f };
	lineStartPoly[3].data.transform.pos = { 0.5f,1.0f,0.0f };
	lineStartPoly[4].data.transform.pos = { 0.5f,-1.0f,-1.0f };
	lineStartPoly[5].data.transform.pos = { -0.5f,-1.0f,0.0f };
	lineStartPoly[6].data.transform.pos = { 0.5f,0.3f,-0.3f };
	lineStartPoly[7].data.transform.pos = { 0.0f,-0.3f,-1.0f };


	blackTex.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "Black.png");
	blackTex.data.pipelineName = PIPELINE_NAME_SPRITE_Z_ALWAYS;
	blackTex.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	blackTex.data.transform.scale = { 2000.0f,2000.0f };
	blackTex.data.colorData.color.a = 0;


	pressAButtonTex.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "PressAButton.png");
	pressAButtonTex.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f + 100.0f };
	pressAButtonTex.data.transform.scale = { 0.4f,0.4f };

	gameOverTex.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::GameOverPath + "GameOver.png");
	gameOverTex.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	gameOverTex.data.transform.scale = { 1.2f,1.2f };


	bgmSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "Bgm.wav");
	lockSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "Lock.wav", false);
	doneSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "Done.wav");
	damageSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "EenemyDamage.wav");

	SoundManager::Instance()->StopSoundMem(bgmSoundHandle);
	SoundManager::Instance()->PlaySoundMem(bgmSoundHandle, 1, true);

	CameraMgr::Instance()->CameraSetting(60.0f, 100000.0f, 0);
	CameraMgr::Instance()->CameraSetting(60.0f, 100000.0f, 1);


	stringLog.Init({ 50.0f,50.0f });
	mainRenderTarget.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };

	renderTarget = std::make_unique<GameRenderTarget>(KazMath::Color(0, 0, 0, 255));
	nextRenderTarget = std::make_unique<GameRenderTarget>(KazMath::Color(29, 19, 72, 255));
	meshEmitter = std::make_unique<MeshParticleEmitter>(0);
}

Game::~Game()
{
	//レンダーターゲットの破棄
	SoundManager::Instance()->StopSoundMem(bgmSoundHandle);
}

void Game::Init(const std::array<std::array<ResponeData, KazEnemyHelper::ENEMY_NUM_MAX>, KazEnemyHelper::LAYER_LEVEL_MAX> &RESPONE_DATA,
	const std::array<std::shared_ptr<IStage>, 3> &STAGE_ARRAY,
	const std::array<std::array<KazEnemyHelper::ForceCameraData, 10>, 3> &CAMERA_ARRAY)
{
	player.Init(KazMath::Transform3D().pos);
	cursor.Init();


	stageUI.Init();

	sceneNum = SCENE_NONE;

	const int NO_ENEMY_NUM = 0;
	BufferMemorySize enemySize;
	enemySize.startSize = NO_ENEMY_NUM;
	enemySize.endSize = KazEnemyHelper::ENEMY_NUM_MAX;

	BufferMemorySize eventSize;
	enemySize.startSize = KazEnemyHelper::ENEMY_NUM_MAX;
	enemySize.endSize = KazEnemyHelper::ENEMY_NUM_MAX + 50;

	responeData = RESPONE_DATA;
	cameraMoveArray = CAMERA_ARRAY;
	stages = STAGE_ARRAY;



	//操作可能OBJを纏めて生成する処理----------------------------------------------------------------
	KazEnemyHelper::GenerateEnemy(enemies, responeData, enemiesHandle);
	//敵を纏めて生成する処理----------------------------------------------------------------



	//ゲームループの初期化----------------------------------------------------------------
	gameStartFlag = false;
	cameraModeChangeFlag = false;
	gameFlame = 0;
	changeLayerLevelMaxTime[0] = KazMath::ConvertSecondToFlame(70);
	changeLayerLevelMaxTime[1] = KazMath::ConvertSecondToFlame(85);
	//ゴールに触れ無かった場合に次のステージに移動する際の最大フレーム数
	for (int i = 2; i < changeLayerLevelMaxTime.size(); ++i)
	{
		changeLayerLevelMaxTime[i] = KazMath::ConvertSecondToFlame(70);
	}
	gameStageLevel = 0;
	stageNum = gameStageLevel;
	//ゲームループの初期化----------------------------------------------------------------


	testEnemyPos = { 0.0f,0.0f,100.0f };

	appearGoalBoxPos[0] = { 0.0f,10.0f,40.0f };
	appearGoalBoxPos[1] = { 20.0f,-15.0f,40.0f };
	responeGoalBoxPos = { -10.0f,-100.0f,40.0f };
	goalBox.Init(responeGoalBoxPos);
	initAppearFlag = false;


	gameOverFlag = false;
	readyToBlackOutFlag = false;
	readyToBlackOutToGoTitleFlag = false;
	flashFlag = false;
	flashTimer = 0;


	tutorialWindow.Init();

	rocketIndex = 0;
	fireIndex = 0;
	cameraWork.Init();

	tutorial.Init(false);
	portalEffect.Init();
}

void Game::Finalize()
{
	tutorial.Finalize();
}

void Game::Input()
{

	if (tutorial.tutorialFlag)
	{
		tutorial.Input();
		//return;
	}


	KeyBoradInputManager *input = KeyBoradInputManager::Instance();
	ControllerInputManager *inputController = ControllerInputManager::Instance();

	bool upFlag = false;
	bool downFlag = false;
	bool leftFlag = false;
	bool rightFlag = false;
	bool doneFlag = false;
	bool releaseFlag = false;

	const int DEAD_ZONE = 3000;
	if (inputController->InputState(XINPUT_GAMEPAD_A))
	{
		doneFlag = true;
	}
	if (inputController->InputRelease(XINPUT_GAMEPAD_A))
	{
		releaseFlag = true;
	}
	if (inputController->InputStickState(LEFT_STICK, UP_SIDE, DEAD_ZONE))
	{
		upFlag = true;
	}
	if (inputController->InputStickState(LEFT_STICK, DOWN_SIDE, DEAD_ZONE))
	{
		downFlag = true;
	}
	if (inputController->InputStickState(LEFT_STICK, LEFT_SIDE, DEAD_ZONE))
	{
		leftFlag = true;
	}
	if (inputController->InputStickState(LEFT_STICK, RIGHT_SIDE, DEAD_ZONE))
	{
		rightFlag = true;
	}

	//ゲーム終了処理
	//三ステージ目で6秒後にタイトル画面に戻る許可を出す
	if (inputController->InputTrigger(XINPUT_GAMEPAD_A) && 2 <= gameStageLevel)
	{
		sceneNum = 0;
	}

	//ゲームオーバー画面でタイトル画面に戻る処理
	if (inputController->InputTrigger(XINPUT_GAMEPAD_A) && gameOverFlag)
	{
		readyToBlackOutToGoTitleFlag = true;
	}


	if (input->InputTrigger(DIK_O))
	{
		sceneNum = 0;
	}


	KazMath::Vec2<float> joyStick;
	joyStick.x = inputController->GetJoyStickLXNum(0) / 32767.0f;
	joyStick.y = inputController->GetJoyStickLYNum(0) / 32767.0f;

	cursor.Input
	(
		upFlag,
		downFlag,
		leftFlag,
		rightFlag,
		doneFlag,
		releaseFlag,
		joyStick
	);


	if (input->InputTrigger(DIK_G))
	{
		emittNum = 2;
		emitters[emittNum]->Init(KazMath::Vec2<float>(WIN_X / 2.0f, WIN_Y / 2.0f));
	}
	if (input->InputTrigger(DIK_H))
	{
		emittNum = KazMath::Rand<int>(3, 0);
		emitters[emittNum]->Init(KazMath::Vec2<float>(WIN_X / 2.0f, WIN_Y / 2.0f));
	}

	if (input->InputTrigger(DIK_SPACE))
	{
		switch (KazMath::Rand(0, 4))
		{
		case 0:
			stringLog.WriteLog("Abc Def", 1.0f);
			break;
		case 1:
			stringLog.WriteLog("Gh f56", 1.0f);
			break;
		case 2:
			stringLog.WriteLog("A bc1234Def", 1.0f);
			break;
		case 3:
			stringLog.WriteLog("Abc0789D ef", 1.0f);
			break;
		case 4:
			stringLog.WriteLog("Ab cD 5904ef", 1.0f);
			break;
		default:
			break;
		}
	}

}

void Game::Update()
{
	tutorial.handle = renderTarget->GetGameRenderTargetHandle();
	if (tutorial.tutorialFlag)
	{
		tutorial.Update();
		stages[0]->Update();
		cursor.Update();
		cameraWork.Update(cursor.GetValue(), &player.pos, cameraModeChangeFlag);
		player.Update();
		return;
	}
	stages[0]->startFlag = true;

	for (int i = 0; i < lineStartPoly.size(); ++i)
	{
		lineStartPoly[i].data.pipelineName = PIPELINE_NAME_COLOR_MULTITEX;
		lineStartPoly[i].data.transform.scale = { 0.1f,0.1f,0.1f };
		lineStartPoly[i].data.color = { 255,0,0,255 };
		lineEffectArrayData[i].startPos = lineStartPoly[i].data.transform.pos + player.pos;
	}
	{
		int lineArrayNum = static_cast<int>(lineStartPoly.size());
		for (int i = lineArrayNum; i < lineArrayNum * 2; ++i)
		{
			lineEffectArrayData[i].startPos = lineStartPoly[i - lineArrayNum].data.transform.pos + player.pos;
		}
	}


	for (int i = 0; i < cameraMoveArray[stageNum].size(); ++i)
	{
		if (cameraMoveArray[stageNum][i].flame <= gameFlame && cameraMoveArray[stageNum][i].flame != -1)
		{
			cameraWork.ForceCamera(cameraMoveArray[stageNum][i].dir);
		}
	}

	cameraWork.Update(cursor.GetValue(), &player.pos, cameraModeChangeFlag);
	eyePos = cameraWork.GetEyePos();
	targetPos = cameraWork.GetTargetPos();
	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f }, 0);



	//敵が一通り生成終わった際に登場させる----------------------------------------------------------------
	if (changeLayerLevelMaxTime[gameStageLevel] <= gameFlame && !initAppearFlag)
		//if (100 <= gameFlame && !initAppearFlag)
	{
		goalBox.Appear(appearGoalBoxPos[stageNum]);
		initAppearFlag = true;
	}
	//敵が一通り生成終わった際に登場させる----------------------------------------------------------------

	changeStageFlag = false;

	portalEffect.Update(player.pos);
	//全部隠れたら次ステージの描画をする
	if (portalEffect.IsFinish())
	{
		//ゲームループの初期化----------------------------------------------
		++gameStageLevel;
		gameFlame = 0;
		//ゲームループの初期化----------------------------------------------

		//ステージの切り替え----------------------------------------------
		++stageNum;
		changeStageFlag = true;
		//ステージの切り替え----------------------------------------------

		//ゴールボックスの初期化----------------------------------------------
		initAppearFlag = false;
		goalBox.Init(responeGoalBoxPos);
		//ゴールボックスの初期化----------------------------------------------

		stages[stageNum]->startFlag = true;

		rocketIndex = 0;
		fireIndex = 0;

		stageUI.Init();
		stageUI.AnnounceStage(stageNum + 1);

		portalEffect.Finalize();
	}


	//ポータル演出開始
	if (goalBox.startPortalEffectFlag && !portalEffect.IsStart())
	{
		portalEffect.Init();
		portalEffect.Start();
	}

	//入った後の初期化
	if (portalEffect.disappearFlag)
	{
		//portal.Init(KazMath::Vec3<float>(0.0f, 3.0f, 50.0f));
		goalBox.startPortalEffectFlag = false;
	}



#pragma region 生成処理
	//敵を追加で初期化する処理----------------------------------------------------------------
	KazEnemyHelper::AddEnemy(enemies, responeData, addEnemiesHandle, gameFlame, gameStageLevel);
	//敵を追加で初期化する処理----------------------------------------------------------------


	//敵をどのタイミングで初期化する処理----------------------------------------------------------------
	for (int enemyType = 0; enemyType < responeData.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < responeData[enemyType].size(); ++enemyCount)
		{
			bool enableToUseThisDataFlag = responeData[enemyType][enemyCount].layerLevel != -1;
			bool readyToStartFlag = responeData[enemyType][enemyCount].flame <= gameFlame &&
				responeData[enemyType][enemyCount].layerLevel == gameStageLevel;

			if (enableToUseThisDataFlag && readyToStartFlag && enemies[enemyType][enemyCount] != nullptr && !enemies[enemyType][enemyCount]->GetData()->oprationObjData->initFlag)
			{
				enemies[enemyType][enemyCount]->Init(responeData[enemyType][enemyCount].generateData, false);

				switch (enemyType)
				{
				case ENEMY_TYPE_NORMAL:
					lightEffect[rocketIndex].Init(enemies[enemyType][enemyCount]->GetData()->hitBox.center, KazMath::Vec3<float>(0.0f, 0.0f, 20.0f), true, &enemies[enemyType][enemyCount]->GetData()->oprationObjData->enableToHitFlag, &enemies[enemyType][enemyCount]->GetData()->radius, &enemies[enemyType][enemyCount]->GetData()->startFlag);
					fireEffect[fireIndex].Init(enemies[enemyType][enemyCount]->GetData()->hitBox.center, KazMath::Vec3<float>(0.0f, 0.0f, 45.0f), true, &enemies[enemyType][enemyCount]->GetData()->oprationObjData->enableToHitFlag);
					++rocketIndex;
					++fireIndex;
					break;

				case ENEMY_TYPE_MISILE:
					lightEffect[rocketIndex].Init(enemies[enemyType][enemyCount]->GetData()->hitBox.center, KazMath::Vec3<float>(0.0f, 0.0f, 10.0f), true, &enemies[enemyType][enemyCount]->GetData()->oprationObjData->enableToHitFlag, &enemies[enemyType][enemyCount]->GetData()->radius, &enemies[enemyType][enemyCount]->GetData()->startFlag);
					fireEffect[rocketIndex].Init(enemies[enemyType][enemyCount]->GetData()->hitBox.center, KazMath::Vec3<float>(0.0f, 0.0f, 35.0f), true, &enemies[enemyType][enemyCount]->GetData()->oprationObjData->enableToHitFlag);
					++rocketIndex;
					++fireIndex;
					break;

				case ENEMY_TYPE_BATTLESHIP_MISILE:
					lightEffect[rocketIndex].Init(enemies[enemyType][enemyCount]->GetData()->hitBox.center, KazMath::Vec3<float>(0.0f, 0.0f, 0.0f), false, &enemies[enemyType][enemyCount]->GetData()->oprationObjData->enableToHitFlag, &enemies[enemyType][enemyCount]->GetData()->radius, &enemies[enemyType][enemyCount]->GetData()->startFlag);
					++rocketIndex;
					break;

				case ENEMY_TYPE_BIKE_MISILE:
					lightEffect[rocketIndex].Init(enemies[enemyType][enemyCount]->GetData()->hitBox.center, KazMath::Vec3<float>(0.0f, 0.0f, 0.0f), false, &enemies[enemyType][enemyCount]->GetData()->oprationObjData->enableToHitFlag, &enemies[enemyType][enemyCount]->GetData()->radius, &enemies[enemyType][enemyCount]->GetData()->startFlag);
					++rocketIndex;
					break;
				default:
					break;
				}

#ifdef _DEBUG
				const float lScale = enemies[enemyType][enemyCount]->GetData()->hitBox.radius;
				enemyHitBox[enemyType][enemyCount].data.transform.scale = { lScale ,lScale ,lScale };
				enemyHitBox[enemyType][enemyCount].data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
#endif
			}
		}
	}
	//敵をどのタイミングで初期化する処理----------------------------------------------------------------
#pragma endregion


#pragma region 敵ロックオン
	for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
		{
			//生成されている、初期化している敵のみ更新処理を通す
			bool enableToUseDataFlag = enemies[enemyType][enemyCount] != nullptr && enemies[enemyType][enemyCount]->GetData()->oprationObjData->initFlag;
			if (enableToUseDataFlag)
			{
				//敵のデータのポインタを代入
				EnemyData *enemyData = enemies[enemyType][enemyCount]->GetData().get();

				//ロックオン判定
				bool enableToLockOnNumFlag = cursor.LockOn();
				bool enableToLockOnEnemyFlag = enemies[enemyType][enemyCount]->IsAlive() && !enemies[enemyType][enemyCount]->LockedOrNot();
				bool hitFlag = CollisionManager::Instance()->CheckRayAndSphere(cursor.hitBox, enemyData->hitBox);
				if (hitFlag &&
					enableToLockOnNumFlag &&
					enableToLockOnEnemyFlag &&
					!cursor.releaseFlag)
				{
					SoundManager::Instance()->PlaySoundMem(lockSoundHandle, 1);

					//カーソルのカウント数を増やす
					cursor.Hit(enemyData->hitBox.center);
					//敵が当たった情報を書く
					enemies[enemyType][enemyCount]->Hit();

					stringLog.WriteLog(enemies[enemyType][enemyCount]->GetData()->oprationObjData->name, LOG_FONT_SIZE);

					//線演出をかける際にどの配列を使用するか決める
					for (int i = 0; i < lineEffectArrayData.size(); ++i)
					{
						if (!lineEffectArrayData[i].usedFlag)
						{
							lineLevel[i].Attack(lineEffectArrayData[i].startPos, *enemyData->hitBox.center);
							lineEffectArrayData[i].usedFlag = true;
							lineEffectArrayData[i].lineIndex = i;
							lineEffectArrayData[i].enemyTypeIndex = enemyType;
							lineEffectArrayData[i].enemyIndex = enemyCount;
							break;
						}
					}
				}
			}
		}
	}


#pragma region イベントロックオン
	{
		//ロックオン判定
		bool enableToLockOnNumFlag = cursor.LockOn();
		bool enableToLockOnEnemyFlag = goalBox.IsAlive() && !goalBox.LockedOrNot();
		bool hitFlag = CollisionManager::Instance()->CheckRayAndSphere(cursor.hitBox, goalBox.hitBox);
		if (hitFlag &&
			enableToLockOnNumFlag &&
			enableToLockOnEnemyFlag)
		{
			SoundManager::Instance()->PlaySoundMem(lockSoundHandle, 1);

			//カーソルのカウント数を増やす
			cursor.Hit(goalBox.hitBox.center);
			goalBox.Hit();

			//線演出をかける際にどの配列を使用するか決める
			for (int i = 0; i < lineEffectArrayData.size(); ++i)
			{
				if (!lineEffectArrayData[i].usedFlag)
				{
					lineLevel[i].Attack(lineEffectArrayData[i].startPos, *goalBox.hitBox.center);
					lineEffectArrayData[i].usedFlag = true;
					lineEffectArrayData[i].lineIndex = i;
					lineEffectArrayData[i].eventType = 0;
					break;
				}
			}
		}
	}

#pragma endregion


	for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
		{
			//生成されている、初期化している敵のみ更新処理を通す
			bool enableToUseDataFlag = enemies[enemyType][enemyCount] != nullptr && enemies[enemyType][enemyCount]->GetData()->oprationObjData->initFlag;
			if (enableToUseDataFlag)
			{
				//敵のデータのポインタを代入
				EnemyData *enemyData = enemies[enemyType][enemyCount]->GetData().get();

				for (int i = 0; i < lineEffectArrayData.size(); ++i)
				{
					bool sameEnemyFlag = lineEffectArrayData[i].enemyTypeIndex == enemyType && lineEffectArrayData[i].enemyIndex == enemyCount;
					if (lineEffectArrayData[i].usedFlag && sameEnemyFlag)
					{
						enemyData->oprationObjData->lockOnFlag = true;
						break;
					}
					else
					{
						enemyData->oprationObjData->lockOnFlag = false;
					}
				}
			}
		}
	}

	for (int i = 0; i < lineEffectArrayData.size(); ++i)
	{
		bool sameEventFlag = lineEffectArrayData[i].eventType == 0 && lineEffectArrayData[i].lineIndex == i;
		if (lineEffectArrayData[i].usedFlag && sameEventFlag)
		{
			goalBox.iOperationData.lockOnFlag = true;
			break;
		}
		else
		{
			goalBox.iOperationData.lockOnFlag = false;
		}
	}





	for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
		{
			//生成されている、初期化している敵のみ更新処理を通す
			bool enableToUseDataFlag = enemies[enemyType][enemyCount] != nullptr && enemies[enemyType][enemyCount]->GetData()->oprationObjData->initFlag;
			if (enableToUseDataFlag)
			{
				//敵のデータのポインタを代入
				EnemyData *enemyData = enemies[enemyType][enemyCount]->GetData().get();

				for (int i = 0; i < lineEffectArrayData.size(); ++i)
				{
					bool sameEnemyFlag = lineEffectArrayData[i].enemyTypeIndex == enemyType && lineEffectArrayData[i].enemyIndex == enemyCount;
					bool releaseFlag = enemyData->outOfStageFlag || enemyData->timer <= 0;

					if (lineEffectArrayData[i].usedFlag && sameEnemyFlag && releaseFlag)
					{
						int lineIndex = lineEffectArrayData[i].lineIndex;
						lineLevel[lineIndex].Release();
						cursor.SubCount(1);
						break;
					}
				}
			}
		}
	}



#pragma region 線のロックオン解放<

	//線がたどり着いたら敵を死亡させる
	for (int i = 0; i < lineEffectArrayData.size(); ++i)
	{
		if (lineEffectArrayData[i].usedFlag)
		{
			int lineIndex = lineEffectArrayData[i].lineIndex;
			int enemyTypeIndex = lineEffectArrayData[i].enemyTypeIndex;
			int enemyIndex = lineEffectArrayData[i].enemyIndex;
			int eventIndex = lineEffectArrayData[i].eventType;

			if (eventIndex != -1)
			{
				break;
			}

			//演出を合わせてダメージと死亡をやる
			if (lineLevel[lineIndex].lineReachObjFlag && !lineLevel[lineIndex].alreadyReachedFlag && enemies[enemyTypeIndex][enemyIndex]->IsAlive())
			{
				SoundManager::Instance()->PlaySoundMem(damageSoundHandle, 1);
				lineLevel[lineIndex].alreadyReachedFlag = true;
			}
			else if (lineLevel[lineIndex].lineReachObjFlag && !enemies[enemyTypeIndex][enemyIndex]->IsAlive() && !lineEffectArrayData[i].hitFlag)
			{
				enemies[enemyTypeIndex][enemyIndex]->Dead();
				lineEffectArrayData[i].hitFlag = true;
				//lineEffectArrayData[i].Reset();
				//lineLevel[lineIndex].lineReachObjFlag = false;

				//ヒット時の円演出
				for (int hitEffectIndex = 0; hitEffectIndex < hitEffect.size(); ++hitEffectIndex)
				{
					if (!hitEffect[hitEffectIndex].IsAlive())
					{
						hitEffect[hitEffectIndex].Start(*enemies[enemyTypeIndex][enemyIndex]->GetData()->hitBox.center);
						break;
					}
				}

				//死亡時の煙演出
				int lEmitterType = KazMath::Rand<int>(3, 0);
				for (int stackIndex = 0; stackIndex < deadEffectEmitter[lEmitterType].size(); ++stackIndex)
				{
					if (!deadEffectEmitter[lEmitterType][stackIndex]->IsActive())
					{
						KazMath::Vec3<float>screenPos =
							KazMath::ConvertWorldPosToScreenPos(*enemies[enemyTypeIndex][enemyIndex]->GetData()->hitBox.center, CameraMgr::Instance()->GetViewMatrix(0), CameraMgr::Instance()->GetPerspectiveMatProjection());
						//敵が画面外に出た際に画面横からウィンドウが出ないように変換後のz値が1.0以上は描画しない
						if (screenPos.z <= 1.0f)
						{
							deadEffectEmitter[lEmitterType][stackIndex]->Init(KazMath::Vec2<float>(screenPos.x, screenPos.y));
						}
						break;
					}
				}

			}
		}
	}

	//線演出が終わった際にリンクのデータをリセットする
	for (int i = 0; i < lineEffectArrayData.size(); ++i)
	{
		if (lineEffectArrayData[i].usedFlag)
		{
			int lineIndex = lineEffectArrayData[i].lineIndex;
			if (lineLevel[lineIndex].allFinishFlag)
			{
				lineEffectArrayData[i].Reset();
			}
		}
	}

	if (cursor.Release() && cursor.GetCount() != 0)
	{
		const std::string lLog("Fire_[" + std::to_string(cursor.GetCount()) + "x]");
		stringLog.WriteLog(lLog, LOG_FONT_SIZE);
	}

#pragma endregion


#pragma endregion


	//攻撃----------------------------------------------
	for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
		{
			//生成されている、初期化している敵のみ更新処理を通す
			bool enableToUseDataFlag = enemies[enemyType][enemyCount] != nullptr && enemies[enemyType][enemyCount]->GetData()->oprationObjData->enableToHitFlag;
			if (enableToUseDataFlag)
			{
				//時間が0秒以下ならプレイヤーに当たったと判断する
				if (enemies[enemyType][enemyCount]->GetData()->timer <= 0)
				{
					player.Hit();
					//enemies[enemyType][enemyCount]->Dead();
				}
			}
		}
	}
	//攻撃----------------------------------------------


	//ゲームオーバー----------------------------------------------
	if (!player.isAlive() && !gameOverFlag)
	{
		readyToBlackOutFlag = true;
	}

	if (readyToBlackOutFlag || readyToBlackOutToGoTitleFlag)
	{
		//暗転
		if (blackTex.data.colorData.color.a <= 255.0f)
		{
			blackTex.data.colorData.color.a += 5;
		}
		//タイトル画面に戻る
		else if (readyToBlackOutToGoTitleFlag && 255 <= blackTex.data.colorData.color.a)
		{
			sceneNum = 0;
		}
		//ゲームオーバー画面を表示
		else
		{
			gameOverFlag = true;
			readyToBlackOutFlag = false;
		}
	}
	else
	{
		//明転
		if (0 <= blackTex.data.colorData.color.a)
		{
			blackTex.data.colorData.color.a -= 5;
		}
	}
	if (gameOverFlag)
	{
		++flashTimer;

		if (60 <= flashTimer)
		{
			flashFlag = !flashFlag;
			flashTimer = 0;
		}
	}
	//ゲームオーバー----------------------------------------------


	if (!gameOverFlag)
	{
#pragma region 更新処理
		goalBox.releaseFlag = cursor.releaseFlag;

		//更新処理----------------------------------------------------------------
		player.Update();
		cursor.Update();
		goalBox.Update();
		stageUI.Update();
		stages[stageNum]->Update();
		tutorialWindow.Update();

		for (int i = 0; i < hitEffect.size(); ++i)
		{
			hitEffect[i].Update();
		}


		//配列外を超えない処理
		if (stageNum + 1 < stages.size())
		{
			//	stages[stageNum + 1]->Update();
		}
		//ロックオンのリリース処理
		if (cursor.releaseFlag)
		{
			for (int i = 0; i < lineEffectArrayData.size(); ++i)
			{
				if (lineEffectArrayData[i].usedFlag)
				{
					int lineIndex = lineEffectArrayData[i].lineIndex;
					lineLevel[lineIndex].ReleaseShot();
				}
			}
		}

		//ロックオン中に必要なデータ
		for (int i = 0; i < lineEffectArrayData.size(); ++i)
		{
			if (lineEffectArrayData[i].usedFlag)
			{
				int lineIndex = lineEffectArrayData[i].lineIndex;
				//イベントブロックかどうか判断する
				if (lineEffectArrayData[i].eventType != -1)
				{
					KazMath::Vec3<float> pos = *goalBox.hitBox.center;
					lineLevel[lineIndex].CalucurateDistance(player.pos, pos);
				}
				else
				{
					int enemyTypeIndex = lineEffectArrayData[i].enemyTypeIndex;
					int enemyIndex = lineEffectArrayData[i].enemyIndex;
					KazMath::Vec3<float> pos = *enemies[enemyTypeIndex][enemyIndex]->GetData()->hitBox.center;
					lineLevel[lineIndex].CalucurateDistance(player.pos, pos);
				}
			}
		}


		for (int i = 0; i < lineLevel.size(); ++i)
		{
			if (lineEffectArrayData[i].usedFlag)
			{
				lineLevel[i].playerPos = lineEffectArrayData[i].startPos;
			}

			if (!lineLevel[i].initFlag)
			{
				lineEffectArrayData[i].Reset();
			}

			lineLevel[i].Update();
		}


		isEnemyNotMoveFlag = true;
		//敵の更新処理
		for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
		{
			for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
			{
				//生成されている、初期化している敵のみ更新処理を通す
				bool enableToUseDataFlag =
					enemies[enemyType][enemyCount] != nullptr &&
					enemies[enemyType][enemyCount]->GetData()->oprationObjData->initFlag &&
					!enemies[enemyType][enemyCount]->GetData()->outOfStageFlag;
				if (enableToUseDataFlag)
				{
					enemies[enemyType][enemyCount]->Update();
#ifdef _DEBUG
					enemyHitBox[enemyType][enemyCount].data.transform.pos = *enemies[enemyType][enemyCount]->GetData()->hitBox.center;
#endif
				}

				//一体でも敵が動いていたらそれを知らせるフラグを上げる
				if (enableToUseDataFlag && enemies[enemyType][enemyCount]->GetData()->oprationObjData->enableToHitFlag)
				{
					isEnemyNotMoveFlag = false;
				}
			}
		}
		//更新処理----------------------------------------------------------------


		for (int i = 0; i < fireEffect.size(); ++i)
		{
			fireEffect[i].Update();
		}
		for (int i = 0; i < lightEffect.size(); ++i)
		{
			lightEffect[i].Update();
		}
#pragma endregion


		emitters[emittNum]->Update();

		for (int emitterTypeIndex = 0; emitterTypeIndex < deadEffectEmitter.size(); ++emitterTypeIndex)
		{
			for (int stackIndex = 0; stackIndex < deadEffectEmitter[emitterTypeIndex].size(); ++stackIndex)
			{
				deadEffectEmitter[emitterTypeIndex][stackIndex]->Update();
			}
		}

		stringLog.Update();
	}


	//ゲームループの経過時間----------------------------------------------------------------

	//ゲーム開始
	if (!tutorial.tutorialFlag && !gameStartFlag)
	{
		//ステージ番号知らせる
		stageUI.Init();
		stageUI.AnnounceStage(stageNum + 1);
		gameStartFlag = true;
	}


	if (gameStartFlag)
	{
		if (isEnemyNotMoveFlag)
		{
			++notMoveTimer;
		}
		else
		{
			notMoveTimer = 0;
		}

		//敵が何もしていない時間が一定時間を超えたらゲーム内時間を早める
		if (KazMath::ConvertSecondToFlame(CHANGE_GMAE_FLAME_SPEED_MAX_TIME) <= notMoveTimer)
		{
			gameSpeed = 60;
		}
		else
		{
			gameSpeed = 1;
		}

		gameFlame += gameSpeed;
	}

	if (60 * 4 <= gameFlame)
	{
		//チュートリアル用のUI非表示
		tutorialWindow.End();
	}

	//ゲームループの経過時間----------------------------------------------------------------
	if (meshEmitter->resetSceneFlag)
	{
		int lNum = meshEmitter->enemyIndex;
		meshEmitter.reset();
		meshEmitter = std::make_unique<MeshParticleEmitter>(lNum);
	}

	meshEmitter->Update();
}

void Game::Draw()
{
	if (tutorial.tutorialFlag)
	{
		tutorial.Draw();
	}
	else
	{
		RenderTargetStatus::Instance()->SetDoubleBufferFlame();
		RenderTargetStatus::Instance()->ClearDoubuleBuffer(BG_COLOR);
	}
	//中間演出の描画
	portalEffect.Draw(&cursor);

	//次のポータルの描画
	if (portalEffect.DrawNextPortal())
	{
		int lStageNum = stageNum + 1;
		if (lStageNum < 0)
		{
			lStageNum = 1;
		}
		else if (stages.size() <= lStageNum)
		{
			lStageNum = static_cast<int>(stages.size() - 1);
		}

		nextRenderTarget->SetRenderTarget();
		CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f }, 1);
		player.Draw();
		stages[lStageNum]->SetCamera(1);
		stages[lStageNum]->Draw();
		nextRenderTarget->Draw();
	}

	if (!gameOverFlag)
	{
		renderTarget->SetRenderTarget();
		if (lineDebugFlag)
		{
			bg.Draw();
		}


		if (changeLayerLevelMaxTime[gameStageLevel] <= gameFlame)
		{
			goalBox.Draw();
		}


		stages[stageNum]->SetCamera(0);
		stages[stageNum]->Draw();

		player.Draw();


		PIXBeginEvent(DirectX12CmdList::Instance()->cmdList.Get(), 0, "Enemy");
		//敵の描画処理----------------------------------------------------------------
		for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
		{
			for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
			{
				//生成されている敵のみ描画処理を通す
				bool enableToUseDataFlag = enemies[enemyType][enemyCount] != nullptr &&
					enemies[enemyType][enemyCount]->GetData()->oprationObjData->initFlag &&
					!enemies[enemyType][enemyCount]->GetData()->outOfStageFlag;
				if (enableToUseDataFlag)
				{
					enemies[enemyType][enemyCount]->Draw();
				}
#ifdef _DEBUG
				if (enableToUseDataFlag && enemies[enemyType][enemyCount]->iOperationData.enableToHitFlag)
				{
					//デバック時のみ当たり判定を描画
					enemyHitBox[enemyType][enemyCount].Draw();
				}
#endif
			}
		}
		PIXEndEvent(DirectX12CmdList::Instance()->cmdList.Get());


		for (int i = 0; i < fireEffect.size(); ++i)
		{
			fireEffect[i].Draw();
		}

		for (int i = 0; i < lightEffect.size(); ++i)
		{
			lightEffect[i].Draw();
		}


		stages[stageNum]->vaporWaveSunRender.Draw();


		for (int i = 0; i < hitEffect.size(); ++i)
		{
			hitEffect[i].Draw();
		}

		for (int i = 0; i < lineLevel.size(); ++i)
		{
			lineLevel[i].Draw();
		}


		if (changeLayerLevelMaxTime[gameStageLevel] <= gameFlame)
		{
			goalBox.lightEffect.Draw();
		}


		//次ポータルの描画
		if (portalEffect.DrawNextPortal())
		{
			portalEffect.nextPortalRender.data.handleData = nextRenderTarget->GetGameRenderTargetHandle();
			portalEffect.nextPortalRender.Draw();
		}
		//中間演出までのポータル
		if (portalEffect.IsStart() && !portalEffect.IsFinish())
		{
			portalEffect.portalRender.Draw();
		}


		renderTarget->Draw();


		//現在の描画
		mainRenderTarget.data.handleData = renderTarget->GetGameRenderTargetHandle();

		//チュートリアル用の描画
		if (tutorial.tutorialFlag)
		{
			mainRenderTarget.data.handleData = tutorial.mainRenderTarget.data.handleData;
		}
		//中間演出の描画
		if (portalEffect.disappearFlag || portalEffect.DrawNextPortal())
		{
			mainRenderTarget.data.handleData = portalEffect.renderTarget->GetGameRenderTargetHandle();
		}



		stringLog.Draw();
		stageUI.Draw();

		mainRenderTarget.Draw();
		cursor.Draw();
	}
	else
	{
		gameOverTex.Draw();
		if (flashFlag)
		{
			pressAButtonTex.Draw();
		}
	}

	//blackTex.Draw();

}

int Game::SceneChange()
{
	if (sceneNum != -1) {
		int tmp = sceneNum;
		sceneNum = -1;
		return tmp;
	}

	return SCENE_NONE;
}