#pragma once
#include"../Scene/SceneBase.h"
#include"../Render/BackGroundForDebug.h"
#include"../Render/KazRender.h"
#include<memory>
#include"../Player.h"
#include"../Cursor.h"
#include"../TestEnemy.h"
#include"../Game/Interface/IEnemy.h"
#include"../Game/Enemy/NormalEnemy.h"

struct ResponeData
{
	short flame;		//“G‚ªŒ»‚ê‚éƒtƒŒ[ƒ€”
	short enemyType;	//“G‚Ìí—Ş
	XMVECTOR initPos;	//‰ŠúÀ•W
};

class GameScene :public SceneBase
{
public:
	GameScene();
	~GameScene();

	void Init();
	void Finalize();
	void Input();
	void Update();
	void Draw();

	int SceneChange();

private:
	int sceneNum;
	BackGroundForDebug bg;

	XMFLOAT3 debugCameraMove;
	XMFLOAT3 eyePos, targetPos;
	XMFLOAT2 angle;

	Player player;
	Cursor cursor;
	TestEnemy hitBox;

	array<unique_ptr<IEnemy>, 2>enemy;


	array<array<unique_ptr<IEnemy>, 10>, 50> enemies;
	array<int, 10> enemiesHandle;
};
