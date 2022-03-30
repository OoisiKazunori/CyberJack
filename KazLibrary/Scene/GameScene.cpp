#include "GameScene.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Imgui/MyImgui.h"

GameScene::GameScene()
{
	besidePoly = std::make_unique<BoxPolygonRender>();
	verticlaPoly = std::make_unique<BoxPolygonRender>();
	cameraPoly = std::make_unique<BoxPolygonRender>();
	testEnemyPoly = std::make_unique<BoxPolygonRender>();
	testEnemyPoly->data.color = { 255.0f,255.0f,255.0f,255.0f };
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	player.Init(KazMath::Transform3D().pos);
	cursor.Init();

	hitBox.Init({ 0.0f,0.0f,30.0f }, 1);

	sceneNum = SCENE_NONE;


	responeData[0][0].enemyType = 0;
	responeData[0][0].layerLevel = 0;
	responeData[0][0].flame = 360;
	responeData[0][0].initPos = { 0.0f,10.0f,20.0f };

	//敵を纏めて生成する処理----------------------------------------------------------------
	for (int enemyType = 0; enemyType < responeData.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < responeData[enemyType].size(); ++enemyCount)
		{
			if (responeData[enemyType][enemyCount].enemyType != -1)
			{
				switch (enemyType)
				{
				case 0:
					enemies[enemyType][enemyCount] = std::make_unique<NormalEnemy>();
					//子敵の生成(テスト用)
					for (int i = 0; i < 8; ++i)
					{
						int index = enemiesHandle[ENEMY_TYPE_KID];
						enemies[ENEMY_TYPE_KID][index] = std::make_unique<KidEnemy>();
						++enemiesHandle[ENEMY_TYPE_KID];
					}

					break;
				default:
					break;
				}
				++enemiesHandle[enemyType];
			}
		}
	}
	//敵を纏めて生成する処理----------------------------------------------------------------



	//ゲームループの初期化----------------------------------------------------------------
	gameFlame = 0;
	//ゴールに触れ無かった場合に次のステージに移動する際の最大フレーム数
	for (int i = 0; i < changeLayerLevelMaxTime.size(); ++i)
	{
		changeLayerLevelMaxTime[i] = 1000;
	}
	gameStageLevel = 0;
	//ゲームループの初期化----------------------------------------------------------------


	eyePos = { 0.0f,5.0f,-10.0f };
	trackingTargetPos = { 0.0f,0.0f,0.0f };
	nowTargerPos = { 0.0f,0.0f,0.0f };
	leftRightAngleVel = { -91.0f,-91.0f,0.0f };
	upDownAngleVel = { -103.0f,-103.0f,0.0f };

	baseEyePos = { 0.0f,5.0f,-10.0f };
	baseTargetPos = { 0.0f,3.0f,0.0f };

	centralPos = { 0.0f,3.0f,8.0f };
	centralPos2 = centralPos;


	besidePoly->data.color = { 255.0f,255.0f,255.0f,255.0f };
	verticlaPoly->data.color = { 255.0f,255.0f,0.0f,255.0f };
	cameraPoly->data.color = { 255.0f,0.0f,0.0f,255.0f };
	r = 12.0f;
	r2 = 12.0f;

	cameraRotaVel = { 30.0f,30.0f,0.0f };



	testEnemyPos = { 0.0f,0.0f,100.0f };
}

void GameScene::Finalize()
{
	//敵の終了処理
	for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
		{
			if (enemies[enemyType][enemyCount] != nullptr)
			{
				enemies[enemyType][enemyCount].reset();
			}
		}
	}

	//生成ハンドルの初期化
	for (int i = 0; i < 10; ++i)
	{
		enemiesHandle[i] = 0;
		addEnemiesHandle[i] = 0;
	}
}

void GameScene::Input()
{
	KeyBoradInputManager *input = KeyBoradInputManager::Instance();

#pragma region カメラ操作
	//debugCameraMove = { 0,0,0 };
	//float debugSpeed = 1;
	////�J�����ړ�
	//if (input->InputState(DIK_D))
	//{
	//	debugCameraMove.x = -debugSpeed;
	//}
	//if (input->InputState(DIK_A))
	//{
	//	debugCameraMove.x = debugSpeed;
	//}
	//if (input->InputState(DIK_W))
	//{
	//	//y�Ȃ̂ɑO�ɐi��....
	//	debugCameraMove.y = debugSpeed;
	//}
	//if (input->InputState(DIK_S))
	//{
	//	debugCameraMove.y = -debugSpeed;
	//}


	////�J�����p�x
	//if (input->InputState(DIK_RIGHTARROW))
	//{
	//	angle.x += debugSpeed;
	//}
	//if (input->InputState(DIK_LEFTARROW))
	//{
	//	angle.x += -debugSpeed;
	//}

	//if (input->InputState(DIK_UPARROW))
	//{
	//	angle.y += debugSpeed;
	//}
	//if (input->InputState(DIK_DOWNARROW))
	//{
	//	angle.y += -debugSpeed;
	//}
	//eyePos = KazMath::CaluEyePosForDebug(eyePos, debugCameraMove, angle);
	//targetPos = KazMath::CaluTargetPosForDebug(eyePos, angle.x);

#pragma endregion


	if (input->InputTrigger(DIK_END))
	{
		bool debug = false;
		debug = true;
	}

	if (input->InputTrigger(DIK_O))
	{
		sceneNum = 0;
	}

	bool upFlag = false;
	bool downFlag = false;
	bool leftFlag = false;
	bool rightFlag = false;

	if (input->InputState(DIK_T))
	{
		upFlag = true;
	}
	if (input->InputState(DIK_G))
	{
		downFlag = true;
	}
	if (input->InputState(DIK_F))
	{
		leftFlag = true;
	}
	if (input->InputState(DIK_H))
	{
		rightFlag = true;
	}


	cursor.Input
	(
		upFlag,
		downFlag,
		leftFlag,
		rightFlag,
		input->InputState(DIK_RETURN),
		input->InputRelease(DIK_RETURN)
	);



	//線演出確認--------------------------
	if (input->InputState(DIK_RETURN))
	{
		lineLevel.Attack2(player.pos, testEnemyPos, {});
	}
	if (input->InputState(DIK_K))
	{
		lineLevel.ReleaseShot();
	}
	if (input->InputTrigger(DIK_L))
	{
		lineLevel.Release();
	}
	//線演出確認--------------------------



	XMVECTOR vel = {};
	XMVECTOR verticalVel = {};
	XMVECTOR besideVel = {};

	float speed = 1.0f;
	if (upFlag)
	{
		vel.m128_f32[1] = -speed;
		verticalVel = { -speed,-speed,0.0f };
	}
	if (downFlag)
	{
		vel.m128_f32[1] = speed;
		verticalVel = { speed,speed,0.0f };
	}
	if (leftFlag)
	{
		vel.m128_f32[0] = -speed;
		besideVel = { speed,speed,0.0f };
	}
	if (rightFlag)
	{
		vel.m128_f32[0] = speed;
		besideVel = { -speed,-speed,0.0f };
	}

	//カメラの制限
	if (60 <= cameraRotaVel.m128_f32[0])
	{
		besideVel = { 0.0f,0.0f,0.0f };
		cameraRotaVel.m128_f32[0] = { 60.0f };
	}
	if (cameraRotaVel.m128_f32[0] <= 0)
	{
		besideVel = { 0.0f,0.0f,0.0f };
		cameraRotaVel.m128_f32[0] = { 0.0f };
	}
	if (50 <= cameraRotaVel.m128_f32[1])
	{
		verticalVel = { 0.0f,0.0f,0.0f };
		cameraRotaVel.m128_f32[1] = { 50.0f };
	}
	if (cameraRotaVel.m128_f32[1] <= -10)
	{
		verticalVel = { 0.0f,0.0f,0.0f };
		cameraRotaVel.m128_f32[1] = { -10.0f };
	}


	nowTargerPos += vel;
	cameraRotaVel += vel;
	upDownAngleVel += verticalVel;
	leftRightAngleVel += besideVel;
}

void GameScene::Update()
{
	ImGui::Begin("Camera");
	ImGui::InputFloat("EyeX", &baseEyePos.m128_f32[0]);
	ImGui::InputFloat("EyeY", &baseEyePos.m128_f32[1]);
	ImGui::InputFloat("EyeZ", &baseEyePos.m128_f32[2]);
	ImGui::InputFloat("TargetX", &baseTargetPos.m128_f32[0]);
	ImGui::InputFloat("TargetY", &baseTargetPos.m128_f32[1]);
	ImGui::InputFloat("TargetZ", &baseTargetPos.m128_f32[2]);
	ImGui::InputFloat("CentralX", &centralPos.m128_f32[0]);
	ImGui::InputFloat("CentralY", &centralPos.m128_f32[1]);
	ImGui::InputFloat("CentralZ", &centralPos.m128_f32[2]);
	ImGui::InputFloat("Central2X", &centralPos2.m128_f32[0]);
	ImGui::InputFloat("Central2Y", &centralPos2.m128_f32[1]);
	ImGui::InputFloat("Central2Z", &centralPos2.m128_f32[2]);
	ImGui::InputFloat("R", &r);
	ImGui::InputFloat("R2", &r2);
	ImGui::Text("leftRightAngleVel:X%f,Y:%f", leftRightAngleVel.m128_f32[0], leftRightAngleVel.m128_f32[1]);
	ImGui::Text("upDownAngleVel:X%f,Y:%f", upDownAngleVel.m128_f32[0], upDownAngleVel.m128_f32[1]);
	ImGui::Text("cameraRotaVel:X%f,Y:%f", cameraRotaVel.m128_f32[0], cameraRotaVel.m128_f32[1]);
	ImGui::Text("trackUpDownAngleVel:X%f,Y:%f", trackUpDownAngleVel.m128_f32[0], trackUpDownAngleVel.m128_f32[1]);
	ImGui::Text("trackLeftRightAngleVel:X%f,Y:%f", trackLeftRightAngleVel.m128_f32[0], trackLeftRightAngleVel.m128_f32[1]);
	ImGui::End();


	ImGui::Begin("Line");
	ImGui::SliderFloat("PosX", &testEnemyPos.m128_f32[0], -100, 100);
	ImGui::SliderFloat("PosY", &testEnemyPos.m128_f32[1], -100, 100);
	ImGui::SliderFloat("PosZ", &testEnemyPos.m128_f32[2], -100, 100);
	ImGui::End();


	testEnemyPoly->data.transform.pos = testEnemyPos;




#pragma region カメラ挙動

	//左右の角度変更のイージング
	{
		XMVECTOR distance = leftRightAngleVel - trackLeftRightAngleVel;
		trackLeftRightAngleVel += distance * 0.1f;
	}
	//上下の角度変更のイージング
	{
		XMVECTOR distance = upDownAngleVel - trackUpDownAngleVel;
		trackUpDownAngleVel += distance * 0.1f;
	}
	//左右の回転
	besidePoly->data.transform.pos =
	{
		cosf(KazMath::AngleToRadian(trackLeftRightAngleVel.m128_f32[0])) * r,
		0.0f,
		sinf(KazMath::AngleToRadian(trackLeftRightAngleVel.m128_f32[1])) * r
	};
	//上下の回転
	verticlaPoly->data.transform.pos =
	{
		0.0f,
		cosf(KazMath::AngleToRadian(trackUpDownAngleVel.m128_f32[0])) * r2,
		sinf(KazMath::AngleToRadian(trackUpDownAngleVel.m128_f32[1])) * r2
	};
	//上下左右の回転
	cameraPoly->data.transform.pos = (besidePoly->data.transform.pos + centralPos) + (verticlaPoly->data.transform.pos + centralPos2);



	eyePos = KazMath::LoadVecotrToXMFLOAT3(cameraPoly->data.transform.pos);
	targetPos = KazMath::LoadVecotrToXMFLOAT3(baseTargetPos);
	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f });

#pragma endregion

	//ゴールに触れ無かった場合に次のステージに移動する処理----------------------------------------------------------------
	if (changeLayerLevelMaxTime[gameStageLevel] <= gameFlame)
	{
		//++gameStageLevel;
		//gameFlame = 0;
	}
	//ゴールに触れ無かった場合に次のステージに移動する処理----------------------------------------------------------------


#pragma region 生成処理
	//敵を追加で初期化する処理----------------------------------------------------------------
	for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
		{
			//生成されている、初期化している敵のみ更新処理を通す
			bool enableToUseDataFlag = enemies[enemyType][enemyCount] != nullptr && enemies[enemyType][enemyCount]->GetData()->oprationObjData->initFlag;
			if (enableToUseDataFlag)
			{
				EnemyData *enemyData = enemies[enemyType][enemyCount]->GetData().get();
				//追加で生成するデータを検知したら生成する
				if (enemyData->genarateData.enemyType != -1)
				{
					//生成する敵の種類
					int enemyTypeData = enemyData->genarateData.enemyType;
					//最後に生成して次のハンドル
					int nowHandle = addEnemiesHandle[enemyTypeData];
					responeData[enemyTypeData][nowHandle].enemyType = enemyData->genarateData.enemyType;
					//現在のレイヤーレベルに合わせる
					responeData[enemyTypeData][nowHandle].layerLevel = gameStageLevel;
					//現在のフレーム数+インターバルフレーム*個数で設定する
					responeData[enemyTypeData][nowHandle].flame = gameFlame;
					responeData[enemyTypeData][nowHandle].initPos = enemyData->genarateData.initPos;
					//ハンドルを増やす
					++addEnemiesHandle[enemyTypeData];

					//追加したら終了処理を入れる
					enemies[enemyType][enemyCount]->GetData()->genarateData.Finalize();
				}
			}
		}
	}
	//敵を追加で初期化する処理----------------------------------------------------------------


	//敵をどのタイミングで初期化する処理----------------------------------------------------------------
	for (int enemyType = 0; enemyType < responeData.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < responeData[enemyType].size(); ++enemyCount)
		{
			bool enableToUseThisDataFlag = responeData[enemyType][enemyCount].enemyType != -1;
			bool readyToInitDataFlag = responeData[enemyType][enemyCount].flame == gameFlame &&
				responeData[enemyType][enemyCount].layerLevel == gameStageLevel;

			if (enableToUseThisDataFlag && readyToInitDataFlag)
			{
				switch (enemyType)
				{
				case ENEMY_TYPE_NORMAL:
					enemies[enemyType][enemyCount]->Init(responeData[enemyType][enemyCount].initPos);
					break;

				case ENEMY_TYPE_KID:
					enemies[enemyType][enemyCount]->Init(responeData[enemyType][enemyCount].initPos);
					break;
				default:
					break;
				}
			}
		}
	}
	//敵をどのタイミングで初期化する処理----------------------------------------------------------------
#pragma endregion


#pragma region ロックオン
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
				bool enableToLockOnEnemyFlag = enemies[enemyType][enemyCount]->AliveOrNot() && !enemies[enemyType][enemyCount]->LockedOrNot();
				if (CollisionManager::Instance()->CheckRayAndSphere(cursor.hitBox, enemyData->hitBox) &&
					enableToLockOnNumFlag &&
					enableToLockOnEnemyFlag)
				{
					//カーソルのカウント数を増やす
					cursor.Count();
					//敵が当たった情報を書く
					enemies[enemyType][enemyCount]->Hit();
					//線演出をかける際にどの配列を使用するか決める
				}

				//敵が生きている&&ロックオンできる回数が0以下&&ロックオン入力がリリースされた時、敵は死亡する
				if (enemies[enemyType][enemyCount]->AliveOrNot() && enemies[enemyType][enemyCount]->LockedOrNot() && cursor.Release())
				{
					enemies[enemyType][enemyCount]->Dead();
				}
			}
		}
	}
#pragma endregion


#pragma region 更新処理
	//更新処理----------------------------------------------------------------
	player.Update();
	cursor.Update();
	hitBox.Update();

	lineLevel.CalucurateDistance(player.pos, testEnemyPos);
	lineLevel.Update();

	//敵の更新処理
	for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
		{
			//生成されている、初期化している敵のみ更新処理を通す
			bool enableToUseDataFlag = enemies[enemyType][enemyCount] != nullptr && enemies[enemyType][enemyCount]->GetData()->oprationObjData->initFlag;
			if (enableToUseDataFlag)
			{
				enemies[enemyType][enemyCount]->Update();
			}
		}
	}
	//更新処理----------------------------------------------------------------
#pragma endregion

	//ゲームループの経過時間----------------------------------------------------------------
	++gameFlame;
	//ゲームループの経過時間----------------------------------------------------------------

}

void GameScene::Draw()
{
	bg.Draw();
	player.Draw();
	lineLevel.Draw();
	cursor.Draw();
	//hitBox.Draw();
	testEnemyPoly->Draw();

	//besidePoly->Draw();
	//verticlaPoly->Draw();
	//cameraPoly->Draw();

	//敵の描画処理----------------------------------------------------------------
	for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
		{
			//生成されている敵のみ描画処理を通す
			bool enableToUseDataFlag = enemies[enemyType][enemyCount] != nullptr && enemies[enemyType][enemyCount]->GetData()->oprationObjData->initFlag;
			if (enableToUseDataFlag)
			{
				//enemies[enemyType][enemyCount]->Draw();
			}
		}
	}
	//敵の描画処理----------------------------------------------------------------

}

int GameScene::SceneChange()
{
	if (sceneNum != -1) {
		int tmp = sceneNum;
		sceneNum = -1;
		return tmp;
	}

	return SCENE_NONE;
}