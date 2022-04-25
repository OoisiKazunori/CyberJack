#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../Game/Interface/IOperationObject.h"
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Event/GoalBoxEffect.h"
#include<array>

class GoalBox :public IOperationObject
{
public:
	GoalBox();

	void Init();
	void Update();
	void Draw();

	//�S�[���G�t�F�N�g
	std::array<GoalBoxEffect, 2> effect;
private:
	//�ړ�-----------------------
	XMVECTOR lerpRota;//�p�x�̃��[�v
	XMVECTOR lerpPos; //���W�̃��[�v
	XMVECTOR moveVel; //���������ۂɂǂꂭ�炢�ړ����邩
	XMVECTOR moveRotaVel;//���������ۂɂǂꂭ�炢�܂�邩
	float lerpMoveVel;		//���[�v�̑��x
	float lerpMoveRotaVel;	//���[�v�̑��x

	int prevHpNum;//�O�t���[���̗̑�
	//�`��-----------------------
	ObjModelRenderPtr model;



};