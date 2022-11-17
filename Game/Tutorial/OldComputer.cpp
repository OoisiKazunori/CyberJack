#include "OldComputer.h"

OldComputer::OldComputer()
{
	render.data.pipelineName;
}

void OldComputer::Init(RESOURCE_HANDLE MOVIE_HANDLE)
{
	//マスク画像
	render.data.addHandle.handle[0] = TextureResourceMgr::Instance()->LoadGraph("");
	//動画画像
	render.data.addHandle.handle[1] = MOVIE_HANDLE;
}

void OldComputer::Update()
{

}

void OldComputer::Draw()
{
	render.Draw();
}

void OldComputer::SetTransform(const KazMath::Transform3D &TRANSFORM)
{
	render.data.transform = TRANSFORM;
}
