#pragma once
#include"../Game/Interface/IOperationObject.h"
#include"../KazLibrary/Collision/CollisionManager.h"
#include"../KazLibrary/Math/KazMath.h"

/// <summary>
/// �G�̎��
/// </summary>
enum eEnemyType
{
	ENEMY_TYPE_NONE = -1,
	ENEMY_TYPE_NORMAL,
	ENEMY_TYPE_MISILE,
	ENEMY_TYPE_MISILE_SPLINE,
	ENEMY_TYPE_MOTHER,
	ENEMY_TYPE_KID,
	ENEMY_TYPE_BATTLESHIP,
	ENEMY_TYPE_BATTLESHIP_MISILE
};

enum BattleShipMisileEnum
{
	BATTLESHIP_MISILE_SHOT_LEFT,
	BATTLESHIP_MISILE_SHOT_RIGHT,
};

struct BattleShipMisileData
{
	//���E�ǂ���ɔ�Ԃ�
	BattleShipMisileEnum shotEnum;
};

struct BattleShipData
{
	//�~�T�C���������ǂ���
	bool isShotFlag;

	BattleShipData() :isShotFlag(false)
	{};
};

/// <summary>
/// �G�𐶐�����ۂɕK�v�ȏ����l�ߍ���
/// </summary>
struct EnemyGenerateData
{
	KazMath::Vec3<float> initPos;	//�������W
	float speed;
	BattleShipMisileData battleShipEnemyData;
	BattleShipData battleShipData;

	EnemyGenerateData() :speed(-1.5f)
	{
	};
};

struct ResponeData
{
	ResponeData() :layerLevel(-1), flame(-1)
	{
	}
	int layerLevel;		//�ǂ̃��C���[���x���Ō���邩
	int flame;		//�G�������t���[����
	EnemyGenerateData generateData;
};


/// <summary>
/// �G���瑼��OBJ�𐶐�����ۂɓn���f�[�^
/// </summary>
struct GenarateOtherObjData
{
	GenarateOtherObjData() :enemyType(-1)
	{
	}

	void Finalize()
	{
		enemyType = -1;
	}
	int enemyType;			//�ǂ̎�ނ�o�ꂳ���邩
	EnemyGenerateData enemyGenerateData;
};

/// <summary>
/// �Q�[�����ɓo�ꂷ��G�̃f�[�^
/// </summary>
struct EnemyData
{
	/// <summary>
	/// �G�̏������������܂�
	/// </summary>
	/// <param name="HITBOX_POS_ADRESS">�����蔻��p�̍��W�A�h���X</param>
	/// <param name="RADIUS">�����蔻��̑傫��</param>
	/// <param name="MAX_ROCKON_NUM">���b�N�I���񐔂̃A�h���X</param>
	EnemyData(KazMath::Vec3<float> *HITBOX_POS_ADRESS, const float &RADIUS, IOperationObjectData *OPERATION_DATA) :
		hitBox({ HITBOX_POS_ADRESS ,RADIUS }),
		oprationObjData(OPERATION_DATA),
		timer(1),
		outOfStageFlag(false),
		radius(0.0f),
		startFlag(false)
	{
	};

	Sphere hitBox;							//�G�̓����蔻��
	IOperationObjectData *oprationObjData;	//����\OBJ������{�f�[�^�̃A�h���X
	GenarateOtherObjData genarateData;		//�ǉ��œG�𐶐�����ۂɕK�v�ȏ��
	int timer;								//�v���C���[�ɓ�����܂ł̎���
	bool outOfStageFlag;					//�X�e�[�W�͈̔͊O�ɏo�Ă��܂���
	float radius;
	bool startFlag;
};