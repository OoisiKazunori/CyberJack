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

	//�����n���h���̕ۑ�
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
	baseScale = { 2.0f,2.0f };
	deadLine = 0.25f;


	for (int i = 0; i < cursorEffectTex.size(); ++i)
	{
		cursorEffectTex[i].cursorEffectTex->data.handle = flameHandle;
		cursorEffectTex[i].cursorEffectTex->data.pipelineName = PIPELINE_NAME_SPRITE;
	}
}

void Cursor::Init()
{
	honraiCursorPos = { WIN_X / 2.0f,WIN_Y / 2.0f,0.0f };
	cursorPos = { WIN_X / 2.0f,WIN_Y / 2.0f,0.0f };

	cursorFlameTex->data.transform.scale = baseScale;
	numberTex->data.transform.scale = baseScale;
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

	//�ő�l�Œ�
	if (LOCKON_MAX_NUM <= lockOnNum)
	{
		lockOnNum = 8;
	}

	//���b�N�I�����͂������[�X���ꂽ�珉��������
	if (releaseFlag)
	{
		enableLockOnTimer = 0;
		lockOnNum = 0;
		notEnableLockOnFlag = false;
	}




	//�ēx���b�N�I������܂ł̎���
	if (notEnableLockOnFlag)
	{
		++enableLockOnTimer;

		//�w��̃t���[���܂Ń^�C�}�[�����B�����珉����
		if (10 <= enableLockOnTimer)
		{
			enableLockOnTimer = 0;
			notEnableLockOnFlag = false;
		}
	}
	numberTex->data.handle = numberHandle[lockOnNum];


	//�������b�N�I�����Ă��Ȃ����͒��S�̏\������
	if (lockOnNum == 0)
	{
		numberTex->data.transform.rotation += 2.0f;
	}
	else
	{
		numberTex->data.transform.rotation = 0.0f;
	}


	//�ړ�----------------------------------------------------------------
	vel = { 0.0f,0.0f,0.0f };


	prevCursorPos = cursorPos;

	XMVECTOR honraiStick = stickAngle;
	//�f�b�g���C��-----------------------
	if (fabs(stickAngle.m128_f32[0]) < deadLine)
	{
		stickAngle.m128_f32[0] = 0.0f;
	}
	else
	{
		stickAngle.m128_f32[0] += -deadLine;
	}
	if (fabs(stickAngle.m128_f32[1]) < deadLine)
	{
		stickAngle.m128_f32[1] = 0.0f;
	}
	else
	{
		stickAngle.m128_f32[1] += -deadLine;
	}
	//�f�b�g���C��-----------------------


	if (1.0f - deadLine <= fabs(stickAngle.m128_f32[0]))
	{
		bool flag = signbit(stickAngle.m128_f32[0]);
		if (flag)
		{
			stickAngle.m128_f32[0] = -1.0f + deadLine;
		}
		else
		{
			stickAngle.m128_f32[0] = 1.0f - deadLine;
		}
	}
	if (1.0f - deadLine <= fabs(stickAngle.m128_f32[1]))
	{
		bool flag = signbit(stickAngle.m128_f32[1]);
		if (flag)
		{
			stickAngle.m128_f32[1] = -1.0f + deadLine;
		}
		else
		{
			stickAngle.m128_f32[1] = 1.0f - deadLine;
		}
	}


	//�X�e�B�b�N�̑��삩��J�[�\���̃X�s�[�h�𒲐�
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

	//�m�b�N�o�b�N�̐���
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


	//�m�b�N�o�b�N�̏���
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

	//��ʊO����o�Ȃ��悤�ɂ��鏈��
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


	//���
	XMVECTOR distance = honraiCursorPos - cursorPos;
	cursorPos += distance * 0.5f;

	cursorFlameTex->data.transform.pos = { cursorPos.m128_f32[0],cursorPos.m128_f32[1] };
	numberTex->data.transform.pos = { cursorPos.m128_f32[0],cursorPos.m128_f32[1] };
	//�ړ�----------------------------------------------------------------


	//�J����������-----------------------
	//�ǔ�
	{
		//�J�[�\���̈ʒu���n�_�ƏI�_�ȓ��������ꍇ�A������͈͂̒ǔ�����߂ăJ�����ړ��ʂ�ǉ����Ȃ�
		if (dontMoveCameraStartPos.m128_f32[0] < cursorPos.m128_f32[0] &&
			cursorPos.m128_f32[0] < dontMoveCameraEndPos.m128_f32[0])
		{

		}
		//�J�[�\���O�ɏo�悤�Ƃ����疳����͈͂̒ǔ����J�n����
		else
		{
			{
				float cursorStartDistanceX = dontMoveCameraStartPos.m128_f32[0] - cursorPos.m128_f32[0];
				float cursorEndDistanceX = dontMoveCameraEndPos.m128_f32[0] - cursorPos.m128_f32[0];
				//�I�_���n�_�̕����߂�������n�_����ɓ�����
				if (fabs(cursorStartDistanceX) < fabs(cursorEndDistanceX))
				{
					dontMoveCameraStartPos.m128_f32[0] = cursorPos.m128_f32[0];
					dontMoveCameraEndPos.m128_f32[0] = cursorPos.m128_f32[0] + NO_MOVE_DISTANCE.m128_f32[0];
				}
				//�n�_���I�_�̕����߂�������I�_����ɓ�����
				else
				{
					dontMoveCameraStartPos.m128_f32[0] = cursorPos.m128_f32[0] - NO_MOVE_DISTANCE.m128_f32[0];
					dontMoveCameraEndPos.m128_f32[0] = cursorPos.m128_f32[0];
				}
			}
			//�ړ��ʂ��J�����ɒǉ����Ă���
			honraiCameraMoveValue.m128_f32[0] += prevCursorPos.m128_f32[0] - cursorPos.m128_f32[0];
		}

		if (dontMoveCameraStartPos.m128_f32[1] < cursorPos.m128_f32[1] &&
			cursorPos.m128_f32[1] < dontMoveCameraEndPos.m128_f32[1])
		{

		}
		//�J�[�\���O�ɏo�悤�Ƃ����疳����͈͂̒ǔ����J�n����
		else
		{
			{
				float cursorStartDistanceY = dontMoveCameraStartPos.m128_f32[1] - cursorPos.m128_f32[1];
				float cursorEndDistanceY = dontMoveCameraEndPos.m128_f32[1] - cursorPos.m128_f32[1];
				//�I�_���n�_�̕����߂�������n�_����ɓ�����
				if (fabs(cursorStartDistanceY) < fabs(cursorEndDistanceY))
				{
					dontMoveCameraStartPos.m128_f32[1] = cursorPos.m128_f32[1];
					dontMoveCameraEndPos.m128_f32[1] = cursorPos.m128_f32[1] + NO_MOVE_DISTANCE.m128_f32[1];
				}
				//�n�_���I�_�̕����߂�������I�_����ɓ�����
				else
				{
					dontMoveCameraStartPos.m128_f32[1] = cursorPos.m128_f32[1] - NO_MOVE_DISTANCE.m128_f32[1];
					dontMoveCameraEndPos.m128_f32[1] = cursorPos.m128_f32[1];
				}
			}
			//�ړ��ʂ��J�����ɒǉ����Ă���
			honraiCameraMoveValue.m128_f32[1] += prevCursorPos.m128_f32[1] - cursorPos.m128_f32[1];
		}
	}

	//�J�����̐���
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

	//�J����������-----------------------

	//�J�[�\�����o
	for (int i = 0; i < cursorEffectTex.size(); ++i)
	{
		if (releaseFlag && !cursorEffectTex[i].initFlag)
		{
			cursorEffectTex[i].cursorEffectTex->data.transform.pos = cursorPos;
			cursorEffectTex[i].cursorEffectTex->data.alpha = 255.0f;
			cursorEffectTex[i].cursorEffectTex->data.transform.scale = baseScale;
			cursorEffectTex[i].initFlag = true;
			break;
		}
	}

	for (int i = 0; i < cursorEffectTex.size(); ++i)
	{
		if (cursorEffectTex[i].initFlag)
		{
			cursorEffectTex[i].cursorEffectTex->data.alpha -= 255.0f / 10.0f;
			XMVECTOR addSize = { 0.1f,0.1f };
			cursorEffectTex[i].cursorEffectTex->data.transform.scale += addSize;
		}
		if (cursorEffectTex[i].cursorEffectTex->data.alpha <= 0.0f)
		{
			cursorEffectTex[i].initFlag = false;
		}
	}


	//�����蔻��----------------------------------------------------------------
	XMVECTOR nearPos = { cursorPos.m128_f32[0],cursorPos.m128_f32[1],0.0f,0.0f };
	XMVECTOR farPos = { cursorPos.m128_f32[0],cursorPos.m128_f32[1],1.0f,0.0f };
	XMVECTOR endPos = KazMath::ConvertScreenPosToWorldPos(farPos, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());

	hitBox.start = KazMath::ConvertScreenPosToWorldPos(nearPos, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());
	hitBox.dir = XMVector3Normalize(endPos - hitBox.start);
	//�����蔻��----------------------------------------------------------------


	ImGui::Begin("Curosr");
	ImGui::InputFloat("Speed", &baseSpeed);
	ImGui::InputFloat("KOCKBACK_MAX_VALUE_X", &KOCKBACK_MAX_VALUE.x);
	ImGui::InputFloat("KOCKBACK_MAX_VALUE_Y", &KOCKBACK_MAX_VALUE.y);
	ImGui::InputFloat("KOCKBACK_VELOCITY_X", &KOCKBACK_VELOCITY.x);
	ImGui::InputFloat("KOCKBACK_VELOCITY_Y", &KOCKBACK_VELOCITY.y);
	ImGui::InputFloat("deadLine", &deadLine);
	ImGui::Text("NowSpeed:%f", speed);
	ImGui::Text("MaxSpeed:%f", baseSpeed * (1.0 - deadLine));
	ImGui::Text("KnockBackValX%f", knockBackVal.m128_f32[0]);
	ImGui::Text("KnockBackValY%f", knockBackVal.m128_f32[1]);
	ImGui::Text("stickAngleX:%f", honraiStick.m128_f32[0]);
	ImGui::Text("stickAngleY:%f", honraiStick.m128_f32[1]);
	ImGui::End();

}

void Cursor::Draw()
{
	numberTex->Draw();
	cursorFlameTex->Draw();

	for (int i = 0; i < cursorEffectTex.size(); ++i)
	{
		if (cursorEffectTex[i].initFlag)
		{
			cursorEffectTex[i].cursorEffectTex->Draw();
		}
	}
}

bool Cursor::LockOn()
{
	//���b�N�I�������ő�l�����ł��邱��
	bool checkLockOnNumFlag = lockOnNum < LOCKON_MAX_NUM;
	//���b�N�I�����L���Ń��b�N�I���̓��͂�����Ă���
	bool enableToLockOnFlag = !notEnableLockOnFlag && doneFlag;

	//���̓�̏����������Ȃ�L��
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
	//��ʒ�����0,0�̒��S���W�Ƃ���
	XMVECTOR rateMaxValue = { WIN_X / 2.0f,WIN_Y / 2.0f };
	//�����v�Z
	XMVECTOR value = cameraMoveValue / rateMaxValue;
	//�J�����̍��W�ɍ��킹��ׂ�X���̕����𔽓]
	value.m128_f32[0] *= -1.0f;
	return value;
}
