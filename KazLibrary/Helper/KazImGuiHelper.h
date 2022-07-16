#pragma once
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Math/KazMath.h"

namespace KazImGuiHelper
{
	void InputTransform3D(KazMath::Transform3D *TRANSFORM)
	{
		ImGui::DragFloat("PosX", &TRANSFORM->pos.x);
		ImGui::DragFloat("PosY", &TRANSFORM->pos.y);
		ImGui::DragFloat("PosZ", &TRANSFORM->pos.z);
		ImGui::DragFloat("ScaleX", &TRANSFORM->scale.x);
		ImGui::DragFloat("ScaleY", &TRANSFORM->scale.y);
		ImGui::DragFloat("ScaleZ", &TRANSFORM->scale.z);
		ImGui::DragFloat("RotationX", &TRANSFORM->rotation.x);
		ImGui::DragFloat("RotationY", &TRANSFORM->rotation.y);
		ImGui::DragFloat("RotationZ", &TRANSFORM->rotation.z);
	}

	void InputTransform2D(KazMath::Transform2D *TRANSFORM)
	{
		ImGui::DragFloat("PosX", &TRANSFORM->pos.x);
		ImGui::DragFloat("PosY", &TRANSFORM->pos.y);
		ImGui::DragFloat("ScaleX", &TRANSFORM->scale.x);
		ImGui::DragFloat("ScaleY", &TRANSFORM->scale.y);
		ImGui::DragFloat("Rotation", &TRANSFORM->rotation);
	}

	void InputVec4(KazMath::Vec4<int> *VEC_4, const std::string &LABEL)
	{
		std::string label = LABEL + "X";
		ImGui::InputInt(label.c_str() , &VEC_4->x);
		label = LABEL + "Y";
		ImGui::InputInt(label.c_str(), &VEC_4->y);
		label = LABEL + "Z";
		ImGui::InputInt(label.c_str(), &VEC_4->z);
		label = LABEL + "W";
		ImGui::InputInt(label.c_str(), &VEC_4->a);
	}

	void InputVec3(KazMath::Vec3<float> *VEC_3, const std::string &LABEL)
	{
		std::string label = LABEL + "X";
		ImGui::InputFloat(label.c_str(), &VEC_3->x);
		label = LABEL + "Y";
		ImGui::InputFloat(label.c_str(), &VEC_3->y);
		label = LABEL + "Z";
		ImGui::InputFloat(label.c_str(), &VEC_3->z);
	}
}
