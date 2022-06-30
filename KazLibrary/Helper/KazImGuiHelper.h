#pragma once
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Math/KazMath.h"

namespace KazImGuiHelper
{
	void InputTransform(KazMath::Transform3D *TRANSFORM)
	{
		ImGui::InputFloat("PosX", &TRANSFORM->pos.x);
		ImGui::InputFloat("PosY", &TRANSFORM->pos.y);
		ImGui::InputFloat("PosZ", &TRANSFORM->pos.z);
		ImGui::InputFloat("ScaleX", &TRANSFORM->scale.x);
		ImGui::InputFloat("ScaleY", &TRANSFORM->scale.y);
		ImGui::InputFloat("ScaleZ", &TRANSFORM->scale.z);
		ImGui::InputFloat("RotationX", &TRANSFORM->rotation.x);
		ImGui::InputFloat("RotationY", &TRANSFORM->rotation.y);
		ImGui::InputFloat("RotationZ", &TRANSFORM->rotation.z);
	}
}