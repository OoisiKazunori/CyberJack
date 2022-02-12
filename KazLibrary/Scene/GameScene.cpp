#include "GameScene.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Imgui/MyImgui.h"

GameScene::GameScene()
{
}

GameScene::~GameScene() {
}

void GameScene::Init()
{
	player.Init(KazMath::Transform3D().pos);
	cursor.Init();

	hitBox.Init({ 0.0f,0.0f,30.0f }, 1);

	sceneNum = SCENE_NONE;

	enemy[0] = std::make_unique<NormalEnemy>();
	enemy[0]->Init({ 0.0f,0.0f,30.0f });
}

void GameScene::Finalize()
{
}

void GameScene::Input()
{
	KeyBoradInputManager *input = KeyBoradInputManager::Instance();

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
	eyePos = KazMath::CaluEyePosForDebug(eyePos, debugCameraMove, angle);
	targetPos = KazMath::CaluTargetPosForDebug(eyePos, angle.x);

#pragma endregion


	cursor.Input
	(
		input->InputState(DIK_T),
		input->InputState(DIK_G),
		input->InputState(DIK_F),
		input->InputState(DIK_H),
		input->InputState(DIK_RETURN),
		input->InputRelease(DIK_RETURN)
	);
}

void GameScene::Update()
{
	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f });

#pragma region ロックオン
	//ロックオン判定
	bool enableToLockOnNumFlag = cursor.LockOn();
	bool enableToLockOnEnemyFlag = hitBox.AliveOrNot() && !hitBox.LockedOrNot();
	if (CollisionManager::Instance()->CheckRayAndSphere(cursor.hitBox, hitBox.sphere) &&
		enableToLockOnNumFlag &&
		enableToLockOnEnemyFlag)
	{
		//カーソルのカウント数を増やす
		cursor.Count();
		//敵が当たった情報を書く
		hitBox.Hit();
		//線演出をかける際にどの配列を使用するか決める
	}

	//敵が生きている&&ロックオンできる回数が0以下&&ロックオン入力がリリースされた時、敵は死亡する
	if (hitBox.AliveOrNot() && hitBox.LockedOrNot() && cursor.Release())
	{
		hitBox.Dead();
	}
#pragma endregion

	player.Update();
	cursor.Update();
	hitBox.Update();

	enemy[0]->GetData();
}

void GameScene::Draw()
{
	bg.Draw();
	player.Draw();
	cursor.Draw();
	//hitBox.Draw();

	enemy[0]->Draw();
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