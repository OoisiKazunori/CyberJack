#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/Sprite2DRender.h"

/// <summary>
/// ���ꂼ��̃X�e�[�W���؂�ւ�����ۂɌ��݂̃X�e�[�W����m�点��UI
/// </summary>
class AnnounceStageUI
{
public:
	AnnounceStageUI();

	void Init();
	void Update();
	void Draw();

	void AnnounceStage(int STAGE_NUM);
private:
	bool startFlag;				//�A�i�E���X���J�n���܂�
	bool endFlag;
	int stageNum;				//���݂̃X�e�[�W�ԍ���\�����܂�

	XMVECTOR basePos, larpScale;

	int areaNum;
	int areaTimer;

	std::array<int, 10> numberHandle;
	Sprite2DRender flameTex;	//�E�B���h�E�̕`��
	Sprite2DRender numberTex;	//�����̕`��

	std::array<Sprite2DRender, 6> areaTex;		//�G���A�����̕`��

	int timer;
};

