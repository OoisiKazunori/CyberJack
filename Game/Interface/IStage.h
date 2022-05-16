#pragma once

/// <summary>
/// ステージの抽象クラス
/// ステージ毎の描画を纏めてあります
/// </summary>
class IStage
{
public:
	~IStage() {};
	virtual void Update() = 0;
	virtual void Draw() = 0;

private:
};

