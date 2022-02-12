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
	/// <param name="HITBOX_DATA">�����蔻��̃A�h���X</param>
	/// <param name="MAX_ROCKON_NUM">���b�N�I���񐔂̃A�h���X</param>
	EnemyData(const Sphere &HITBOX_DATA, const short &MAX_ROCKON_NUM):
		hitBox(std::make_shared<Sphere>(HITBOX_DATA)), rockOnNum(std::make_shared<short>(MAX_ROCKON_NUM)),
		deadFlag(false), initFlag(false)
	{
	};

	shared_ptr<Sphere> hitBox;		//�G�̓����蔻��
	bool deadFlag;					//�G�̎��S�t���O
	bool initFlag;					//�G�̐����t���O
	shared_ptr<short> rockOnNum;	//���b�N�I���ł����
};

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
	BoxPolygonRenderPtr iEnemy_ModelRender;
};
