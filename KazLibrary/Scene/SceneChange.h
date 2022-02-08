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
		/// �V�[���J�ڂ��J�n���܂�
		/// </summary>
		void Start();

		/// <summary>
		/// ��ʂ��B�ꂽ�u�Ԃ̔�������܂�
		/// �K���g���K�[����Ŏ�鎖
		/// </summary>
		/// <returns>true...�B�ꂽ,false...�B��ĂȂ�</returns>
		bool AllHiden();

		/// <summary>
		/// �J�ڊJ�n���̕ϐ��̏�����
		/// </summary>
		void InitOutbound();

		/// <summary>
		/// �J�ڐ؂�Ԃ����̕ϐ��̏�����
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

		// �ω���
		float t_;

		// �J�ڂ��n�܂����u��
		bool start_trigger_;

		// ��ʂ��؂�ւ�����u��
		bool change_trigger_;

		// �����ʒu
		float initial_pos_;

		// ���ړ���
		float total_movement_;

		// �J��
		bool is_inbound_;

		unique_ptr<DirtyFlag<bool>>startDirtyFlag;
	};
}