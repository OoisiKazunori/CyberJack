#include"../Scene/SceneChange.h"
#include"../Scene/SceneChange.h"
#include"../Helper/ResourceFilePass.h"
#include"../Loader/TextureResourceMgr.h"
#include "../Easing/easing.h"
#include "../Imgui/MyImgui.h"

using namespace ChangeScene;

SceneChange::SceneChange()
{
	sceneTex.data.handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::SceneChangePath + "SceneChange.png");
	sceneTex.data.pipelineName = PIPELINE_NAME_SPRITE_Z_ALWAYS;
	transform.pos.m128_f32[1] = WIN_Y / 2.0f;
}

void SceneChange::Init()
{
	startFlag = false;
}

void SceneChange::Finalize()
{
	startFlag = false;
}

void SceneChange::Update()
{
	if (startFlag)
	{
		//“ü‚è
		if (startOutInT[0] < 1.0)
		{
			Rate(&startOutInT[0], 0.03, 1.0);
			transform.pos.m128_f32[0] = WIN_X + (WIN_X / 2) + EasingMaker(Out, Cubic, startOutInT[0]) * -WIN_X;
			tmp = transform.pos.m128_f32[0];
		}
		//I‚í‚è
		else
		{
			//‰B‚ê‚½ƒtƒ‰ƒO
			if (!initFlag)
			{
				allHidenFlag = true;
				initFlag = true;
			}

			Rate(&startOutInT[1], 0.03, 1.0);
			transform.pos.m128_f32[0] = tmp + EasingMaker(In, Cubic, startOutInT[1]) * -WIN_X;
		}

		if (1.0 <= startOutInT[1])
		{
			startFlag = false;
		}
	}
	else
	{
		initFlag = false;
		startOutInT[0] = 0;
		startOutInT[1] = 0;
		transform.pos.m128_f32[0] = WIN_X + (WIN_X / 2);
	}

	sceneTex.data.transform = transform;
}

void SceneChange::Draw()
{
	sceneTex.Draw();
}

void SceneChange::Start()
{
	startFlag = true;
}

bool SceneChange::AllHiden()
{
	if (allHidenFlag)
	{
		allHidenFlag = false;
		return true;
	}
	return false;
}
