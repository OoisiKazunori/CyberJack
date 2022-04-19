#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Collision/CollisionManager.h"
#include"../KazLibrary/Render/KazRender.h"

class Cursor
{
public:
	Cursor();

	void Init();
	void Input(bool UP_FLAG, bool DOWN_FLAG, bool LEFT_FLAG, bool RIGHT_FLAG, bool DONE_FLAG, bool RELEASE_FLAG, const XMVECTOR &ANGLE);
	void Update();
	void Draw();

	/// <summary>
	/// ���b�N�I�������ő�l�����ł��邱�ƁA���b�N�I�����L���Ń��b�N�I���̓��͂�����Ă���
	/// ����true���ς���
	/// </summary>
	/// <returns></returns>
	bool LockOn();
	bool Release();

	void Count();
	const int &GetCount();

	const XMVECTOR &GetValue();

	XMVECTOR honraiCursorPos;
	XMVECTOR cursorPos;
	XMVECTOR prevCursorPos;
	XMVECTOR vel;
	Ray hitBox;
	XMINT2 stopFlag;
	bool releaseFlag;


	XMVECTOR honraiCameraMoveValue;//�㉺���E�ɃJ�������ǂꂭ�炢���������̒l��ۑ�����(�{���|�Y)
	XMVECTOR cameraMoveValue;//�㉺���E�ɃJ�������ǂꂭ�炢���������̒l��ۑ�����
	XMVECTOR dontMoveCameraStartPos;//�J�����̖�����J�n�n�_
	XMVECTOR dontMoveCameraEndPos;//�J�����̖�����I���n�_

	XMVECTOR limitValue;//�J�����̒[�̒l

	static XMVECTOR NO_MOVE_DISTANCE;//XY���ꂼ��ǂ��܂ł͈̔͂Ȃ�J�[�\���������Ă��J�����ɉe�����o�Ȃ���

private:
	//���b�N�I����----------------------------------------------------------------
	static const int LOCKON_MAX_NUM = 8;	//�ő働�b�N�I����
	int lockOnNum;							//���݂̃��b�N�I����
	bool notEnableLockOnFlag;
	int enableLockOnTimer;

	//�ړ�----------------------------------------------------------------
	bool upFlag, downFlag, leftFlag, rightFlag,doneFlag;
	XMVECTOR stickAngle;
	float deadLine;
	float baseSpeed;
	XMFLOAT2 speed;

	//���͉��o-----------------------

	struct CursorEffectData
	{
		bool initFlag;
		Sprite2DRenderPtr cursorEffectTex;

		CursorEffectData() :cursorEffectTex(std::make_unique<Sprite2DRender>()), initFlag(false)
		{
		}
	};

	std::array<CursorEffectData, 10> cursorEffectTex;





	//�摜----------------------------------------------------------------
	Sprite2DRenderPtr cursorFlameTex, numberTex;	//�X�N���[�����W�̕`��
	array<short, LOCKON_MAX_NUM + 1> numberHandle;			//�����p�̉摜�n���h��
	short flameHandle;
	XMVECTOR baseScale;

	XMVECTOR oldPos;
	XMVECTOR oldVel;


	//�m�b�N�o�b�N����-----------------------
	XMVECTOR knockBackVal;	//���]�ړ��p�̈ړ���
	static XMFLOAT2 KOCKBACK_MAX_VALUE;		//�m�b�N�o�b�N�̍ő��(�l�����܂�����const�ɂ���)
	static XMFLOAT2 KOCKBACK_VELOCITY;		//�m�b�N�o�b�N�̉��Z��(�l�����܂�����const�ɂ���)
	XMINT2 noTochFlag;
	XMINT2 noTockTimer;
	//�m�b�N�o�b�N����-----------------------
};

