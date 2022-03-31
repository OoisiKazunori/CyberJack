#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/LineEffect/LineEffect.h"

struct EnemyMoveData
{
	bool straightFlag;
	bool leftFlag;
	bool backFlag;
	bool rightFlag;
};

enum EnumLineMove
{
	LINE_MOVE_RIGHT,
	LINE_MOVE_LEFT,
	LINE_MOVE_UP,
	LINE_MOVE_DOWN,
	LINE_MOVE_STRAIGHT,
	LINE_MOVE_BACK
};

enum eLineMove
{
	LINE_MOVE_NONE = -1 ,
	LINE_MOVE_X,
	LINE_MOVE_Y,
	LINE_MOVE_Z
};

enum eSurface
{
	SURFACE_NONE = -1,
	SURFACE_LEFT,
	SURFACE_RIGHT,
	SURFACE_FRONT,
	SURFACE_BACK,
	SURFACE_TOP,
	SURFACE_BUTTOM,
};


class LineLevel1
{
public:
	LineLevel1();

	void CalucurateDistance(const XMVECTOR &PLAYER_POS, const XMVECTOR &ENEMY_POS);

	void Attack(const XMVECTOR &PLAYER_POS, const XMVECTOR &ENEMY_POS, const EnemyMoveData &FLAG_DATA);
	void Attack2(const XMVECTOR &PLAYER_POS, const XMVECTOR &ENEMY_POS, const EnemyMoveData &FLAG_DATA);

	void ReleaseShot();

	void Release();

	void Update();
	void Draw();

	bool allFinishFlag;
	bool initFlag;
private:
	vector<XMVECTOR> limitPos;
	vector<BoxPolygonRenderPtr> limitPolygon;
	vector<unique_ptr<LineEffect>> line;

	array<int, 3> countVec;

	XMVECTOR rockOnDistance;//���b�N�I�������ۂ̓G�̍��W
	XMVECTOR distanceValue;//���b�N�I�����̋����ƌ��݂̃v���C���[�ƓG�̋����̊���

	XMVECTOR CalucurateMoveVector(const int &RANDM_NUM, const float &LENGTH);
	int CalucurateDirection(const int &MOVE_VEC, const int &MOVE_VEC_MAX_NUM);
	int CalucurateDirection2(const int &MOVE_VEC, const int &MOVE_VEC_MAX_NUM);

	eLineMove CaluRandom(int DONT_USE, int OLD_NUM);

	XMVECTOR firstDir(eSurface SURFACE);
};
