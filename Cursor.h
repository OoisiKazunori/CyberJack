#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Collision/CollisionManager.h"
#include"../KazLibrary/Render/KazRender.h"

class Cursor
{
public:
	Cursor();

	void Init();
	void Input(bool UP_FLAG, bool DOWN_FLAG, bool LEFT_FLAG, bool RIGHT_FLAG, bool DONE_FLAG,bool RELEASE_FLAG);
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
	XMVECTOR vel;
	Ray hitBox;
	XMINT2 stopFlag;
private:
	//���b�N�I����----------------------------------------------------------------
	static const int LOCKON_MAX_NUM = 8;	//�ő働�b�N�I����
	int lockOnNum;							//���݂̃��b�N�I����
	bool notEnableLockOnFlag;
	int enableLockOnTimer;

	bool releaseFlag;

	//�ړ�----------------------------------------------------------------
	bool upFlag, downFlag, leftFlag, rightFlag,doneFlag;


	//�摜----------------------------------------------------------------
	Sprite2DRenderPtr cursorFlameTex, numberTex;	//�X�N���[�����W�̕`��
	array<short, LOCKON_MAX_NUM + 1> numberHandle;			//�����p�̉摜�n���h��
	short flameHandle;
	
	float speed;
	XMVECTOR knockBackVal;	//���]�ړ��p�̈ړ���
	XMVECTOR oldPos;
	XMVECTOR oldVel;
};

