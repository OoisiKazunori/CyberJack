#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/KazRender.h"

/// <summary>
/// �Q�[�����̃E�B���h�E�̃t���[��
/// </summary>
class WindowFlame
{
public:
	WindowFlame();

	void Init(const KazMath::Transform3D &POS, const KazMath::Vec2<float> &SCALE, bool APPEAR_WINDOW);
	void Update();
	void Draw();

	void Start();
	void End();
private:
	bool startFlag;				//�A�i�E���X���J�n���܂�
	bool endFlag;
	int stageNum;				//���݂̃X�e�[�W�ԍ���\�����܂�

	KazMath::Vec3<float> basePos, larpScale;

	std::array<int, 10> numberHandle;
	Sprite3DRender flameTex;	//�E�B���h�E�̕`��

	KazMath::Vec2<float> lerpMaxScale;
};

