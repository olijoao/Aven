#pragma once

#include <3rdParty/imgui/imgui.h>
#include <acore/gui/ImGui_Util.h>


namespace aven {

	// concept
	template <class T, class U>
	concept Derived = std::is_base_of<U, T>::value;


	template <Derived<Tool_Brush> T>
	inline void display_imgui(T& tool) {
		// brush
		ImGui::SetNextItemWidth(100);
		char const* items[] = { "Sphere", "Cube", "Cone", "Cylinder"};
		int brushIndex = static_cast<int>(tool.properties.brush);
		if (ImGui::Combo("##Brush", &brushIndex, items, IM_ARRAYSIZE(items)))
			tool.properties.brush = static_cast<Brush>(brushIndex);

		ImGui::PushItemWidth(100);

		ImGui::SameLine();
		display_imgui("##radius",	tool.properties.radius,		"radius: %d");
		ImGui::SameLine();
		display_imgui("##flow",		tool.properties.flow,		"flow: %d");
		ImGui::SameLine();
		display_imgui("##opacity",	tool.properties.opacity,	"opacity: %d");

		ImGui::PopItemWidth();

		// menu
		ImGui::SameLine();

		if (ImGui::Button("Settings"))
			ImGui::OpenPopup("SettingsPopUp");

		if (ImGui::BeginPopup("SettingsPopUp")) {
			// mirror
			ImGui::SameLine();
			ImGui::Text("mirror");
			ImGui::SameLine();
			display_imgui("##mirror", tool.properties.mirrored);

			ImGui::Columns(2, "Columns"); // 4-ways, with border
	
			// spacing
			ImGui::Text("spacing");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(50);
			display_imgui("##spacing", tool.properties.spacing);
			ImGui::NextColumn();

			// jitter pos 
			ImGui::Text("pos jitter");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(50);
			display_imgui("##pos jitter", tool.properties.jitter_pos);
			ImGui::NextColumn();

			// jitter size
			ImGui::Text("size jitter");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(50);
			display_imgui("##size jitter", tool.properties.jitter_size);
			ImGui::NextColumn();
		
			// jitter color 
			ImGui::Text("color jitter");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(50);
			display_imgui("##color jitter", tool.properties.jitter_color);
			ImGui::NextColumn();

			// jitter flow 
			ImGui::Text("flow jitter");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(50);
			display_imgui("##flow jitter", tool.properties.jitter_flow);
			ImGui::NextColumn();

			ImGui::Columns(1);
			
			ImGui::EndPopup();
		}
	}



}