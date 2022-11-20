#pragma once
#include"../KazLibrary/Render/KazRender.h"

class OldComputer
{
public:
	OldComputer();
	void Init(RESOURCE_HANDLE MOVIE_HANDLE);
	void Update();
	void Draw();

	void SetMonitorTexture(RESOURCE_HANDLE MOVIE_HANDLE);
	void SetTransform(const KazMath::Transform3D &TRANSFORM);
private:
	ObjModelRender render;
};

