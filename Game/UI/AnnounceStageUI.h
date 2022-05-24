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
	int stageNum;				//���݂̃X�e�[�W�ԍ���\�����܂�

	XMVECTOR basePos, larpScale;

	std::array<int, 10> numberHandle;
	Sprite2DRender flameTex;	//�E�B���h�E�̕`��
	Sprite2DRender numberTex;	//�����̕`��
	Sprite2DRender areaTex;		//�G���A�����̕`��
};

