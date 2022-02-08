#include"../Scene/SceneChange.h"
#include"../Scene/SceneChange.h"
#include"../Helper/ResourceFilePass.h"
#include"../Loader/TextureResourceMgr.h"
#include "../Easing/easing.h"
#include "../Imgui/MyImgui.h"

using namespace ChangeScene;

SceneChange::SceneChange() {
	startFlag = false;
	finishFlag = false;
	noiseTransform.scale = { 10.0f,10.0f };

	eye_catch_handle_ = TextureResourceMgr::Instance()->LoadGraph("Resource/SceneChange/EyeCatch.png");
	eye_catch_.reset(new Sprite2DRender);
	eye_catch_->data.handle = eye_catch_handle_;
	eye_catch_->data.pipelineName = PIPELINE_NAME_SPRITE_Z_ALWAYS;
	eye_catch_->data.transform.pos = { WIN_X + WIN_X / 2, WIN_Y / 2 };

	change_trigger_ = false;
	startDirtyFlag.reset(new DirtyFlag<bool>(&startFlag));
	InitOutbound();
}

void SceneChange::Init() {
}

void SceneChange::Finalize() {
}

void SceneChange::Update() {

	startDirtyFlag->Dirty();

	if (startFlag) {
		//timer++;

		Rate(&t_, 0.1f, 1.0f);
		pos_ = EasingMaker(EasingType::Out, EaseInType::SoftBack, t_);

		eye_catch_->data.transform.pos.m128_f32[0] = initial_pos_ + total_movement_ * pos_;
		//eye_catch_->data.transform.pos = { WIN_X / 2, WIN_Y / 2, 0 };

		/*if (30 <= timer) {
			startFlag = false;
			timer = 0;
			finishFlag = true;
		}*/
	}

	if (is_inbound_) {

		Rate(&t_, 0.001f, 1.0f);
		pos_ = EasingMaker(EasingType::In, EaseInType::Cubic, t_);

		eye_catch_->data.transform.pos.m128_f32[0] = initial_pos_ + total_movement_ * pos_;
		//eye_catch_->data.transform.pos = { WIN_X / 2, WIN_Y / 2 };

		if (t_ >= 1.0f) {

			startFlag = false;

			if (startDirtyFlag) {

				InitOutbound();
			}
		}
	}

	startDirtyFlag->Record();
}

void SceneChange::Draw() {
	//if (startFlag)
	{
		//noiseTex.Draw2DGraph(noiseTransform, handle, 0, false, false, 255, PIPELINE_NAME_SPRITE_NOISE);

	}

	eye_catch_->Draw();
}

void SceneChange::Start() {
	startFlag = true;
}

bool SceneChange::AllHiden() {
	if (startFlag) {

		// 全隠れの瞬間を得る
		if (t_ == 1.0 && !change_trigger_) {

			change_trigger_ = true;
		}

		// 全部隠れたよの合図
		if (change_trigger_) {

			//startFlag = false;

			change_trigger_ = false;

			// 復路パターンに変数を変更する
			InitInbound();

			return true;
		}
	}

	return false;
}

void SceneChange::InitOutbound() {
	// 往路パターンにする
	is_inbound_ = false;
	pos_ = 0;
	t_ = 0;
	initial_pos_ = WIN_X + WIN_X / 2;
	total_movement_ = -WIN_X;
}

void SceneChange::InitInbound() {
	// 復路パターンにする
	is_inbound_ = true;
	pos_ = 0;
	t_ = 0;
	initial_pos_ = WIN_X / 2;
	total_movement_ = -WIN_X;
}
