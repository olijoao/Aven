#pragma once

#include <3rdParty/imgui/imgui.h>
#include <aven/gui/ImGui_Util.h>
#include <aven/volumeOperations/Tool_Paint.h>
#include <aven/volumeOperations/Tool_Eraser.h>


namespace aven {

	// concept
	template <class T, class U>
	concept Derived = std::is_base_of<U, T>::value;


	template <Derived<Tool> T>
	inline void display_imgui(T& tool) {
		// brush
		ImGui::Text("Brush");
		ImGui::SameLine();
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

			// spacing
			ImGui::Text("spacing");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50);
			display_imgui("##spacing", tool.properties.spacing);

			ImGui::EndPopup();
		}
	}



}