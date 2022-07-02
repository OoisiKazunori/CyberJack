#pragma once
#include"../KazLibrary/Math/KazMath.h"
namespace KazImGuiHelper
{
	void InputTransform3D(KazMath::Transform3D *TRANSFORM)
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

	void InputTransform2D(KazMath::Transform2D *TRANSFORM)
	{
		ImGui::InputFloat("PosX", &TRANSFORM->pos.x);
		ImGui::InputFloat("PosY", &TRANSFORM->pos.y);
		ImGui::InputFloat("ScaleX", &TRANSFORM->scale.x);
		ImGui::InputFloat("ScaleY", &TRANSFORM->scale.y);
		ImGui::InputFloat("Rotation", &TRANSFORM->rotation);
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
