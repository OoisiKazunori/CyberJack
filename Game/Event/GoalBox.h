#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../Game/Interface/IOperationObject.h"
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Event/GoalBoxEffect.h"
#include"../Game/Event/GoalBoxPortalEffect.h"
#include<array>

class GoalBox :public IOperationObject
{
public:
	GoalBox();

	void Init(const XMVECTOR &POS);
	void Update();
	void Draw();
	
	void Appear(const XMVECTOR &POS);

	//�S�[���G�t�F�N�g
	GoalBoxEffect lightEffect;
	GoalBoxPortalEffect portalEffect;
	Sphere hitBox;
	bool releaseFlag;
	bool startPortalEffectFlag;

private:
	static const int HP = 8;

	//�ړ�-----------------------
	XMVECTOR lerpRota;//�p�x�̃��[�v
	XMVECTOR lerpScale;//�p�x�̃��[�v
	XMVECTOR lerpPos; //���W�̃��[�v
	XMVECTOR moveVel; //���������ۂɂǂꂭ�炢�ړ����邩
	XMVECTOR moveRotaVel;//���������ۂɂǂꂭ�炢�܂�邩
	float lerpMoveVel;		//���[�v�̑��x
	float lerpMoveRotaVel;	//���[�v�̑��x

	XMVECTOR goCenterPos;//�S�e�q�b�g���ɃS�[���n�_�Ɍ�����


	int prevHpNum;//�O�t���[���̗̑�
	//�`��-----------------------
	ObjModelRenderPtr model;

	float addRota;
	float addVel;
	bool hitFlag;

	int intervalTimer;

};