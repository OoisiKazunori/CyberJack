#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/KazRender.h"
#include"../KazLibrary/Collision/CollisionManager.h"

class TestEnemy
{
public:
	TestEnemy();
	void Init(const XMVECTOR &POS,const int &HP);
	void Finalize();
	void Update();
	void Draw();

	void Hit();
	void Dead();

	/// <summary>
	/// ���S�t���O�������Ă��炸�A�c�胍�b�N�I������1�ȏ�Ȃ�true���Ԃ��Ă���
	/// </summary>
	bool AliveOrNot();
	
	/// <summary>
	/// hp��0�ȉ��Ȃ�true��Ԃ�
	/// </summary>
	bool LockedOrNot();

	Sphere sphere;
private:
	//���----------------------------------------------------------------
	short hp;				//���񃍃b�N�I������邩
	bool deadFlag;			//���S���Ă��邩
	bool hitFlag;			//���b�N�I����Ƀ����[�X���ꂽ����
	XMFLOAT4 hitColor;		//�����蔻���������ۂ̐F
	XMFLOAT4 normalColor;	//�������������Ă��Ȃ����̐F
	//���----------------------------------------------------------------


	//�`��----------------------------------------------------------------
	BoxPolygonRenderPtr render;		//�����̃|���S���`��
	//�`��----------------------------------------------------------------
};

