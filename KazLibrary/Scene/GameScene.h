#pragma once
#include"../Game/Game.h"
#include"../KazLibrary/Scene/SceneBase.h"

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
	std::unique_ptr<Game> game;//ゲームに関するデータ

	array<array<ResponeData, Game::ENEMY_NUM_MAX>, Game::LAYER_LEVEL_MAX>responeData;

};