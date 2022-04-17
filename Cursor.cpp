#include "Cursor.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"

XMFLOAT2 Cursor::KOCKBACK_MAX_VALUE = { 200.0f,200.0f };
XMFLOAT2 Cursor::KOCKBACK_VELOCITY = { 5.0f,5.0f };
XMVECTOR Cursor::NO_MOVE_DISTANCE = { 150.0f,50.0f };

Cursor::Cursor()
{
	enableLockOnTimer = 0;
	notEnableLockOnFlag = false;

	cursorFlameTex = std::make_unique<Sprite2DRender>();
	numberTex = std::make_unique<Sprite2DRender>();

	baseSpeed = 10.0f;
	flameHandle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorBase.png");
	cursorFlameTex->data.handle = flameHandle;
	cursorFlameTex->data.pipelineName = PIPELINE_NAME_SPRITE_CUTALPHA;


	numberTex->data.pipelineName = PIPELINE_NAME_SPRITE_CUTALPHA;

	//数字ハンドルの保存
	numberHandle[0] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorBase2.png");
	numberHandle[1] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorNum1.png");
	numberHandle[2] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorNum2.png");
	numberHandle[3] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorNum3.png");
	numberHandle[4] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorNum4.png");
	numberHandle[5] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorNum5.png");
	numberHandle[6] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorNum6.png");
	numberHandle[7] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorNum7.png");
	numberHandle[8] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorNumMax.png");

	cursorPos = { WIN_X / 2.0f,WIN_Y / 2.0f,0.0f };
	limitValue = { 570.0f,WIN_Y / 2.0f,0.0f };

	XMVECTOR scale = { 2.0f,2.0f };
	cursorFlameTex->data.transform.scale = scale;
	numberTex->data.transform.scale = scale;
	stopFlag = { 0,0 };

	deadLine = 0.25f;
}

void Cursor::Init()
{
	honraiCursorPos = { WIN_X / 2.0f,WIN_Y / 2.0f,0.0f };
	cursorPos = { WIN_X / 2.0f,WIN_Y / 2.0f,0.0f };

	XMVECTOR scale = { 2.0f,2.0f };
	cursorFlameTex->data.transform.scale = scale;
	numberTex->data.transform.scale = scale;
	stopFlag = { 0,0 };

	enableLockOnTimer = 0;
	notEnableLockOnFlag = false;

}

void Cursor::Input(bool UP_FLAG, bool DOWN_FLAG, bool LEFT_FLAG, bool RIGHT_FLAG, bool DONE_FLAG, bool RELEASE_FLAG, const XMVECTOR &ANGLE)
{
	upFlag = UP_FLAG;
	downFlag = DOWN_FLAG;
	leftFlag = LEFT_FLAG;
	rightFlag = RIGHT_FLAG;
	doneFlag = DONE_FLAG;
	releaseFlag = RELEASE_FLAG;

	stickAngle = ANGLE;
}

void Cursor::Update()
{
	ImGui::Begin("Curosr");
	ImGui::InputFloat("Speed", &baseSpeed);
	ImGui::InputFloat("KOCKBACK_MAX_VALUE_X", &KOCKBACK_MAX_VALUE.x);
	ImGui::InputFloat("KOCKBACK_MAX_VALUE_Y", &KOCKBACK_MAX_VALUE.y);
	ImGui::InputFloat("KOCKBACK_VELOCITY_X", &KOCKBACK_VELOCITY.x);
	ImGui::InputFloat("KOCKBACK_VELOCITY_Y", &KOCKBACK_VELOCITY.y);
	ImGui::Text("KnockBackValX%f", knockBackVal.m128_f32[0]);
	ImGui::Text("KnockBackValY%f", knockBackVal.m128_f32[1]);
	ImGui::Text("stickAngleX:%f", stickAngle.m128_f32[0]);
	ImGui::Text("stickAngleY:%f", stickAngle.m128_f32[1]);
	ImGui::InputFloat("deadLine", &deadLine);
	ImGui::End();




	//最大値固定
	if (LOCKON_MAX_NUM <= lockOnNum)
	{
		lockOnNum = 8;
	}

	//ロックオン入力がリリースされたら初期化する
	if (releaseFlag)
	{
		enableLockOnTimer = 0;
		lockOnNum = 0;
		notEnableLockOnFlag = false;
	}




	//再度ロックオンするまでの時間
	if (notEnableLockOnFlag)
	{
		++enableLockOnTimer;

		//指定のフレームまでタイマーが到達したら初期化
		if (10 <= enableLockOnTimer)
		{
			enableLockOnTimer = 0;
			notEnableLockOnFlag = false;
		}
	}
	numberTex->data.handle = numberHandle[lockOnNum];


	//何もロックオンしていない時は中心の十字を回す
	if (lockOnNum == 0)
	{
		numberTex->data.transform.rotation += 2.0f;
	}
	else
	{
		numberTex->data.transform.rotation = 0.0f;
	}


	//移動----------------------------------------------------------------
	vel = { 0.0f,0.0f,0.0f };


	prevCursorPos = cursorPos;


	if (fabs(stickAngle.m128_f32[0]) < deadLine)
	{
		stickAngle.m128_f32[0] = 0.0f;
	}
	if (fabs(stickAngle.m128_f32[1]) < deadLine)
	{
		stickAngle.m128_f32[1] = 0.0f;
	}
	speed.x = baseSpeed * -stickAngle.m128_f32[0];
	speed.y = baseSpeed * stickAngle.m128_f32[1];


	if (leftFlag)
	{
		vel.m128_f32[0] = -speed.x;
		knockBackVal.m128_f32[0] += KOCKBACK_VELOCITY.x;
		noTochFlag.x = 1;
	}
	if (rightFlag)
	{
		vel.m128_f32[0] = -speed.x;
		knockBackVal.m128_f32[0] += KOCKBACK_VELOCITY.x;
		noTochFlag.x = 1;
	}

	if (upFlag)
	{
		vel.m128_f32[1] = -speed.y;
		knockBackVal.m128_f32[1] += KOCKBACK_VELOCITY.y;
		noTochFlag.y = 1;
	}
	if (downFlag)
	{
		vel.m128_f32[1] = -speed.y;
		knockBackVal.m128_f32[1] += KOCKBACK_VELOCITY.y;
		noTochFlag.y = 1;
	}

	//ノックバックの制限
	if (KOCKBACK_MAX_VALUE.x <= knockBackVal.m128_f32[0])
	{
		knockBackVal.m128_f32[0] = KOCKBACK_MAX_VALUE.x;
	}
	if (KOCKBACK_MAX_VALUE.y <= knockBackVal.m128_f32[1])
	{
		knockBackVal.m128_f32[1] = KOCKBACK_MAX_VALUE.y;
	}


	if (!upFlag && !downFlag)
	{
		noTochFlag.y = 0;
	}
	if (!leftFlag && !rightFlag)
	{
		noTochFlag.x = 0;
	}

	if (!noTochFlag.x)
	{
		++noTockTimer.x;
	}
	else
	{
		noTockTimer.x = 0;
		oldVel.m128_f32[0] = vel.m128_f32[0];
	}


	if (!noTochFlag.y)
	{
		++noTockTimer.y;
	}
	else
	{
		noTockTimer.y = 0;
		oldVel.m128_f32[1] = vel.m128_f32[1];
	}


	//ノックバックの処理
	if (5 <= noTockTimer.x && false)
	{
		if (signbit(oldVel.m128_f32[0]))
		{
			vel.m128_f32[0] = knockBackVal.m128_f32[0];
		}
		else
		{
			vel.m128_f32[0] = -knockBackVal.m128_f32[0];
		}
		knockBackVal.m128_f32[0] = 0.0f;
	}
	if (5 <= noTockTimer.y && false)
	{
		if (signbit(oldVel.m128_f32[1]))
		{
			vel.m128_f32[1] = knockBackVal.m128_f32[1];
		}
		else
		{
			vel.m128_f32[1] = -knockBackVal.m128_f32[1];
		}
		knockBackVal.m128_f32[1] = 0.0f;
	}

	honraiCursorPos += vel;

	//画面外から出ないようにする処理
	if (honraiCursorPos.m128_f32[0] < 0.0f)
	{
		honraiCursorPos.m128_f32[0] = 0.0f;
	}
	if (WIN_X < honraiCursorPos.m128_f32[0])
	{
		honraiCursorPos.m128_f32[0] = WIN_X;
	}

	if (honraiCursorPos.m128_f32[1] < 0.0f)
	{
		honraiCursorPos.m128_f32[1] = 0.0f;
	}
	if (WIN_Y < honraiCursorPos.m128_f32[1])
	{
		honraiCursorPos.m128_f32[1] = WIN_Y;
	}


	//補間
	XMVECTOR distance = honraiCursorPos - cursorPos;
	cursorPos += distance * 0.5f;

	cursorFlameTex->data.transform.pos = { cursorPos.m128_f32[0],cursorPos.m128_f32[1] };
	numberTex->data.transform.pos = { cursorPos.m128_f32[0],cursorPos.m128_f32[1] };
	//移動----------------------------------------------------------------


	//カメラ無操作-----------------------
	//追尾
	{
		//カーソルの位置が始点と終点以内だった場合、無操作範囲の追尾をやめてカメラ移動量を追加しない
		if (dontMoveCameraStartPos.m128_f32[0] < cursorPos.m128_f32[0] &&
			cursorPos.m128_f32[0] < dontMoveCameraEndPos.m128_f32[0])
		{

		}
		//カーソル外に出ようとしたら無操作範囲の追尾を開始する
		else
		{
			{
				float cursorStartDistanceX = dontMoveCameraStartPos.m128_f32[0] - cursorPos.m128_f32[0];
				float cursorEndDistanceX = dontMoveCameraEndPos.m128_f32[0] - cursorPos.m128_f32[0];
				//終点より始点の方が近かったら始点を基準に動かす
				if (fabs(cursorStartDistanceX) < fabs(cursorEndDistanceX))
				{
					dontMoveCameraStartPos.m128_f32[0] = cursorPos.m128_f32[0];
					dontMoveCameraEndPos.m128_f32[0] = cursorPos.m128_f32[0] + NO_MOVE_DISTANCE.m128_f32[0];
				}
				//始点より終点の方が近かったら終点を基準に動かす
				else
				{
					dontMoveCameraStartPos.m128_f32[0] = cursorPos.m128_f32[0] - NO_MOVE_DISTANCE.m128_f32[0];
					dontMoveCameraEndPos.m128_f32[0] = cursorPos.m128_f32[0];
				}
			}
			//移動量をカメラに追加していく
			honraiCameraMoveValue.m128_f32[0] += prevCursorPos.m128_f32[0] - cursorPos.m128_f32[0];
		}

		if (dontMoveCameraStartPos.m128_f32[1] < cursorPos.m128_f32[1] &&
			cursorPos.m128_f32[1] < dontMoveCameraEndPos.m128_f32[1])
		{

		}
		//カーソル外に出ようとしたら無操作範囲の追尾を開始する
		else
		{
			{
				float cursorStartDistanceY = dontMoveCameraStartPos.m128_f32[1] - cursorPos.m128_f32[1];
				float cursorEndDistanceY = dontMoveCameraEndPos.m128_f32[1] - cursorPos.m128_f32[1];
				//終点より始点の方が近かったら始点を基準に動かす
				if (fabs(cursorStartDistanceY) < fabs(cursorEndDistanceY))
				{
					dontMoveCameraStartPos.m128_f32[1] = cursorPos.m128_f32[1];
					dontMoveCameraEndPos.m128_f32[1] = cursorPos.m128_f32[1] + NO_MOVE_DISTANCE.m128_f32[1];
				}
				//始点より終点の方が近かったら終点を基準に動かす
				else
				{
					dontMoveCameraStartPos.m128_f32[1] = cursorPos.m128_f32[1] - NO_MOVE_DISTANCE.m128_f32[1];
					dontMoveCameraEndPos.m128_f32[1] = cursorPos.m128_f32[1];
				}
			}
			//移動量をカメラに追加していく
			honraiCameraMoveValue.m128_f32[1] += prevCursorPos.m128_f32[1] - cursorPos.m128_f32[1];
		}
	}

	//カメラの制限
	if (honraiCursorPos.m128_f32[0] <= 0.0f)
	{
		honraiCameraMoveValue.m128_f32[0] = limitValue.m128_f32[0];
	}
	if (WIN_X <= honraiCursorPos.m128_f32[0])
	{
		honraiCameraMoveValue.m128_f32[0] = -limitValue.m128_f32[0];
	}

	if (honraiCursorPos.m128_f32[1] <= 0.0f)
	{
		honraiCameraMoveValue.m128_f32[1] = limitValue.m128_f32[1] - NO_MOVE_DISTANCE.m128_f32[1];
	}
	if (WIN_Y <= honraiCursorPos.m128_f32[1])
	{
		honraiCameraMoveValue.m128_f32[1] = -limitValue.m128_f32[1];
	}

	XMVECTOR cameraDistance = honraiCameraMoveValue - cameraMoveValue;
	cameraMoveValue += cameraDistance * 0.5f;

	//カメラ無操作-----------------------




	//当たり判定----------------------------------------------------------------
	XMVECTOR nearPos = { cursorPos.m128_f32[0],cursorPos.m128_f32[1],0.0f,0.0f };
	XMVECTOR farPos = { cursorPos.m128_f32[0],cursorPos.m128_f32[1],1.0f,0.0f };
	XMVECTOR endPos = KazMath::ConvertScreenPosToWorldPos(farPos, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());

	hitBox.start = KazMath::ConvertScreenPosToWorldPos(nearPos, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());
	hitBox.dir = XMVector3Normalize(endPos - hitBox.start);
	//当たり判定----------------------------------------------------------------

}

void Cursor::Draw()
{
	numberTex->Draw();
	cursorFlameTex->Draw();
}

bool Cursor::LockOn()
{
	//ロックオン数が最大値未満であること
	bool checkLockOnNumFlag = lockOnNum < LOCKON_MAX_NUM;
	//ロックオンが有効でロックオンの入力がされている
	bool enableToLockOnFlag = !notEnableLockOnFlag && doneFlag;

	//この二つの条件が揃うなら有効
	return checkLockOnNumFlag && enableToLockOnFlag;
}

bool Cursor::Release()
{
	return releaseFlag;
}

void Cursor::Count()
{
	++lockOnNum;
	notEnableLockOnFlag = true;
}

const int &Cursor::GetCount()
{
	return lockOnNum;
}

const XMVECTOR &Cursor::GetValue()
{
	//画面中央を0,0の中心座標とする
	XMVECTOR rateMaxValue = { WIN_X / 2.0f,WIN_Y / 2.0f };
	//割合計算
	XMVECTOR value = cameraMoveValue / rateMaxValue;
	//カメラの座標に合わせる為にX軸の符号を反転
	value.m128_f32[0] *= -1.0f;
	return value;
}
