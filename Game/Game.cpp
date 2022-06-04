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


Game::Game()
{
	besidePoly = std::make_unique<BoxPolygonRender>();
	verticlaPoly = std::make_unique<BoxPolygonRender>();
	cameraPoly = std::make_unique<BoxPolygonRender>();

	model = std::make_unique<ObjModelRender>();
	model->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
	model->data.transform.pos = { 0.0f,0.0f,20.0f };
	model->data.transform.scale = { 5.0f,5.0f,5.0f };
	model->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::TestPath + "hamster.obj");
	model->data.color = { 255.0f,0.0f,0.0f,255.0f };

	//mainRenderTarget.data.handle = RenderTargetStatus::Instance()->CreateRenderTarget({ WIN_X,WIN_Y }, BG_COLOR, DXGI_FORMAT_R8G8B8A8_UNORM);
	mainRenderTarget.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	mainRenderTarget.data.pipelineName = PIPELINE_NAME_SPRITE_NOBLEND;

	std::vector<MultiRenderTargetData> data;
	data.push_back(MultiRenderTargetData());
	data.push_back(MultiRenderTargetData());
	data[0].graphSize = { WIN_X,WIN_Y };
	data[0].backGroundColor = BG_COLOR;
	data[1].graphSize = { WIN_X,WIN_Y };
	data[1].backGroundColor = { 0.0f,0.0f,0.0f };

	handles =
		RenderTargetStatus::Instance()->CreateMultiRenderTarget(data, DXGI_FORMAT_R8G8B8A8_UNORM);
	mainRenderTarget.data.handle = handles[0];

	addHandle = RenderTargetStatus::Instance()->CreateRenderTarget({ WIN_X,WIN_Y }, XMFLOAT3(0.0f, 0.0f, 0.0f), DXGI_FORMAT_R8G8B8A8_UNORM);
	addRenderTarget.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	addRenderTarget.data.pipelineName = PIPELINE_NAME_ADDBLEND;

	buler = std::make_unique<GaussianBuler>(XMFLOAT2(WIN_X, WIN_Y), XMFLOAT3(0.0f, 0.0f, 0.0f));


	luminaceTex.data.pipelineName = PIPELINE_NAME_SPRITE_LUMI;
	luminaceTex.data.handle = handles[0];
	luminaceTex.data.addHandle.handle[0] = handles[1];
	luminaceTex.data.addHandle.paramType[0] = GRAPHICS_PRAMTYPE_TEX2;
	luminaceTex.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };


	lineStartPoly[0].data.transform.pos = { 0.5f,0.15f,0.0f };
	lineStartPoly[1].data.transform.pos = { -0.5f,0.0f,-0.5f };
	lineStartPoly[2].data.transform.pos = { 0.5f,0.6f,0.5f };
	lineStartPoly[3].data.transform.pos = { 0.5f,1.0f,0.0f };
	lineStartPoly[4].data.transform.pos = { 0.5f,-1.0f,-1.0f };
	lineStartPoly[5].data.transform.pos = { -0.5f,-1.0f,0.0f };
	lineStartPoly[6].data.transform.pos = { 0.5f,0.3f,-0.3f };
	lineStartPoly[7].data.transform.pos = { 0.0f,-0.3f,-1.0f };

	CameraMgr::Instance()->CameraSetting(60.0f, 1200.0f);

	stages[0] = std::make_unique<FirstStage>();
	stages[1] = std::make_unique<SecondStage>();
	stages[2] = std::make_unique<ThridStage>();


	blackTex.data.handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "Black.png");
	blackTex.data.pipelineName = PIPELINE_NAME_SPRITE_Z_ALWAYS;
	blackTex.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	blackTex.data.transform.scale = { 2000.0f,2000.0f };
	blackTex.data.alpha = { 0.0f };


	pressAButtonTex.data.handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "PressAButton.png");
	pressAButtonTex.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f + 100.0f };
	pressAButtonTex.data.transform.scale = { 0.4f,0.4f };

	gameOverTex.data.handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::GameOverPath + "GameOver.png");
	gameOverTex.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	gameOverTex.data.transform.scale = { 1.2f,1.2f };


	potalTexHandle = RenderTargetStatus::Instance()->CreateRenderTarget({ WIN_X,WIN_Y }, XMFLOAT3(0.0f, 0.0f, 0.0f), DXGI_FORMAT_R8G8B8A8_UNORM);
}

Game::~Game()
{
	//レンダーターゲットの破棄
	RenderTargetStatus::Instance()->DeleteRenderTarget(addHandle);
	RenderTargetStatus::Instance()->DeleteMultiRenderTarget(handles);
	RenderTargetStatus::Instance()->DeleteRenderTarget(potalTexHandle);
}

void Game::Init(const array<array<ResponeData, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> &RESPONE_DATA)
{
	player.Init(KazMath::Transform3D().pos);
	cursor.Init();

	hitBox.Init({ 0.0f,0.0f,30.0f }, 1);

	stageUI.Init();

	sceneNum = SCENE_NONE;


	BufferMemorySize enemySize = { 0,ENEMY_NUM_MAX };
	BufferMemorySize eventSize = { ENEMY_NUM_MAX,ENEMY_NUM_MAX + 50 };

	responeData = RESPONE_DATA;

	//操作可能OBJを纏めて生成する処理----------------------------------------------------------------
	for (int enemyType = 0; enemyType < responeData.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < responeData[enemyType].size(); ++enemyCount)
		{
			if (responeData[enemyType][enemyCount].layerLevel != -1)
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

				case ENEMY_TYPE_MISILE:
					enemies[enemyType][enemyCount] = std::make_unique<NormalMisileEnemy>();
					enemies[ENEMY_TYPE_MISILE_SPLINE][enemyCount] = std::make_unique<SplineMisile>();
					++enemiesHandle[ENEMY_TYPE_MISILE_SPLINE];
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
	gameStartFlag = false;
	gameFlame = 0;
	//ゴールに触れ無かった場合に次のステージに移動する際の最大フレーム数
	for (int i = 0; i < changeLayerLevelMaxTime.size(); ++i)
	{
		changeLayerLevelMaxTime[i] = 1800;
	}
	changeLayerLevelMaxTime[0] = 60 * 30;
	changeLayerLevelMaxTime[1] = 60 * 36;
	gameStageLevel = 0;
	//ゲームループの初期化----------------------------------------------------------------


	eyePos = { 0.0f,5.0f,-10.0f };
	trackingTargetPos = { 0.0f,0.0f,0.0f };
	nowTargerPos = { 0.0f,0.0f,0.0f };
	leftRightAngleVel = { -91.0f,-91.0f,0.0f };
	upDownAngleVel = { 0.0f,0.0f,0.0f };

	trackLeftRightAngleVel = leftRightAngleVel;
	trackUpDownAngleVel = upDownAngleVel;

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
	cameraChangeFlag = true;

	forceCameraDirVel.m128_f32[0] = -90.0f;

	appearGoalBoxPos = { -10.0f,5.0f,40.0f };
	responeGoalBoxPos = { -10.0f,-100.0f,40.0f };
	goalBox.Init(responeGoalBoxPos);

	stageNum = 0;
	initAppearFlag = false;

	movieEffect.Init();


	gameOverFlag = false;
	readyToBlackOutFlag = false;
	readyToBlackOutToGoTitleFlag = false;
	flashFlag = false;
	flashTimer = 0;

	layerLevelEyePos = { -4.15f,3.0f,40.0f };
	layerLevelTargetPos = { -4.15f,3.0f,45.0f };


	titleLogoTex.data.alpha = 255.0f;
	titleLogoTex.data.transform.pos = { 0.0f,50.0f,500.0f };
	titleLogoTex.data.handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TitlePath + "TitleName.png");
	doneSprite.Init({ 0.0f,0.0f,500.0f }, TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TitlePath + "Start.png"));


	tutorialWindow.Init();

	startFlag = false;

}

void Game::Finalize()
{
}

void Game::Input()
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
	if (inputController->InputTrigger(XINPUT_GAMEPAD_A) && 360 <= gameFlame && 2 <= gameStageLevel)
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
	if (input->InputState(DIK_Z))
	{
		forceCameraDirVel.m128_f32[0] = 0.0f;
	}
	if (input->InputState(DIK_X))
	{
		forceCameraDirVel.m128_f32[0] = -90.0f;
	}
	if (input->InputState(DIK_C))
	{
		forceCameraDirVel.m128_f32[0] = -180.0f;
	}
	if (input->InputState(DIK_V))
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

	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_R))
	{
		gameFlame = 0;
		for (int i = 0; i < addEnemiesHandle.size(); ++i)
		{
			addEnemiesHandle[i] = 0;;
		}
	}


}

void Game::Update()
{
	/*ImGui::Begin("Camera");
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
	ImGui::End();*/


	//操作感に関わる設定
	/*ImGui::Begin("Move");
	ImGui::Text("dontMoveCameraStartPos:X%f,Y:%f", cursor.dontMoveCameraStartPos.m128_f32[0], cursor.dontMoveCameraStartPos.m128_f32[1]);
	ImGui::Text("dontMoveCameraEndPos:X%f,Y:%f", cursor.dontMoveCameraEndPos.m128_f32[0], cursor.dontMoveCameraEndPos.m128_f32[1]);
	ImGui::Text("CursorPos:X%f,Y:%f", cursor.cursorPos.m128_f32[0], cursor.cursorPos.m128_f32[1]);
	ImGui::Text("CameraMoveValue:X%f,Y:%f", cursor.cameraMoveValue.m128_f32[0], cursor.cameraMoveValue.m128_f32[1]);
	ImGui::InputFloat("limitValue:X", &cursor.limitValue.m128_f32[0]);
	ImGui::InputFloat("limitValue:Y", &cursor.limitValue.m128_f32[1]);
	ImGui::InputFloat("NO_MOVE_DISTANCE:X", &cursor.NO_MOVE_DISTANCE.m128_f32[0]);
	ImGui::InputFloat("NO_MOVE_DISTANCE:Y", &cursor.NO_MOVE_DISTANCE.m128_f32[1]);
	ImGui::End();*/

	ImGui::Begin("Debug");
	ImGui::Checkbox("DebugCamera", &cameraChangeFlag);
	ImGui::Checkbox("DebugLine", &lineDebugFlag);
	ImGui::End();


	ImGui::Begin("StartLine");
	for (int i = 0; i < lineStartPoly.size(); ++i)
	{
		lineStartPoly[i].data.pipelineName = PIPELINE_NAME_COLOR_MULTITEX;
		std::string name = "Start" + std::to_string(i) + "X";
		ImGui::InputFloat(name.c_str(), &lineStartPoly[i].data.transform.pos.m128_f32[0]);
		name = "Start" + std::to_string(i) + "Y";
		ImGui::InputFloat(name.c_str(), &lineStartPoly[i].data.transform.pos.m128_f32[1]);
		name = "Start" + std::to_string(i) + "Z";
		ImGui::InputFloat(name.c_str(), &lineStartPoly[i].data.transform.pos.m128_f32[2]);

		lineStartPoly[i].data.transform.scale = { 0.1f,0.1f,0.1f };
		lineStartPoly[i].data.color = { 255.0f,0.0f,0.0f,255.0f };
		lineEffectArrayData[i].startPos = lineStartPoly[i].data.transform.pos + player.pos;
	}
	ImGui::End();



#pragma region カメラ挙動

	//カメラ固定
	if (goalBox.startPortalEffectFlag)
	{
		leftRightAngleVel = { -90.0f,-90.0f,0.0f };
		upDownAngleVel = { 0.0f,0.0f,0.0f };
		targetPos = { 0.0f,3.0f,0.0f };
		movieEffect.startFlag = true;
		cursor.Disappear();
	}


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

		//ポータル演出開始したらプレイヤーの動きを固定する
		XMFLOAT3 rate = { cursor.GetValue().m128_f32[0] ,cursor.GetValue().m128_f32[1],0.0f };
		honraiPlayerCameraPos.m128_f32[0] = 0.0f + (2.0f * -rate.x);
		honraiPlayerCameraPos.m128_f32[2] = 0.0f + (limitValue * rate.y) * mul;

		if (0.0f <= honraiPlayerCameraPos.m128_f32[2])
		{
			honraiPlayerCameraPos.m128_f32[2] = 0.0f;
		}

		if (goalBox.startPortalEffectFlag)
		{
			honraiPlayerCameraPos = { 0.0f,0.0f,0.0f };
		}
		XMVECTOR distance = honraiPlayerCameraPos - player.pos;
		player.pos += distance * 0.1f;
	}


	//上下左右の回転
	cameraPoly->data.transform.pos = centralPos + (besidePoly->data.transform.pos + verticlaPoly->data.transform.pos);



	if (cameraChangeFlag)
	{
		eyePos = KazMath::LoadVecotrToXMFLOAT3(cameraPoly->data.transform.pos);
		targetPos = KazMath::LoadVecotrToXMFLOAT3(baseTargetPos);
	}
	else
	{
		//デバック用
		eyePos = KazMath::CaluEyePosForDebug(eyePos, debugCameraMove, angle);
		targetPos = KazMath::CaluTargetPosForDebug(eyePos, angle.x);
	}

	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f }, 0);

#pragma endregion

	goalBox.Appear(appearGoalBoxPos);
	//敵が一通り生成終わった際に登場させる----------------------------------------------------------------
	if (changeLayerLevelMaxTime[gameStageLevel] <= gameFlame && !initAppearFlag)
	//if (100 <= gameFlame && !initAppearFlag)
	{
		goalBox.Appear(appearGoalBoxPos);
		initAppearFlag = true;
	}
	//敵が一通り生成終わった際に登場させる----------------------------------------------------------------

	changeStageFlag = false;
	//全部隠れたら次ステージの描画をする
	if (goalBox.portalEffect.AllHidden())
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


		leftRightAngleVel = { -90.0f,-90.0f,0.0f };
		trackLeftRightAngleVel = leftRightAngleVel;
		upDownAngleVel = { 0.0f,0.0f,0.0f };
		trackUpDownAngleVel = upDownAngleVel;
		movieEffect.startFlag = false;

		stageUI.Init();
		stageUI.AnnounceStage(stageNum + 1);
		cursor.Appear();
	}


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
			bool enableToUseThisDataFlag = responeData[enemyType][enemyCount].layerLevel != -1;
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

				case ENEMY_TYPE_MISILE:
					enemies[enemyType][enemyCount]->Init(responeData[enemyType][enemyCount].initPos);
					break;

				case ENEMY_TYPE_MISILE_SPLINE:
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
							lineLevel[i].Attack2(lineEffectArrayData[i].startPos, *enemyData->hitBox.center, {});
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
			//カーソルのカウント数を増やす
			cursor.Count();
			goalBox.Hit();

			//線演出をかける際にどの配列を使用するか決める
			for (int i = 0; i < lineEffectArrayData.size(); ++i)
			{
				if (!lineEffectArrayData[i].usedFlag)
				{
					lineLevel[i].Attack2(lineEffectArrayData[i].startPos, *goalBox.hitBox.center, {});
					lineEffectArrayData[i].usedFlag = true;
					lineEffectArrayData[i].lineIndex = i;
					lineEffectArrayData[i].eventType = 0;
					break;
				}
			}
		}
	}

	{
		//ロックオン判定
		bool enableToLockOnNumFlag = cursor.LockOn();
		bool enableToLockOnEnemyFlag = doneSprite.IsAlive() && !doneSprite.LockedOrNot();
		bool hitFlag = CollisionManager::Instance()->CheckRayAndSphere(cursor.hitBox, doneSprite.hitBox);
		if (hitFlag &&
			enableToLockOnEnemyFlag &&
			enableToLockOnNumFlag)
		{
			doneSprite.Hit();
			startFlag = true;
		}
		else if (hitFlag)
		{
			doneSprite.selectingFlag = true;
		}
		else
		{
			doneSprite.selectingFlag = false;
		}
	}
#pragma endregion





	//線がたどり着いたら敵を死亡させる
	for (int i = 0; i < lineEffectArrayData.size(); ++i)
	{
		if (lineEffectArrayData[i].usedFlag)
		{
			int lineIndex = lineEffectArrayData[i].lineIndex;
			int enemyTypeIndex = lineEffectArrayData[i].enemyTypeIndex;
			int enemyIndex = lineEffectArrayData[i].enemyIndex;
			int eventIndex = lineEffectArrayData[i].eventType;

			if (eventIndex == -1)
			{
				//演出を合わせてダメージと死亡をやる
				if (lineLevel[lineIndex].lineReachObjFlag && enemies[enemyTypeIndex][enemyIndex]->IsAlive())
				{
					lineEffectArrayData[i].Reset();
				}
				else if (lineLevel[lineIndex].lineReachObjFlag && !enemies[enemyTypeIndex][enemyIndex]->IsAlive())
				{
					enemies[enemyTypeIndex][enemyIndex]->Dead();
					lineEffectArrayData[i].Reset();
				}
			}
			else
			{
				//演出を合わせて死亡
				if (lineLevel[lineIndex].lineReachObjFlag && goalBox.IsAlive())
				{
					lineEffectArrayData[i].Reset();
				}
				else if (lineLevel[lineIndex].lineReachObjFlag && !goalBox.IsAlive())
				{
					bool debug = false;
					lineEffectArrayData[i].Reset();
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


#pragma endregion


	//攻撃----------------------------------------------
	for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
		{
			//生成されている、初期化している敵のみ更新処理を通す
			bool enableToUseDataFlag = enemies[enemyType][enemyCount] != nullptr && enemies[enemyType][enemyCount]->GetData()->oprationObjData->initFlag;
			if (enableToUseDataFlag)
			{
				//時間が0秒以下ならプレイヤーに当たったと判断する
				if (enemies[enemyType][enemyCount]->GetData()->timer <= 0)
				{
					player.Hit();
					enemies[enemyType][enemyCount]->Dead();
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
		if (blackTex.data.alpha <= 255.0f)
		{
			blackTex.data.alpha += 5.0f;
		}
		//タイトル画面に戻る
		else if (readyToBlackOutToGoTitleFlag && 255.0f <= blackTex.data.alpha)
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
		if (0 <= blackTex.data.alpha)
		{
			blackTex.data.alpha -= 5.0f;
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



#pragma region 更新処理

	goalBox.releaseFlag = cursor.releaseFlag;

	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_G))
	{
		stageUI.Init();
	}
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_B))
	{
		stageUI.AnnounceStage(0);
	}


	//更新処理----------------------------------------------------------------
	player.Update();
	cursor.Update();
	hitBox.Update();
	goalBox.portalEffect.sprite->data.handle = potalTexHandle;
	goalBox.Update();
	movieEffect.Update();
	stageUI.Update();
	stages[stageNum]->Update();
	doneSprite.Update();
	tutorialWindow.Update();



	//配列外を超えない処理
	if (stageNum + 1 < stages.size())
	{
		stages[stageNum + 1]->Update();
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
				XMVECTOR pos = *goalBox.hitBox.center;
				lineLevel[lineIndex].CalucurateDistance(player.pos, pos);
			}
			else
			{
				int enemyTypeIndex = lineEffectArrayData[i].enemyTypeIndex;
				int enemyIndex = lineEffectArrayData[i].enemyIndex;
				XMVECTOR pos = *enemies[enemyTypeIndex][enemyIndex]->GetData()->hitBox.center;
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





	if (1.0f <= titleT)
	{
		titleTReversFlag = false;
	}
	else if (titleT <= 0.0f)
	{
		titleTReversFlag = true;
	}

	float rate = 1.0f / 30.0f;
	if (titleTReversFlag)
	{
		titleT += rate;
	}
	else
	{
		titleT += -rate;
	}

	titleLogoTex.data.transform.pos = { 0.0f,100.0f,500.0f };
	titleLogoTex.data.transform.scale = { 0.8f,0.8f };
	baseTitlePosY = 100.0f;
	titleLogoTex.data.transform.pos.m128_f32[1] = baseTitlePosY + EasingMaker(Out, Cubic, titleT) * 50.0f;



	//ゲームループの経過時間----------------------------------------------------------------
	titleLogoTex.data.alpha = doneSprite.spriteRender.data.alpha;
	if (titleLogoTex.data.alpha <= 0.0f && !stages[0]->startFlag)
	{
		//ゲーム開始
		gameStartFlag = true;
		//ステージ番号知らせる
		stageUI.Init();
		stageUI.AnnounceStage(stageNum + 1);
		//ゲーム開始
		stages[0]->startFlag = true;
	}

	if (gameStartFlag)
	{
		++gameFlame;
	}

	//ちょっと遅らせる事で同時に起こる事柄を少なくし、混乱しないようにする
	if (60 * 4 <= gameFlame)
	{
		//チュートリアル用のUI非表示
		tutorialWindow.End();
	}

	//ゲームループの経過時間----------------------------------------------------------------


}

void Game::Draw()
{
	short mainHandle = mainRenderTarget.data.handle;


	if (!gameOverFlag)
	{
		RenderTargetStatus::Instance()->PrepareToChangeBarrier(handles[0]);
		RenderTargetStatus::Instance()->ClearRenderTarget(handles[0]);

		goalBox.Draw();
		goalBox.portalEffect.Draw();

		if (lineDebugFlag)
		{
			bg.Draw();
		}
		player.Draw();
		stages[stageNum]->SetCamera(0);
		stages[stageNum]->Draw();
		for (int i = 0; i < lineLevel.size(); ++i)
		{
			lineLevel[i].Draw();
		}

		//敵の描画処理----------------------------------------------------------------
		for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
		{
			for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
			{
				//生成されている敵のみ描画処理を通す
				bool enableToUseDataFlag = enemies[enemyType][enemyCount] != nullptr && enemies[enemyType][enemyCount]->GetData()->oprationObjData->initFlag;
				if (enableToUseDataFlag)
				{
					enemies[enemyType][enemyCount]->Draw();
				}
			}
		}
		goalBox.lightEffect.Draw();

		doneSprite.Draw();
		titleLogoTex.Draw();

		tutorialWindow.Draw();

		//輝度抽出
		RenderTargetStatus::Instance()->PrepareToChangeBarrier(addHandle, handles[0]);
		RenderTargetStatus::Instance()->ClearRenderTarget(addHandle);
		luminaceTex.Draw();
		RenderTargetStatus::Instance()->PrepareToCloseBarrier(addHandle);
		RenderTargetStatus::Instance()->SetDoubleBufferFlame(BG_COLOR);

		mainRenderTarget.Draw();
		addRenderTarget.data.handle = buler->BlurImage(addHandle);
		addRenderTarget.Draw();

		movieEffect.Draw();
		cursor.Draw();


		if (goalBox.startPortalEffectFlag)
		{
			RenderTargetStatus::Instance()->PrepareToChangeBarrier(potalTexHandle);
			RenderTargetStatus::Instance()->ClearRenderTarget(potalTexHandle);

			//float vel = subPotalSpritePos.m128_f32[2] - goalBox.portalEffect.sprite->data.transform.pos.m128_f32[2];

			//layerCameraMove.y = -vel;
			//layerLevelEyePos = KazMath::CaluEyePosForDebug(layerLevelEyePos, layerCameraMove, angle);
			//layerLevelTargetPos = KazMath::CaluTargetPosForDebug(layerLevelEyePos, angle.x);
			
			CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f }, 1);
			stages[stageNum + 1]->SetCamera(1);
			stages[stageNum + 1]->Draw();
			RenderTargetStatus::Instance()->PrepareToCloseBarrier(potalTexHandle);
			RenderTargetStatus::Instance()->SetDoubleBufferFlame(BG_COLOR);
		}
		subPotalSpritePos.m128_f32[2] = goalBox.portalEffect.sprite->data.transform.pos.m128_f32[2];
	}
	else
	{
		gameOverTex.Draw();
		if (flashFlag)
		{
			pressAButtonTex.Draw();
		}
	}

	stageUI.Draw();
	blackTex.Draw();
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
