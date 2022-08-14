#pragma once
#include<memory>
#include<array>
#include"../Game/Enemy/NormalEnemy.h"
#include"../Game/Enemy/KidEnemy.h"
#include"../Game/Enemy/SplineMisile.h"
#include"../Game/Enemy/NormalMisileEnemy.h"
#include"../Game/Enemy/BattleshipEnemy.h"
#include"../Game/Enemy/SplineMisileForBattleShip.h"

namespace KazEnemyHelper
{
	static const int LAYER_LEVEL_MAX = 10;				//レイヤーレベルの最大数
	static const int ENEMY_NUM_MAX = 100;				//1レイヤーレベルに登場する敵の最大数

	struct ResponeData
	{
		ResponeData() :layerLevel(-1), flame(-1), initPos({})
		{
		}
		int layerLevel;		//どのレイヤーレベルで現れるか
		int flame;		//敵が現れるフレーム数
		KazMath::Vec3<float> initPos;	//初期座標

	};

	void GenerateEnemy(
		std::array<std::array<std::unique_ptr<IEnemy>, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> &ENEMIES,
		array<array<ResponeData, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> RESPONE_DATA,
		std::array<int, 10> &ENEMISE_HANDLE
	);

	void AddEnemy(
		const std::array<std::array<std::unique_ptr<IEnemy>, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> &ENEMIES,
		array<array<ResponeData, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> &RESPONE_DATA,
		std::array<int, 10> &ADD_ENEMISE_HANDLE,
		int GAME_FLAME,
		int GAME_STAGE_LEVEL
	);

	void InitEnemy(
		std::array<std::array<std::unique_ptr<IEnemy>, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> &ENEMIES,
		array<array<ResponeData, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> RESPONE_DATA,
		int GAME_FLAME,
		int GAME_STAGE_LEVEL,
		bool DEMO_FLAG = false
	);
}
