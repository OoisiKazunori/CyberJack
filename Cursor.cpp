#include "Cursor.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

Cursor::Cursor()
{
	enableLockOnTimer = 0;
	notEnableLockOnFlag = false;

	cursorFlameTex.reset(new Sprite2DRender);
	numberTex.reset(new Sprite2DRender);

	speed = 15.0f;
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

	XMVECTOR scale = { 2.0f,2.0f };
	cursorFlameTex->data.transform.scale = scale;
	numberTex->data.transform.scale = scale;
	stopFlag = { 0,0 };
}

void Cursor::Init()
{
	cursorPos = { WIN_X / 2.0f,WIN_Y / 2.0f,0.0f };

	XMVECTOR scale = { 2.0f,2.0f };
	cursorFlameTex->data.transform.scale = scale;
	numberTex->data.transform.scale = scale;
	stopFlag = { 0,0 };

	enableLockOnTimer = 0;
	notEnableLockOnFlag = false;

}

void Cursor::Input(bool UP_FLAG, bool DOWN_FLAG, bool LEFT_FLAG, bool RIGHT_FLAG, bool DONE_FLAG, bool RELEASE_FLAG)
{
	upFlag = UP_FLAG;
	downFlag = DOWN_FLAG;
	leftFlag = LEFT_FLAG;
	rightFlag = RIGHT_FLAG;
	doneFlag = DONE_FLAG;
	releaseFlag = RELEASE_FLAG;
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


	if (leftFlag)
	{
		vel.m128_f32[0] = -speed;
	}
	if (rightFlag)
	{
		vel.m128_f32[0] = speed;
	}

	if (upFlag)
	{
		vel.m128_f32[1] = -speed;
	}
	if (downFlag)
	{
		vel.m128_f32[1] = speed;
	}


	cursorPos += vel;
	if (cursorPos.m128_f32[0] < 0.0f)
	{
		cursorPos.m128_f32[0] = 0.0f;
	}
	if (WIN_X < cursorPos.m128_f32[0])
	{
		cursorPos.m128_f32[0] = WIN_X;
	}

	if (cursorPos.m128_f32[1] < 0.0f)
	{
		cursorPos.m128_f32[1] = 0.0f;
	}
	if (WIN_Y < cursorPos.m128_f32[1])
	{
		cursorPos.m128_f32[1] = WIN_Y;
	}


	cursorFlameTex->data.transform.pos = { cursorPos.m128_f32[0],cursorPos.m128_f32[1] };
	numberTex->data.transform.pos = { cursorPos.m128_f32[0],cursorPos.m128_f32[1] };
	//�ړ�----------------------------------------------------------------


	//�����蔻��----------------------------------------------------------------
	XMVECTOR nearPos = { cursorPos.m128_f32[0],cursorPos.m128_f32[1],0.0f,0.0f };
	XMVECTOR farPos = { cursorPos.m128_f32[0],cursorPos.m128_f32[1],1.0f,0.0f };
	XMVECTOR endPos = KazMath::ConvertScreenPosToWorldPos(farPos, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());

	hitBox.start = KazMath::ConvertScreenPosToWorldPos(nearPos, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());
	hitBox.dir = XMVector3Normalize(endPos - hitBox.start);
	//�����蔻��----------------------------------------------------------------

}

void Cursor::Draw()
{
	numberTex->Draw();
	cursorFlameTex->Draw();
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
