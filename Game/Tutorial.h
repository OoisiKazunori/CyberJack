#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../Game/Stage/DrawGrid.h"
#include"../KazLibrary/RenderTarget/GaussianBuler.h"
#include"../KazLibrary/Helper/Select.h"
#include"../Game/UI/DrawMenu.h"
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Player.h"
#include"../Game/UI/AttackLog.h"
#include"../Game/Event/WindowFlame.h"
#include"../Game/UI/Cursor.h"
#include"../Game/Helper/CameraWork.h"
#include"../Game/LineEffect/LineLevel1.h"
#include"../Game/Event/TutorialGame.h"
#include"../Game/Effect/PortalIntermediateDirection.h"
#include"../Game/GameRenderTarget.h"

class Tutorial
{
public:
	Tutorial();
	~Tutorial();

	void Init(bool SKIP_FLAG);
	void Finalize();
	void Input();
	void Update();
	void Draw();

	bool tutorialFlag;
	RESOURCE_HANDLE handle;

	Sprite2DRender mainRenderTarget;

private:
	int sceneNum;

	int cameraIndex;
	KazMath::Vec3<float> debugCameraMove;
	KazMath::Vec3<float> eyePos, targetPos;
	KazMath::Vec2<float> angle;

	std::array<std::unique_ptr<DrawGrid>, 2> gridR;

	Player player;
	Cursor cursor;
	CameraWork cameraWork;
	const float LOG_FONT_SIZE;
	AttackLog stringLog;

	//�����o----------------------------------------------------------------
	static const int LINE_MAX_ARRAY_NUM = 16;
	std::array<LineLevel1, LINE_MAX_ARRAY_NUM> lineLevel;			//���p�ɓG�ɐ���L�΂��Ă������o
	std::array<LineEffectData, LINE_MAX_ARRAY_NUM>lineEffectArrayData;
	KazMath::Vec3<float> testEnemyPos;			//���b�N�I������Ώۍ��W(�e�X�g)
	//�����o----------------------------------------------------------------


	std::array<std::array<std::unique_ptr<IEnemy>, 10>, 3> enemies;
	std::array<std::array<LineRender, 10>, 3> hitLineR;
	std::array<std::array<int, 10>, 3>hitLineTimer;
	RESOURCE_HANDLE damageSoundHandle, lockSoundHandle;

	//�`���[�g���A���p�[�[�[�[�[�[
	std::array<TutorialGame, 3>tutorial;
	std::array<bool, 3>initEnemyFlagArray;
	int tutorialArrayIndex;
	bool tutorialAllClearFlag;
	bool initEffectFlag;
	//�`���[�g���A���p�[�[�[�[�[�[

	std::array<int, 2> flameTimer;
	std::array<std::array<LineRender, 2>, 2> portalFlameLineR;
	std::array<KazMath::Vec3<float>, 2> portalFlameBasePosArray;
	std::array<std::array<int, 2>, 2> portalLineTimer;
	const int FLAME_MAX_TIME;
	void StrachLine(const KazMath::Vec3<float> &BASE_POS, const KazMath::Vec3<float> &AFTER_POS, KazMath::Vec3<float> *LINE, int TIME, int MAX_TIME)
	{
		float lRate = static_cast<float>(TIME) / static_cast<float>(MAX_TIME);
		*LINE = BASE_POS + AFTER_POS * lRate;
	};

	int cursorLockOnNum;

	std::unique_ptr<GameRenderTarget> renderTarget;
	PortalIntermediateDirection portalEffect;
	bool initSceneFlag;
};