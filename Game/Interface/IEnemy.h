#pragma once
#include"../Game/Interface/IOperationObject.h"
#include"../KazLibrary/Collision/CollisionManager.h"

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
	EnemyData(XMVECTOR *HITBOX_POS_ADRESS,const float &RADIUS ,IOperationObjectData *OPERATION_DATA) :
		hitBox({ HITBOX_POS_ADRESS ,RADIUS }),
		oprationObjData(OPERATION_DATA)
	{
	};

	Sphere hitBox;							//�G�̓����蔻��
	IOperationObjectData *oprationObjData;	//����\OBJ������{�f�[�^�̃A�h���X
};

/// <summary>
/// �G�̒��ۃN���X
/// </summary>
class IEnemy :public IOperationObject
{
public:

	/// <summary>
	/// �m���ɏ������������f�[�^������
	/// </summary>
	IEnemy();
	/// <summary>
	/// ���z�f�X�g���N�^
	/// </summary>
	virtual ~IEnemy() {};

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="POS">�������W</param>
	virtual void Init(const XMVECTOR &POS) = 0;

	/// <summary>
	/// �I������
	/// </summary>
	virtual void Finalize() = 0;
	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// �G�����S�����܂�
	/// </summary>
	void Dead();

	
	/// <returns>�G�̏��</returns>
	const unique_ptr<EnemyData> &GetData();


	unique_ptr<EnemyData> iEnemy_EnemyStatusData;		//�G�̏�Ԃ�ۑ�����f�[�^
	BoxPolygonRenderPtr iEnemy_ModelRender;				//�G�̕`��
};
