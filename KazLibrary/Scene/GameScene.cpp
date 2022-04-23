#include "GameScene.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Input/ControllerInputManager.h"
#include"../Imgui/MyImgui.h"
#include"../Loader/ObjResourceMgr.h"
#include"../Helper/ResourceFilePass.h"
GameScene::GameScene()
{
	besidePoly = std::make_unique<BoxPolygonRender>();
	verticlaPoly = std::make_unique<BoxPolygonRender>();
	cameraPoly = std::make_unique<BoxPolygonRender>();
	testEnemyPoly = std::make_unique<BoxPolygonRender>();

	model = std::make_unique<ObjModelRender>();
	model->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
	model->data.transform.pos = { 0.0f,0.0f,20.0f };
	model->data.transform.scale = { 5.0f,5.0f,5.0f };
	model->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::TestPath + "hamster.obj");
	model->data.color = { 255.0f,0.0f,0.0f,255.0f };
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


	responeData[0][0].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][0].layerLevel = 0;
	responeData[0][0].flame = 360;
	responeData[0][0].initPos = { 0.0f,10.0f,100.0f };


	//テスト用の敵挙動
	for (int layerLevel = 0; layerLevel < 1; ++layerLevel)
	{
		for (int enemyNum = 0; enemyNum < responeData[layerLevel].size(); ++enemyNum)
		{
			responeData[layerLevel][enemyNum].enemyType = ENEMY_TYPE_NORMAL;
			responeData[layerLevel][enemyNum].layerLevel = 0;

			//0~60秒以内に出す
			responeData[layerLevel][enemyNum].flame = KazMath::IntRand(3600, 0);

			//プレイヤーからある程度離れた場所に登場させる
			XMVECTOR tmp;
			for (int dir = 0; dir < 3; ++dir)
			{
				int setAreaNum = KazMath::IntRand(2, 0);
				//X軸とZ軸
				if (dir != 1)
				{
					if (1 <= setAreaNum)
					{
						tmp.m128_f32[dir] = KazMath::FloatRand(100.0f, 20.0f);
					}
					else
					{
						tmp.m128_f32[dir] = KazMath::FloatRand(-20.0f, -100.0f);
					}
				}
				//Y軸
				else
				{
					if (1 <= setAreaNum)
					{
						tmp.m128_f32[dir] = KazMath::FloatRand(50.0f, 0.0f);
					}
					else
					{
						tmp.m128_f32[dir] = KazMath::FloatRand(0.0f, -50.0f);
					}
				}
			}
			responeData[layerLevel][enemyNum].initPos = tmp;

		}
	}


	//敵を纏めて生成する処理----------------------------------------------------------------
	for (int enemyType = 0; enemyType < responeData.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < responeData[enemyType].size(); ++enemyCount)
		{
			if (responeData[enemyType][enemyCount].enemyType != -1)
			{
				switch (enemyType)
				{
				case ENEMY_TYPE_NORMAL:
					enemies[enemyType][enemyCount] = std::make_unique<NormalEnemy>();
					break;

				case ENEMY_TYPE_MOTHER:
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

	centralPos = { 0.0f,3.0f,0.0f };
	centralPos2 = centralPos;


	besidePoly->data.color = { 255.0f,255.0f,255.0f,255.0f };
	verticlaPoly->data.color = { 255.0f,255.0f,0.0f,255.0f };
	cameraPoly->data.color = { 255.0f,0.0f,0.0f,255.0f };
	r = 8.0f;
	r2 = 8.0f;


	testEnemyPos = { 0.0f,0.0f,100.0f };
	mulValue = { 10.0f,30.0f };
	mulValue2 = { 60.0f,60.0f };
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
	ControllerInputManager *inputController = ControllerInputManager::Instance();

#pragma region カメラ操作
	debugCameraMove = { 0,0,0 };
	float debugSpeed = 1;
	//�J�����ړ�
	if (input->InputState(DIK_D))
	{
		debugCameraMove.x = -debugSpeed;
	}
	if (input->InputState(DIK_A))
	{
		debugCameraMove.x = debugSpeed;
	}
	if (input->InputState(DIK_W))
	{
		//y�Ȃ̂ɑO�ɐi��....
		debugCameraMove.y = debugSpeed;
	}
	if (input->InputState(DIK_S))
	{
		debugCameraMove.y = -debugSpeed;
	}


	//�J�����p�x
	if (input->InputState(DIK_RIGHTARROW))
	{
		angle.x += debugSpeed;
	}
	if (input->InputState(DIK_LEFTARROW))
	{
		angle.x += -debugSpeed;
	}

	if (input->InputState(DIK_UPARROW))
	{
		angle.y += debugSpeed;
	}
	if (input->InputState(DIK_DOWNARROW))
	{
		angle.y += -debugSpeed;
	}

#pragma endregion


	if (inputController->InputTrigger(XINPUT_GAMEPAD_A))
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
	bool doneFlag = false;
	bool releaseFlag = false;

	const int DEAD_ZONE = 3000;
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
	if (inputController->InputState(XINPUT_GAMEPAD_A))
	{
		doneFlag = true;
	}
	if (inputController->InputRelease(XINPUT_GAMEPAD_A))
	{
		releaseFlag = true;
	}



	if (!upFlag && !downFlag && !leftFlag && !rightFlag)
	{
		bool debug = false;
	}

	XMVECTOR joyStick;
	joyStick.m128_f32[0] = inputController->GetJoyStickLXNum(0) / 32767.0f;
	joyStick.m128_f32[1] = inputController->GetJoyStickLYNum(0) / 32767.0f;

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


	//カメラの前後左右強制に向かせる処理
	if (input->InputState(DIK_1))
	{
		forceCameraDirVel.m128_f32[0] = 0.0f;
	}
	if (input->InputState(DIK_2))
	{
		forceCameraDirVel.m128_f32[0] = -90.0f;
	}
	if (input->InputState(DIK_3))
	{
		forceCameraDirVel.m128_f32[0] = -180.0f;
	}
	if (input->InputState(DIK_4))
	{
		forceCameraDirVel.m128_f32[0] = -270.0f;
	}



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


	XMVECTOR cursorValue = cursor.GetValue();


	nowTargerPos += vel;

	upDownAngleVel =
	{
		mulValue.x * -cursorValue.m128_f32[1],
		mulValue.y * -cursorValue.m128_f32[1]
	};
	leftRightAngleVel =
	{
		forceCameraDirVel.m128_f32[0] + mulValue2.x * -cursorValue.m128_f32[0],
		forceCameraDirVel.m128_f32[0] + mulValue2.y * -cursorValue.m128_f32[0]
	};


}

void GameScene::Update()
{
	ImGui::Begin("Camera");
	ImGui::Text("Target");
	ImGui::InputFloat("TargetX", &baseTargetPos.m128_f32[0]);
	ImGui::InputFloat("TargetY", &baseTargetPos.m128_f32[1]);
	ImGui::InputFloat("TargetZ", &baseTargetPos.m128_f32[2]);
	ImGui::Text("Central_Side");
	ImGui::InputFloat("CentralX", &centralPos.m128_f32[0]);
	ImGui::InputFloat("CentralY", &centralPos.m128_f32[1]);
	ImGui::InputFloat("CentralZ", &centralPos.m128_f32[2]);
	ImGui::Text("Central_UpDown");
	ImGui::InputFloat("Central2X", &centralPos2.m128_f32[0]);
	ImGui::InputFloat("Central2Y", &centralPos2.m128_f32[1]);
	ImGui::InputFloat("Central2Z", &centralPos2.m128_f32[2]);
	ImGui::Text("Force");
	ImGui::InputFloat("forceCameraDirVel0", &forceCameraDirVel.m128_f32[0]);
	ImGui::InputFloat("forceCameraDirVel1", &forceCameraDirVel.m128_f32[1]);
	ImGui::Text("Circle");
	ImGui::InputFloat("R", &r);
	ImGui::InputFloat("R2", &r2);
	ImGui::Text("MulValue");
	ImGui::InputFloat("mulValueX", &mulValue.x);
	ImGui::InputFloat("mulValueY", &mulValue.y);
	ImGui::InputFloat("mulValue2X", &mulValue2.x);
	ImGui::InputFloat("mulValue2Y", &mulValue2.y);
	ImGui::Text("leftRightAngleVel:X%f,Y:%f", leftRightAngleVel.m128_f32[0], leftRightAngleVel.m128_f32[1]);
	ImGui::Text("upDownAngleVel:X%f,Y:%f", upDownAngleVel.m128_f32[0], upDownAngleVel.m128_f32[1]);
	ImGui::Text("trackUpDownAngleVel:X%f,Y:%f", trackUpDownAngleVel.m128_f32[0], trackUpDownAngleVel.m128_f32[1]);
	ImGui::Text("trackLeftRightAngleVel:X%f,Y:%f", trackLeftRightAngleVel.m128_f32[0], trackLeftRightAngleVel.m128_f32[1]);
	ImGui::End();


	ImGui::Begin("Line");
	ImGui::SliderFloat("PosX", &testEnemyPos.m128_f32[0], -100, 100);
	ImGui::SliderFloat("PosY", &testEnemyPos.m128_f32[1], -100, 100);
	ImGui::SliderFloat("PosZ", &testEnemyPos.m128_f32[2], -100, 100);
	ImGui::End();




	//操作感に関わる設定
	ImGui::Begin("Move");
	ImGui::Text("dontMoveCameraStartPos:X%f,Y:%f", cursor.dontMoveCameraStartPos.m128_f32[0], cursor.dontMoveCameraStartPos.m128_f32[1]);
	ImGui::Text("dontMoveCameraEndPos:X%f,Y:%f", cursor.dontMoveCameraEndPos.m128_f32[0], cursor.dontMoveCameraEndPos.m128_f32[1]);
	ImGui::Text("CursorPos:X%f,Y:%f", cursor.cursorPos.m128_f32[0], cursor.cursorPos.m128_f32[1]);
	ImGui::Text("CameraMoveValue:X%f,Y:%f", cursor.cameraMoveValue.m128_f32[0], cursor.cameraMoveValue.m128_f32[1]);
	ImGui::InputFloat("limitValue:X", &cursor.limitValue.m128_f32[0]);
	ImGui::InputFloat("limitValue:Y", &cursor.limitValue.m128_f32[1]);
	ImGui::InputFloat("NO_MOVE_DISTANCE:X", &cursor.NO_MOVE_DISTANCE.m128_f32[0]);
	ImGui::InputFloat("NO_MOVE_DISTANCE:Y", &cursor.NO_MOVE_DISTANCE.m128_f32[1]);
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
		trackUpDownAngleVel.m128_f32[0],
		0.0f,
	};


	//注視点の自動
	//baseTargetPos.m128_f32[2] = 0.0f + 1.0f * -cursor.GetValue().m128_f32[1];
	//if (baseTargetPos.m128_f32[2] <= 0.0f)
	//{
	//	baseTargetPos.m128_f32[2] = 0.0f;
	//}

	//centralPos.m128_f32[2] = 0.0f + 3.0f * -cursor.GetValue().m128_f32[1];




	{
		float mul = 1.0f;
		float limitValue = 3.0f;
		//上下でプレイヤーのいる位置を変える
		if (0.1f <= cursor.GetValue().m128_f32[1])
		{
			mul *= -1.0f;
			limitValue = 2.5f;
		}

		honraiPlayerCameraPos.m128_f32[0] = 0.0f + (2.0f * -cursor.GetValue().m128_f32[0]);
		honraiPlayerCameraPos.m128_f32[2] = 0.0f + (limitValue * cursor.GetValue().m128_f32[1]) * mul;


		if (0.0f <= honraiPlayerCameraPos.m128_f32[2])
		{
			honraiPlayerCameraPos.m128_f32[2] = 0.0f;
		}
		XMVECTOR distance = honraiPlayerCameraPos - player.pos;
		player.pos += distance * 0.1f;
	}


	//上下左右の回転
	cameraPoly->data.transform.pos = centralPos + (besidePoly->data.transform.pos + verticlaPoly->data.transform.pos);




	//eyePos = KazMath::LoadVecotrToXMFLOAT3(cameraPoly->data.transform.pos);
	//targetPos = KazMath::LoadVecotrToXMFLOAT3(baseTargetPos);

	//デバック用
	eyePos = KazMath::CaluEyePosForDebug(eyePos, debugCameraMove, angle);
	targetPos = KazMath::CaluTargetPosForDebug(eyePos, angle.x);
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
				bool hitFlag = CollisionManager::Instance()->CheckRayAndSphere(cursor.hitBox, enemyData->hitBox);
				if (hitFlag &&
					enableToLockOnNumFlag &&
					enableToLockOnEnemyFlag)
				{
					//カーソルのカウント数を増やす
					cursor.Count();
					//敵が当たった情報を書く
					enemies[enemyType][enemyCount]->Hit();

					//線演出をかける際にどの配列を使用するか決める
					for (int i = 0; i < lineEffectArrayData.size(); ++i)
					{
						if (!lineEffectArrayData[i].usedFlag)
						{
							lineLevel[i].Attack2(player.pos, *enemyData->hitBox.center, {});
							lineEffectArrayData[i].usedFlag = true;
							lineEffectArrayData[i].lineIndex = i;
							lineEffectArrayData[i].enemyTypeIndex = enemyType;
							lineEffectArrayData[i].enemyIndex = enemyCount;
							break;
						}
					}
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
	stage.Update();

	//ロックオンのリリース処理
	if (cursor.releaseFlag)
	{
		for (int i = 0; i < lineEffectArrayData.size(); ++i)
		{
			if (lineEffectArrayData[i].usedFlag)
			{
				int lineIndex = lineEffectArrayData[i].lineIndex;
				lineLevel[lineIndex].ReleaseShot();
				lineEffectArrayData[i].Reset();
			}
		}
	}

	//ロックオン中に必要なデータ
	for (int i = 0; i < lineEffectArrayData.size(); ++i)
	{
		if (lineEffectArrayData[i].usedFlag)
		{
			int lineIndex = lineEffectArrayData[i].lineIndex;
			int enemyTypeIndex = lineEffectArrayData[i].enemyTypeIndex;
			int enemyIndex = lineEffectArrayData[i].enemyIndex;
			XMVECTOR pos = *enemies[enemyTypeIndex][enemyIndex]->GetData()->hitBox.center;
			lineLevel[lineIndex].CalucurateDistance(player.pos, pos);
		}
	}


	for (int i = 0; i < lineLevel.size(); ++i)
	{
		lineLevel[i].playerPos = player.pos;
		lineLevel[i].Update();
	}

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
	for (int i = 0; i < lineLevel.size(); ++i)
	{
		lineLevel[i].Draw();
	}
	cursor.Draw();
	//hitBox.Draw();
	//testEnemyPoly->Draw();

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

	model->Draw();

	stage.Draw();
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