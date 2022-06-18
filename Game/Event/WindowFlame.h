#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/KazRender.h"

/// <summary>
/// ゲーム内のウィンドウのフレーム
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
	bool startFlag;				//アナウンスを開始します
	bool endFlag;
	int stageNum;				//現在のステージ番号を表示します

	KazMath::Vec3<float> basePos, larpScale;

	std::array<int, 10> numberHandle;
	Sprite3DRender flameTex;	//ウィンドウの描画

	KazMath::Vec2<float> lerpMaxScale;
};

