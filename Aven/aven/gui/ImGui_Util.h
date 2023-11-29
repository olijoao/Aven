#pragma once

#include <3rdParty/imgui/imgui.h>
#include <aven/util/clamped.h>
#include <aven/util/math.h>
#include <aven/volumeOperations/Tool_Paint.h>
#include <string>


namespace aven {

	inline bool display_imgui(std::string const& displayName, vec3& value) {
		return ImGui::DragFloat3(displayName.c_str(), &value[0]);
	}

	inline bool display_imgui(std::string const& displayName, float& value) {
		return ImGui::DragFloat(displayName.c_str(), &value);
	}	
	
	inline bool display_imgui(std::string const& displayName, bool& value) {
		return ImGui::Checkbox(displayName.c_str(), &value);
	}




	inline void display_imgui(std::string const& label, bvec3& value) {
		ImGui::Checkbox( ("##"+label+"_x").c_str(), &value.x);
		ImGui::SameLine();
		ImGui::Checkbox( ("##"+label+"_y").c_str(), &value.y);
		ImGui::SameLine();
		ImGui::Checkbox( ("##"+label+"_z").c_str(), &value.z);
	}



	// Used because ImGui::DragInt appears to go temporarely over bounds 
	template<int MIN, int MAX>
	bool display_imgui(std::string const& displayName, clamped<int, MIN, MAX>& clamped_val, std::string format = "%d") {
		auto value = clamped_val.getValue();
		if (ImGui::DragInt(displayName.c_str(), &value, 1, MIN, MAX, format.c_str())) {
			clamped_val.setValue(value);
			return true;
		}
		return false;
	}



	// Used because ImGui::DragInt appears to go temporarely over bounds 
	template<float MIN, float MAX>
	bool display_imgui(std::string const& displayName, clamped<float, MIN, MAX>& clamped_val, std::string format = "%.3f") {
		auto value = clamped_val.getValue();
		if (ImGui::DragFloat(displayName.c_str(), &value, 1, MIN, MAX, format.c_str())) {
			clamped_val.setValue(value);
			return true;
		}
		return false;
	}

	// Used because ImGui::DragInt appears to go temporarely over bounds 
	template<ivec3 MIN, ivec3 MAX>
	bool display_imgui(std::string const& displayName, clamped<ivec3, MIN, MAX>& clamped_val) {
		auto value = clamped_val.getValue();
		if (ImGui::DragInt3(displayName.c_str(), &value[0], 1, MIN.x, MAX.x)) {	//only uses first component
			clamped_val.setValue(value);
			return true;
		}
		return false;
	}


}
