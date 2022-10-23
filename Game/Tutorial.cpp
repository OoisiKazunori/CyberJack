#include"../KazLibrary/Input/KeyBoradInputManager.h"
#include"../KazLibrary/Input/ControllerInputManager.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../Game/Enemy/TutorialBlock.h"
#include"../Game/Stage/FirstStage.h"
#include"Tutorial.h"

Tutorial::Tutorial() :LOG_FONT_SIZE(1.0f), FLAME_MAX_TIME(20)
{
	gridR[0] = std::make_unique<DrawGrid>(KazMath::Color(14, 12, 13, 255));
	gridR[1] = std::make_unique<DrawGrid>(KazMath::Color(14, 12, 13, 255));

	eyePos = { 0.0f, 3.0f, -8.0f + 15.0f };
	targetPos = { 0.0f, 3.0f, 15.0f };


	lockSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "Lock.wav", false);
	damageSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "EenemyDamage.wav", false);


	renderTarget = std::make_unique<GameRenderTarget>(KazMath::Color(14, 12, 13, 255));

	mainRenderTarget.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	mainRenderTarget.data.pipelineName = PIPELINE_NAME_SPRITE_NOBLEND;
	mainRenderTarget.data.handleData = renderTarget->GetGameRenderTargetHandle();

}

Tutorial::~Tutorial()
{
	renderTarget->Finalize();
}

void Tutorial::Init(bool SKIP_FLAG)
{
	sceneNum = -1;
	cameraIndex = 0;

	KazMath::Color lBaseColor(155, 155, 155, 155);
	KazMath::Color lFlashColor(155, 155, 155, 255);
	std::array<KazMath::Color, 2>lFlashColorArray = { lBaseColor,lFlashColor };
	gridR[0]->Init(true, 800.0f, -200.0f, &cameraIndex, false, false, lFlashColorArray);
	gridR[1]->Init(true, 800.0f, -200.0f, &cameraIndex, false, false, lFlashColorArray);

	player.Init({ 0.0f,0.0f,15.0f }, false, false);
	cameraWork.Init();
	stringLog.Init({ 50.0f,50.0f });

	portalFlameBasePosArray[0] = { -60.0f,45.0f,150.0f };
	portalFlameBasePosArray[1] = { 60.0f, -35.0f,150.0f };

	float lLength = abs(portalFlameBasePosArray[0].x) + abs(portalFlameBasePosArray[1].x);
	tutorialArrayIndex = 0;
	{
		std::vector<KazMath::Vec3<float>>lBoxPos;
		lBoxPos.push_back(portalFlameBasePosArray[0] + KazMath::Vec3<float>(0.0f, 0.0f, -50.0f));
		lBoxPos.push_back(portalFlameBasePosArray[1] + KazMath::Vec3<float>(0.0f, 0.0f, -100.0f));
		tutorial[0].Init("test", lBoxPos);
	}
	{
		std::vector<KazMath::Vec3<float>>lBoxPos;
		lBoxPos.push_back(portalFlameBasePosArray[0] + KazMath::Vec3<float>(0.0f, -20.0f, -100.0f));
		lBoxPos.push_back(portalFlameBasePosArray[1] + KazMath::Vec3<float>(0.0f, 20.0f, -100.0f));
		tutorial[1].Init("test", lBoxPos);
	}
	{
		std::vector<KazMath::Vec3<float>>lBoxPos;
		for (int i = 0; i < 4; ++i)
		{
			KazMath::Vec3<float>lPos;
			lPos = portalFlameBasePosArray[0] + KazMath::Vec3<float>((lLength / 5.0f) * i + 15.0f, 0.0f, -KazMath::Rand(130.0f, 10.0f));
			lBoxPos.push_back(lPos);
		}
		for (int i = 0; i < 4; ++i)
		{
			KazMath::Vec3<float>lPos;
			lPos = portalFlameBasePosArray[1] + KazMath::Vec3<float>(-(lLength / 5.0f) * i - 15.0f, 0.0f, -KazMath::Rand(130.0f, 10.0f));
			lBoxPos.push_back(lPos);
		}
		tutorial[2].Init("test", lBoxPos);
	}


	//チュートリアル数、そこで生成される敵の数
	for (int tutorialNum = 0; tutorialNum < enemies.size(); ++tutorialNum)
	{
		for (int enemyIndex = 0; enemyIndex < tutorial[tutorialNum].GetBoxNum(); ++enemyIndex)
		{
			enemies[tutorialNum][enemyIndex] = std::make_unique<TutorialBlock>();
		}
	}

	for (int tutorialNum = 0; tutorialNum < enemies.size(); ++tutorialNum)
	{
		for (int enemyIndex = 0; enemyIndex < enemies[tutorialNum].size(); ++enemyIndex)
		{
			hitLineTimer[tutorialNum][enemyIndex] = 0;
		}
	}

	for (int tutorialNum = 0; tutorialNum < enemies.size(); ++tutorialNum)
	{
		initEnemyFlagArray[tutorialNum] = false;
	}
	cursor.Init();

	flameTimer[0] = 0;
	flameTimer[1] = 0;

	if (SKIP_FLAG)
	{
		tutorialAllClearFlag = true;
		tutorialFlag = false;
	}
	else
	{
		tutorialAllClearFlag = false;
		tutorialFlag = true;
	}
	initSceneFlag = false;

	tutorialAllClearFlag = true;

	angleC = 60.0f;
}

void Tutorial::Finalize()
{
}

void Tutorial::Input()
{
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

	if (inputController->InputTrigger(XINPUT_GAMEPAD_B))
	{
		for (int enemyIndex = 0; enemyIndex < enemies[tutorialArrayIndex].size(); ++enemyIndex)
		{
			bool enableToUseDataFlag = enemies[tutorialArrayIndex][enemyIndex] == nullptr ||
				!enemies[tutorialArrayIndex][enemyIndex]->GetData()->oprationObjData->initFlag;
			if (!enableToUseDataFlag)
			{
				enemies[tutorialArrayIndex][enemyIndex]->Dead();
			}
		}
	}
}

void Tutorial::Update()
{
	ImGui::Begin("Angle");
	ImGui::DragFloat("a", &angleC);
	ImGui::End();

	const float MAX_ANGLE = 120.0f;
	const float DEFAULT_ANGLE = 60.0f;
	if (portalEffect.DrawNextPortal())
	{
		angleC = MAX_ANGLE + portalEffect.GetNextRate() * -(MAX_ANGLE - DEFAULT_ANGLE);
	}
	else
	{
		angleC = DEFAULT_ANGLE + portalEffect.GetRate() * (MAX_ANGLE - DEFAULT_ANGLE);
	}


	CameraMgr::Instance()->CameraSetting(angleC, 10000.0f, 0);
	CameraMgr::Instance()->CameraSetting(angleC, 10000.0f, 1);

	eyePos = cameraWork.GetEyePos();
	targetPos = cameraWork.GetTargetPos();
	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f }, 0);


	for (int enemyIndex = 0; enemyIndex < tutorial[tutorialArrayIndex].GetBoxNum(); ++enemyIndex)
	{
		if (initEnemyFlagArray[tutorialArrayIndex])
		{
			continue;
		}
		EnemyGenerateData lData;
		lData.initPos = tutorial[tutorialArrayIndex].GetBoxPosArray()[enemyIndex];
		enemies[tutorialArrayIndex][enemyIndex]->Init(lData);
	}
	initEnemyFlagArray[tutorialArrayIndex] = true;


#pragma region ブロックのロックオン処理
	for (int tutorialNum = 0; tutorialNum < enemies.size(); ++tutorialNum)
	{
		for (int enemyIndex = 0; enemyIndex < enemies[tutorialNum].size(); ++enemyIndex)
		{
			if (enemies[tutorialNum][enemyIndex] == nullptr)
			{
				continue;
			}
			bool enableToLockOnEnemyFlag = enemies[tutorialNum][enemyIndex]->IsAlive() && !enemies[tutorialNum][enemyIndex]->LockedOrNot();

			//チュートリアルによって判定の処理を変える
			switch (tutorialNum)
			{
			case 0:
				if (enableToLockOnEnemyFlag && CollisionManager::Instance()->CheckRayAndSphere(cursor.hitBox, enemies[0][enemyIndex]->GetData()->hitBox))
				{
					//敵が当たった情報を書く
					enemies[0][enemyIndex]->Dead();
					stringLog.WriteLog(enemies[0][enemyIndex]->GetData()->oprationObjData->name, LOG_FONT_SIZE);
				}
				else
				{
					continue;
				}
				break;
			case 1:
				if (!KazEnemyHelper::LockOn(&cursor, enemies[tutorialNum][enemyIndex], &stringLog, LOG_FONT_SIZE, lockSoundHandle))
				{
					continue;
				}
				//線演出をかける際にどの配列を使用するか決める
				for (int lineIndex = 0; lineIndex < lineEffectArrayData.size(); ++lineIndex)
				{
					if (!lineEffectArrayData[lineIndex].usedFlag)
					{
						lineLevel[lineIndex].Attack(lineEffectArrayData[lineIndex].startPos, *enemies[tutorialNum][enemyIndex]->GetData()->hitBox.center);
						lineEffectArrayData[lineIndex].usedFlag = true;
						lineEffectArrayData[lineIndex].lineIndex = lineIndex;
						lineEffectArrayData[lineIndex].enemyTypeIndex = tutorialNum;
						lineEffectArrayData[lineIndex].enemyIndex = enemyIndex;
						break;
					}
				}
			case 2:
				if (!KazEnemyHelper::LockOn(&cursor, enemies[tutorialNum][enemyIndex], &stringLog, LOG_FONT_SIZE, lockSoundHandle))
				{
					continue;
				}
				++cursorLockOnNum;
				//線演出をかける際にどの配列を使用するか決める
				for (int lineIndex = 0; lineIndex < lineEffectArrayData.size(); ++lineIndex)
				{
					if (!lineEffectArrayData[lineIndex].usedFlag)
					{
						lineLevel[lineIndex].Attack(lineEffectArrayData[lineIndex].startPos, *enemies[tutorialNum][enemyIndex]->GetData()->hitBox.center);
						lineEffectArrayData[lineIndex].usedFlag = true;
						lineEffectArrayData[lineIndex].lineIndex = lineIndex;
						lineEffectArrayData[lineIndex].enemyTypeIndex = tutorialNum;
						lineEffectArrayData[lineIndex].enemyIndex = enemyIndex;
						break;
					}
				}
				break;
			default:
				break;
			}
		}
	}



	for (int tutorialIndex = 0; tutorialIndex < enemies.size(); ++tutorialIndex)
	{
		for (int enemyIndex = 0; enemyIndex < enemies[tutorialIndex].size(); ++enemyIndex)
		{
			//生成されている、初期化している敵のみ更新処理を通す
			bool enableToUseDataFlag = enemies[tutorialIndex][enemyIndex] != nullptr && enemies[tutorialIndex][enemyIndex]->GetData()->oprationObjData->initFlag;
			if (enableToUseDataFlag)
			{
				//敵のデータのポインタを代入
				EnemyData *enemyData = enemies[tutorialIndex][enemyIndex]->GetData().get();

				for (int i = 0; i < lineEffectArrayData.size(); ++i)
				{
					bool sameEnemyFlag = lineEffectArrayData[i].enemyTypeIndex == tutorialIndex && lineEffectArrayData[i].enemyIndex == enemyIndex;
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

	//ロックオン中に必要なデータ
	for (int i = 0; i < lineEffectArrayData.size(); ++i)
	{
		if (lineEffectArrayData[i].usedFlag)
		{
			int lineIndex = lineEffectArrayData[i].lineIndex;
			int enemyTypeIndex = lineEffectArrayData[i].enemyTypeIndex;
			int enemyIndex = lineEffectArrayData[i].enemyIndex;
			KazMath::Vec3<float> pos = *enemies[enemyTypeIndex][enemyIndex]->GetData()->hitBox.center;
			lineLevel[lineIndex].CalucurateDistance(player.pos, pos);
		}
	}

#pragma endregion

#pragma region 線のロックオン解放
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

	//線がたどり着いたら敵を死亡させる
	for (int i = 0; i < lineEffectArrayData.size(); ++i)
	{
		if (lineEffectArrayData[i].usedFlag)
		{
			int lineIndex = lineEffectArrayData[i].lineIndex;
			int enemyTypeIndex = lineEffectArrayData[i].enemyTypeIndex;
			int enemyIndex = lineEffectArrayData[i].enemyIndex;

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
			}
		}
	}

	//8体ロックオン時のチュートリアル処理
	//8体一括で倒せたら敵全員を倒す、倒せなければやり直す
	if (tutorialArrayIndex == 2)
	{
		bool lEnemyDeadFlag = false;
		for (int enemyIndex = 0; enemyIndex < enemies[2].size(); ++enemyIndex)
		{
			if (enemies[2][enemyIndex] != nullptr && !enemies[2][enemyIndex]->GetData()->oprationObjData->enableToHitFlag)
			{
				lEnemyDeadFlag = true;
			}
		}

		if (cursorLockOnNum < 8 && lEnemyDeadFlag)
		{
			cursorLockOnNum = 0;
			initEnemyFlagArray[2] = false;

			for (int i = 0; i < hitLineTimer[2].size(); ++i)
			{
				hitLineTimer[2][i] = 0;
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
				lineLevel[lineIndex].lineReachObjFlag = false;
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


	//ポータルを形成する線
	for (int flameIndex = 0; flameIndex < portalLineTimer.size(); ++flameIndex)
	{
		if (hitLineTimer[0][flameIndex] < FLAME_MAX_TIME)
		{
			continue;
		}

		portalFlameLineR[flameIndex][0].data.startPos = portalFlameBasePosArray[flameIndex];
		portalFlameLineR[flameIndex][1].data.startPos = portalFlameBasePosArray[flameIndex];
		KazMath::Vec3<float>lDistance = {};
		if (flameIndex % 2 == 0)
		{
			lDistance = portalFlameBasePosArray[1] - portalFlameBasePosArray[0];
		}
		else
		{
			lDistance = portalFlameBasePosArray[0] - portalFlameBasePosArray[1];
		}

		++flameTimer[flameIndex];
		if (FLAME_MAX_TIME <= flameTimer[flameIndex])
		{
			flameTimer[flameIndex] = FLAME_MAX_TIME;
		}
		StrachLine(portalFlameBasePosArray[flameIndex], KazMath::Vec3<float>(lDistance.x, 0.0f, 0.0f), &portalFlameLineR[flameIndex][0].data.endPos, flameTimer[flameIndex], FLAME_MAX_TIME);
		StrachLine(portalFlameBasePosArray[flameIndex], KazMath::Vec3<float>(0.0f, lDistance.y, 0.0f), &portalFlameLineR[flameIndex][1].data.endPos, flameTimer[flameIndex], FLAME_MAX_TIME);
	}


	//ブロック壊した際に線を伸ばす
	for (int tutorialNum = 0; tutorialNum < enemies.size(); ++tutorialNum)
	{
		for (int enemyIndex = 0; enemyIndex < enemies[tutorialNum].size(); ++enemyIndex)
		{
			bool enableToUseDataFlag = enemies[tutorialNum][enemyIndex] == nullptr ||
				!enemies[tutorialNum][enemyIndex]->GetData()->oprationObjData->initFlag;
			if (enableToUseDataFlag)
			{
				continue;
			}

			if (!enemies[tutorialNum][enemyIndex]->GetData()->oprationObjData->enableToHitFlag)
			{
				int *lTimer = &hitLineTimer[tutorialNum][enemyIndex];
				++ *lTimer;
				if (FLAME_MAX_TIME <= *lTimer)
				{
					*lTimer = FLAME_MAX_TIME;
				}
				float lZDistance = portalFlameBasePosArray[0].z - hitLineR[tutorialNum][enemyIndex].data.startPos.z;
				hitLineR[tutorialNum][enemyIndex].data.startPos = *enemies[tutorialNum][enemyIndex]->GetData()->hitBox.center;
				StrachLine(hitLineR[tutorialNum][enemyIndex].data.startPos, KazMath::Vec3<float>(0.0f, 0.0f, lZDistance), &hitLineR[tutorialNum][enemyIndex].data.endPos, *lTimer, FLAME_MAX_TIME);
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

	player.Update();
	cursor.Update();
	cameraWork.Update(cursor.GetValue(), &player.pos, false);
	gridR[0]->Update(-1.0f);
	gridR[1]->Update(800.0f);


	for (int tutorialNum = 0; tutorialNum < enemies.size(); ++tutorialNum)
	{
		bool lIsNotAllKillFlag = true;
		for (int enemyIndex = 0; enemyIndex < enemies[tutorialNum].size(); ++enemyIndex)
		{
			bool enableToUseDataFlag = enemies[tutorialNum][enemyIndex] == nullptr ||
				!enemies[tutorialNum][enemyIndex]->GetData()->oprationObjData->initFlag;
			if (enableToUseDataFlag)
			{
				continue;
			}
			if (enemies[tutorialNum][enemyIndex]->GetData()->oprationObjData->enableToHitFlag)
			{
				lIsNotAllKillFlag = false;
			}
			enemies[tutorialNum][enemyIndex]->Update();
		}

		//そのステージの敵が全て死んでいたらチュートリアルが完了したことを知らせる
		if (lIsNotAllKillFlag && tutorialArrayIndex == tutorialNum)
		{
			tutorial[tutorialNum].Succeed();
		}
	}


	if (tutorial[tutorialArrayIndex].NextTutorial())
	{
		++tutorialArrayIndex;
		//チュートリアル数より超えたら全て達成したことを示す
		if (tutorial.size() <= tutorialArrayIndex)
		{
			tutorialArrayIndex = static_cast<int>(tutorial.size() - 1);
			tutorialAllClearFlag = true;
		}
	}
	if (!initEffectFlag && tutorialAllClearFlag)
	{
		portalEffect.Init();
		portalEffect.Start();
		initEffectFlag = true;
	}

	tutorial[tutorialArrayIndex].Update();
	portalEffect.Update(player.pos);

	if (tutorialAllClearFlag && portalEffect.IsFinish() && !initSceneFlag)
	{
		tutorialFlag = false;
		initSceneFlag = true;
	}
}

void Tutorial::Draw()
{
	RenderTargetStatus::Instance()->SetDoubleBufferFlame();
	RenderTargetStatus::Instance()->ClearDoubuleBuffer(BG_COLOR);

	portalEffect.Draw(&cursor);


	//レンダーターゲットセット
	renderTarget->SetRenderTarget();
	//ゲーム内描画

	for (int i = 0; i < gridR.size(); ++i)
	{
		gridR[i]->Draw();
	}

	player.Draw();

	for (int i = 0; i < lineLevel.size(); ++i)
	{
		lineLevel[i].Draw();
	}

	for (int tutorialNum = 0; tutorialNum < enemies.size(); ++tutorialNum)
	{
		for (int enemyIndex = 0; enemyIndex < enemies[tutorialNum].size(); ++enemyIndex)
		{
			//生成されている敵のみ描画処理を通す
			bool enableToUseDataFlag = enemies[tutorialNum][enemyIndex] == nullptr ||
				!enemies[tutorialNum][enemyIndex]->GetData()->oprationObjData->initFlag;
			if (enableToUseDataFlag)
			{
				continue;
			}
			enemies[tutorialNum][enemyIndex]->Draw();
		}
	}


	for (int tutorialNum = 0; tutorialNum < hitLineR.size(); ++tutorialNum)
	{
		for (int enemyIndex = 0; enemyIndex < hitLineR[tutorialNum].size(); ++enemyIndex)
		{
			hitLineR[tutorialNum][enemyIndex].Draw();
		}
	}

	for (int portalFlameIndex = 0; portalFlameIndex < portalFlameLineR.size(); ++portalFlameIndex)
	{
		for (int portalLineIndex = 0; portalLineIndex < portalFlameLineR[portalFlameIndex].size(); ++portalLineIndex)
		{
			portalFlameLineR[portalFlameIndex][portalLineIndex].Draw();
		}
	}

	for (int i = 0; i < tutorial.size(); ++i)
	{
		tutorial[i].Draw();
	}

	//次ポータルの描画
	if (portalEffect.DrawNextPortal())
	{
		portalEffect.nextPortalRender.data.handleData = handle;
	}
	if (portalEffect.IsStart() && !portalEffect.IsFinish())
	{
		portalEffect.portalRender.Draw();
	}

	cursor.Draw();
	//ブルーム系統のレンダーターゲット切り替え
	renderTarget->Draw();


	//ポータル遷移中の切り替え
	if (portalEffect.disappearFlag || portalEffect.DrawNextPortal())
	{
		mainRenderTarget.data.handleData = portalEffect.renderTarget->GetGameRenderTargetHandle();
	}
	else
	{
		mainRenderTarget.data.handleData = renderTarget->GetGameRenderTargetHandle();
	}

	//ゲーム画面描画
	//mainRenderTarget.Draw();
}