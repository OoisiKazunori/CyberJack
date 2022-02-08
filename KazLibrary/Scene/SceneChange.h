#pragma once
#include"../DirectXCommon/Base.h"
#include"../Math/KazMath.h"
#include"../Render/KazRender.h"
#include"../Render/KazRender.h"
#include"../Helper/DirtyFlag.h"
namespace ChangeScene {

	class SceneChange {
	public:
		SceneChange();

		void Init();
		void Finalize();
		void Update();
		void Draw();

		/// <summary>
		/// ƒV[ƒ“‘JˆÚ‚ğŠJn‚µ‚Ü‚·
		/// </summary>
		void Start();

		/// <summary>
		/// ‰æ–Ê‚ª‰B‚ê‚½uŠÔ‚Ì”»’è‚ğæ‚è‚Ü‚·
		/// •K‚¸ƒgƒŠƒK[”»’è‚Åæ‚é–
		/// </summary>
		/// <returns>true...‰B‚ê‚½,false...‰B‚ê‚Ä‚È‚¢</returns>
		bool AllHiden();

		/// <summary>
		/// ‘JˆÚŠJn‚Ì•Ï”‚Ì‰Šú‰»
		/// </summary>
		void InitOutbound();

		/// <summary>
		/// ‘JˆÚØ‚è•Ô‚µ‚Ì•Ï”‚Ì‰Šú‰»
		/// </summary>
		void InitInbound();

	private:
		bool startFlag, finishFlag;

		KazMath::Transform2D noiseTransform;
		//TextureRender noiseTex;
		short handle;

		int timer;

		short eye_catch_handle_;
		Sprite2DRenderPtr eye_catch_;

		float pos_;

		// •Ï‰»—¦
		float t_;

		// ‘JˆÚ‚ªn‚Ü‚Á‚½uŠÔ
		bool start_trigger_;

		// ‰æ–Ê‚ªØ‚è‘Ö‚í‚Á‚½uŠÔ
		bool change_trigger_;

		// ‰ŠúˆÊ’u
		float initial_pos_;

		// ‘ˆÚ“®—Ê
		float total_movement_;

		// ‘JˆÚ
		bool is_inbound_;

		unique_ptr<DirtyFlag<bool>>startDirtyFlag;
	};
}