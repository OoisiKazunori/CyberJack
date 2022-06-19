#include "Cursor.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Sound/SoundManager.h"

XMFLOAT2 Cursor::KOCKBACK_MAX_VALUE = { 200.0f,200.0f };
XMFLOAT2 Cursor::KOCKBACK_VELOCITY = { 5.0f,5.0f };
KazMath::Vec2<float> Cursor::NO_MOVE_DISTANCE = { 150.0f,50.0f };

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
	//cursorFlameTex->data.pipelineName = PIPELINE_NAME_SPRITE_MULTITEX;


	//numberTex->data.pipelineName = PIPELINE_NAME_SPRITE_CUTALPHA;
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

	cursorPos = { static_cast<float>(WIN_X) / 2.0f,static_cast<float>(WIN_Y) / 2.0f };
	limitValue = { 570.0f,static_cast<float>(WIN_Y) / 2.0f };

	KazMath::Vec2<float> scale = { 2.0f,2.0f };
	cursorFlameTex->data.transform.scale = scale;
	numberTex->data.transform.scale = scale;
	stopFlag = { 0,0 };
	baseScale = { 2.0f,2.0f };
	deadLine = 0.25f;


	for (int i = 0; i < cursorEffectTex.size(); ++i)
	{
		cursorEffectTex[i].cursorEffectTex->data.handle = flameHandle;
		cursorEffectTex[i].cursorEffectTex->data.pipelineName = PIPELINE_NAME_SPRITE_MULTITEX;
	}

	clickSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "Push.wav", false);
	initClickSoundFlag = false;
}

void Cursor::Init()
{
	honraiCursorPos = { static_cast<float>(WIN_X) / 2.0f,static_cast<float>(WIN_Y) / 2.0f };
	cursorPos = { static_cast<float>(WIN_X) / 2.0f,static_cast<float>(WIN_Y) / 2.0f };

	cursorFlameTex->data.transform.scale = baseScale;
	numberTex->data.transform.scale = baseScale;
	stopFlag = { 0,0 };

	enableLockOnTimer = 0;
	notEnableLockOnFlag = false;

}

void Cursor::Input(bool UP_FLAG, bool DOWN_FLAG, bool LEFT_FLAG, bool RIGHT_FLAG, bool DONE_FLAG, bool RELEASE_FLAG, const KazMath::Vec2<float> &ANGLE)
{
	if (!dontMoveFlag)
	{
		upFlag = UP_FLAG;
		downFlag = DOWN_FLAG;
		leftFlag = LEFT_FLAG;
		rightFlag = RIGHT_FLAG;
		doneFlag = DONE_FLAG;
		releaseFlag = RELEASE_FLAG;
		stickAngle = ANGLE;
	}
	else
	{
		upFlag = false;
		downFlag = false;
		leftFlag = false;
		rightFlag = false;
		doneFlag = false;
		releaseFlag = false;
		stickAngle = { 0.0f,0.0f };
	}
}

void Cursor::Update()
{
	//�ő�l�Œ�
	if (LOCKON_MAX_NUM <= lockOnNum)
	{
		lockOnNum = 8;
	}

	if (doneFlag && !initClickSoundFlag)
	{
		SoundManager::Instance()->PlaySoundMem(clickSoundHandle, 1);
		initClickSoundFlag = true;
	}

	//���b�N�I�����͂������[�X���ꂽ�珉��������
	if (releaseFlag)
	{
		enableLockOnTimer = 0;
		lockOnNum = 0;
		notEnableLockOnFlag = false;
		initClickSoundFlag = false;
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
	vel = { 0.0f,0.0f };


	prevCursorPos = cursorPos;

	KazMath::Vec2<float> honraiStick = stickAngle;
	//�f�b�g���C��-----------------------
	if (fabs(stickAngle.x) < deadLine)
	{
		stickAngle.x = 0.0f;
	}
	else
	{
		stickAngle.x += -deadLine;
	}
	if (fabs(stickAngle.y) < deadLine)
	{
		stickAngle.y = 0.0f;
	}
	else
	{
		stickAngle.y += -deadLine;
	}
	//�f�b�g���C��-----------------------


	if (1.0f - deadLine <= fabs(stickAngle.x))
	{
		bool flag = signbit(stickAngle.x);
		if (flag)
		{
			stickAngle.x = -1.0f + deadLine;
		}
		else
		{
			stickAngle.x = 1.0f - deadLine;
		}
	}
	if (1.0f - deadLine <= fabs(stickAngle.y))
	{
		bool flag = signbit(stickAngle.y);
		if (flag)
		{
			stickAngle.y = -1.0f + deadLine;
		}
		else
		{
			stickAngle.y = 1.0f - deadLine;
		}
	}


	//�X�e�B�b�N�̑��삩��J�[�\���̃X�s�[�h�𒲐�
	speed.x = baseSpeed * -stickAngle.x;
	speed.y = baseSpeed * stickAngle.y;


	if (leftFlag)
	{
		vel.x = -speed.x;
		knockBackVal.x += KOCKBACK_VELOCITY.x;
		noTochFlag.x = 1;
	}
	if (rightFlag)
	{
		vel.x = -speed.x;
		knockBackVal.x += KOCKBACK_VELOCITY.x;
		noTochFlag.x = 1;
	}

	if (upFlag)
	{
		vel.y = -speed.y;
		knockBackVal.y += KOCKBACK_VELOCITY.y;
		noTochFlag.y = 1;
	}
	if (downFlag)
	{
		vel.y = -speed.y;
		knockBackVal.y += KOCKBACK_VELOCITY.y;
		noTochFlag.y = 1;
	}

	//�m�b�N�o�b�N�̐���
	if (KOCKBACK_MAX_VALUE.x <= knockBackVal.x)
	{
		knockBackVal.x = KOCKBACK_MAX_VALUE.x;
	}
	if (KOCKBACK_MAX_VALUE.y <= knockBackVal.y)
	{
		knockBackVal.y = KOCKBACK_MAX_VALUE.y;
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
		oldVel.x = vel.x;
	}


	if (!noTochFlag.y)
	{
		++noTockTimer.y;
	}
	else
	{
		noTockTimer.y = 0;
		oldVel.y = vel.y;
	}


	//�m�b�N�o�b�N�̏���
	if (5 <= noTockTimer.x && false)
	{
		if (signbit(oldVel.x))
		{
			vel.x = knockBackVal.x;
		}
		else
		{
			vel.x = -knockBackVal.x;
		}
		knockBackVal.x = 0.0f;
	}
	if (5 <= noTockTimer.y && false)
	{
		if (signbit(oldVel.y))
		{
			vel.y = knockBackVal.y;
		}
		else
		{
			vel.y = -knockBackVal.y;
		}
		knockBackVal.y = 0.0f;
	}

	honraiCursorPos += vel;

	//��ʊO����o�Ȃ��悤�ɂ��鏈��
	if (honraiCursorPos.x < 0.0f)
	{
		honraiCursorPos.x = 0.0f;
	}
	if (WIN_X < honraiCursorPos.x)
	{
		honraiCursorPos.x = WIN_X;
	}

	if (honraiCursorPos.y < 0.0f)
	{
		honraiCursorPos.y = 0.0f;
	}
	if (WIN_Y < honraiCursorPos.y)
	{
		honraiCursorPos.y = WIN_Y;
	}


	//���
	KazMath::Vec2<float> distance = honraiCursorPos - cursorPos;
	cursorPos += distance * 0.5f;

	cursorFlameTex->data.transform.pos = { cursorPos.x,cursorPos.y };
	numberTex->data.transform.pos = { cursorPos.x,cursorPos.y };
	//�ړ�----------------------------------------------------------------


	//�J����������-----------------------
	//�ǔ�
	{
		//�J�[�\���̈ʒu���n�_�ƏI�_�ȓ��������ꍇ�A������͈͂̒ǔ�����߂ăJ�����ړ��ʂ�ǉ����Ȃ�
		if (dontMoveCameraStartPos.x < cursorPos.x &&
			cursorPos.x < dontMoveCameraEndPos.x)
		{

		}
		//�J�[�\���O�ɏo�悤�Ƃ����疳����͈͂̒ǔ����J�n����
		else
		{
			{
				float cursorStartDistanceX = dontMoveCameraStartPos.x - cursorPos.x;
				float cursorEndDistanceX = dontMoveCameraEndPos.x - cursorPos.x;
				//�I�_���n�_�̕����߂�������n�_����ɓ�����
				if (fabs(cursorStartDistanceX) < fabs(cursorEndDistanceX))
				{
					dontMoveCameraStartPos.x = cursorPos.x;
					dontMoveCameraEndPos.x = cursorPos.x + NO_MOVE_DISTANCE.x;
				}
				//�n�_���I�_�̕����߂�������I�_����ɓ�����
				else
				{
					dontMoveCameraStartPos.x = cursorPos.x - NO_MOVE_DISTANCE.x;
					dontMoveCameraEndPos.x = cursorPos.x;
				}
			}
			//�ړ��ʂ��J�����ɒǉ����Ă���
			honraiCameraMoveValue.x += prevCursorPos.x - cursorPos.x;
		}

		if (dontMoveCameraStartPos.y < cursorPos.y &&
			cursorPos.y < dontMoveCameraEndPos.y)
		{

		}
		//�J�[�\���O�ɏo�悤�Ƃ����疳����͈͂̒ǔ����J�n����
		else
		{
			{
				float cursorStartDistanceY = dontMoveCameraStartPos.y - cursorPos.y;
				float cursorEndDistanceY = dontMoveCameraEndPos.y - cursorPos.y;
				//�I�_���n�_�̕����߂�������n�_����ɓ�����
				if (fabs(cursorStartDistanceY) < fabs(cursorEndDistanceY))
				{
					dontMoveCameraStartPos.y = cursorPos.y;
					dontMoveCameraEndPos.y = cursorPos.y + NO_MOVE_DISTANCE.y;
				}
				//�n�_���I�_�̕����߂�������I�_����ɓ�����
				else
				{
					dontMoveCameraStartPos.y = cursorPos.y - NO_MOVE_DISTANCE.y;
					dontMoveCameraEndPos.y = cursorPos.y;
				}
			}
			//�ړ��ʂ��J�����ɒǉ����Ă���
			honraiCameraMoveValue.y += prevCursorPos.y - cursorPos.y;
		}
	}


	//�J�[�\��������----------------------------------------------
	if (disappearFlag)
	{
		int sub = 5;
		cursorAlpha -= sub;
		cursorFlameTex->data.pipelineName = PIPELINE_NAME_SPRITE_MULTITEX;
		//���S�ɏ�������J�[�\���̈ʒu������������----------------------------------------------
		//�J�[�\�����܂��������Ȃ�
		if (cursorAlpha <= 0)
		{
			dontMoveFlag = true;
			honraiCameraMoveValue = { 0.0f,0.0f };
			honraiCursorPos = { WIN_X / 2.0f,WIN_Y / 2.0f };
			cursorPos = honraiCursorPos;
			prevCursorPos = honraiCursorPos;
			dontMoveCameraStartPos = { 490.0f,310.0f };
			dontMoveCameraEndPos = { 640.0f,360.0f };
		}
		//���S�ɏ�������J�[�\���̈ʒu������������----------------------------------------------
	}
	else
	{
		cursorAlpha = 255;
		dontMoveFlag = false;
		cursorFlameTex->data.pipelineName = PIPELINE_NAME_SPRITE_CUTALPHA;
	}

	numberTex->data.alpha = static_cast<float>(cursorAlpha);
	cursorFlameTex->data.alpha = static_cast<float>(cursorAlpha);
	//�J�[�\��������----------------------------------------------




	//�J�����̐���
	if (honraiCursorPos.x <= 0.0f)
	{
		float localLimit = 485.0f;
		honraiCameraMoveValue.x = localLimit;
	}
	if (WIN_X <= honraiCursorPos.x)
	{
		honraiCameraMoveValue.x = -limitValue.x;
	}

	if (honraiCursorPos.y <= 0.0f)
	{
		honraiCameraMoveValue.y = limitValue.y - NO_MOVE_DISTANCE.y;
	}
	if (WIN_Y <= honraiCursorPos.y)
	{
		honraiCameraMoveValue.y = -limitValue.y;
	}

	KazMath::Vec2<float> cameraDistance = honraiCameraMoveValue - cameraMoveValue;
	cameraMoveValue += cameraDistance * 0.5f;

	//�J����������-----------------------

	//�J�[�\�����o
	for (int i = 0; i < cursorEffectTex.size(); ++i)
	{
		if (releaseFlag && !cursorEffectTex[i].initFlag && !disappearFlag)
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
			KazMath::Vec2<float> addSize = { 0.1f,0.1f };
			cursorEffectTex[i].cursorEffectTex->data.transform.scale += addSize;
		}
		if (cursorEffectTex[i].cursorEffectTex->data.alpha <= 0.0f)
		{
			cursorEffectTex[i].initFlag = false;
		}
	}


	//�����蔻��----------------------------------------------------------------
	KazMath::Vec3<float> nearPos = { cursorPos.x,cursorPos.y,0.0f };
	KazMath::Vec3<float> farPos = { cursorPos.x,cursorPos.y,1.0f };
	KazMath::Vec3<float> endPos = KazMath::ConvertScreenPosToWorldPos(farPos, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());

	hitBox.start = KazMath::ConvertScreenPosToWorldPos(nearPos, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());
	KazMath::Vec3<float> dir(endPos - hitBox.start);
	dir.Normalize();
	hitBox.dir = dir;
	//�����蔻��----------------------------------------------------------------


	/*ImGui::Begin("Curosr");
	ImGui::InputFloat("Speed", &baseSpeed);
	ImGui::InputFloat("KOCKBACK_MAX_VALUE_X", &KOCKBACK_MAX_VALUE.x);
	ImGui::InputFloat("KOCKBACK_MAX_VALUE_Y", &KOCKBACK_MAX_VALUE.y);
	ImGui::InputFloat("KOCKBACK_VELOCITY_X", &KOCKBACK_VELOCITY.x);
	ImGui::InputFloat("KOCKBACK_VELOCITY_Y", &KOCKBACK_VELOCITY.y);
	ImGui::InputFloat("deadLine", &deadLine);
	ImGui::Text("NowSpeed:%f", speed);
	ImGui::Text("MaxSpeed:%f", baseSpeed * (1.0 - deadLine));
	ImGui::Text("KnockBackValX%f", knockBackVal.x);
	ImGui::Text("KnockBackValY%f", knockBackVal.y);
	ImGui::Text("stickAngleX:%f", honraiStick.x);
	ImGui::Text("stickAngleY:%f", honraiStick.y);
	ImGui::End();*/

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

KazMath::Vec2<float> Cursor::GetValue()
{
	//��ʒ�����0,0�̒��S���W�Ƃ���
	KazMath::Vec2<float> rateMaxValue = { WIN_X / 2.0f,WIN_Y / 2.0f };
	//�����v�Z
	KazMath::Vec2<float> value = cameraMoveValue / rateMaxValue;

	//�J�����̍��W�ɍ��킹��ׂ�X���̕����𔽓]
	value.x *= -1.0f;
	return value;
}

void Cursor::Disappear()
{
	disappearFlag = true;
}

void Cursor::Appear()
{
	disappearFlag = false;
}

void Cursor::SubCount(int SUB_VALUE)
{
	lockOnNum -= SUB_VALUE;
	if (lockOnNum <= -1)
	{
		lockOnNum = 0;
	}
}
