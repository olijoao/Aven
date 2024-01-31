
#include <3rdParty/GL/glad.h>		//order matters
#include <3rdParty/GL/glfw3.h>		//order matters

#include <3rdParty/imgui/imgui.h>
#include <3rdParty/imgui/imgui_impl_glfw.h>
#include <3rdParty/imgui/imgui_impl_opengl3.h>
#include <3rdParty/imgui/imgui_internal.h>
#include <aven/Aven.h>
#include <aven/gui/GUI.h>
#include <aven/gui/KeyBindings.h>
#include <aven/objects/BrickPool.h>
#include <aven/util/FpsCounter.h>
#include <aven/volumeOperations/FilterManager.h>
#include <aven/volumeOperations/ToolManager.h>
#include <chrono>
#include <functional>
#include <iostream>


namespace aven{
	namespace gui {
		ImVec2 operator+(ImVec2 a, ImVec2 b) { return ImVec2(a.x + b.x, a.y + b.y); };
		ImVec2 operator+(ImVec2 a, float s) { return ImVec2(a.x + s, a.y + s); };
		ImVec2 operator*(ImVec2 a, float s) { return ImVec2(a.x * s, a.y * s); };
		ImVec4 operator+(ImVec4 a, ImVec4 b) { return ImVec4(a.x + b.x, a.y + b.y, a.z+b.z, a.w+b.w); };

		GLFWwindow* glfwWindow;

		void displayMainMenu();
		void displayCanvas();
		void displayColorPicker();
		void displayScene();

		void displayChildWindow_Tools();
		void displayChildWindow_ToolsProperties();

		void displayPopup_FilterEdit();
		void displayPopup_NewProject();

		void displayDevWindow();


		//
		bool	wasFilterPopupOpenLastFrame = false;
		// enum class SceneViewer_state {Default, RotatingCamera, Tool};
		// SceneViewer_state sceneViewerState = SceneViewer_state::Default;
		bool request_modalWindow_newProject = false;	

			
		void character_callback(GLFWwindow* window, unsigned int codepoint) {
			keybindings::pushChar(codepoint);
		}

		void startFrame() {
			glfwPollEvents();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		}

		void endFrame() {
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			glfwSwapBuffers(glfwWindow);
		}
	}



	void gui::init(std::string const title, bool decorated) {
		//init glfw
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_MAXIMIZED, true);
		glfwWindowHint(GLFW_DECORATED, true);


		//create window
		glfwWindow = glfwCreateWindow(800, 600, title.c_str(), NULL, NULL);
		if (glfwWindow == NULL) {
			std::cout << "Failed to create GLFW window" << std::endl;
			std::cout << "Press enter to continue" << std::endl;
			std::cin;
			exit(EXIT_FAILURE);
		}
		glfwMakeContextCurrent(glfwWindow);

		glfwSwapInterval(0);

		glfwSetCharCallback(glfwWindow, character_callback);

		//init glad
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			std::cout << "Press enter to continue" << std::endl;
			std::cin;
			exit(EXIT_FAILURE);
		}


		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigWindowsMoveFromTitleBarOnly = true;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui_ImplOpenGL3_Init();
		ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);	//setup after callbacks since this will replace + chain callbakcs

		//set style
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_Text]					= (ImVec4)ImColor::ImColor(230, 230, 230);
		style.Colors[ImGuiCol_CheckMark]			= (ImVec4)ImColor::ImColor(0, 255, 0);
		style.Colors[ImGuiCol_FrameBg]				= (ImVec4)ImColor::ImColor(50, 50, 50);
		style.Colors[ImGuiCol_FrameBgHovered]		= (ImVec4)ImColor::ImColor(100, 100, 100);
		style.Colors[ImGuiCol_TitleBgActive]		= (ImVec4)ImColor::ImColor(0, 0, 0);
		style.Colors[ImGuiCol_TabActive]			= (ImVec4)ImColor::ImColor(70, 70, 70);
		style.Colors[ImGuiCol_Button]				= (ImVec4)ImColor::ImColor(128, 128, 128);
		style.Colors[ImGuiCol_ButtonHovered]		= (ImVec4)ImColor::ImColor(168, 168, 168);
		style.Colors[ImGuiCol_Separator]			= (ImVec4)ImColor::ImColor(0, 0, 0);
		style.Colors[ImGuiCol_SeparatorHovered]		= (ImVec4)ImColor::ImColor(135, 135, 135);
		style.Colors[ImGuiCol_SeparatorActive]		= (ImVec4)ImColor::ImColor(185, 185, 185);
		style.Colors[ImGuiCol_SliderGrab]			= (ImVec4)ImColor::ImColor(255, 255, 255);
		style.Colors[ImGuiCol_ModalWindowDimBg]		= (ImVec4)ImColor::ImColor(0, 0, 0, 0);
		style.Colors[ImGuiCol_WindowBg]				= (ImVec4)ImColor::ImColor(28, 28, 28);
		style.Colors[ImGuiCol_TabUnfocusedActive]	= style.Colors[ImGuiCol_WindowBg];
		style.Colors[ImGuiCol_Separator]			= (ImVec4)ImColor::ImColor(128, 128, 128);
		style.Colors[ImGuiCol_TitleBg]				= (ImVec4)ImColor::ImColor(17, 17, 17);
		style.Colors[ImGuiCol_TitleBgActive]		= (ImVec4)ImColor::ImColor(17, 17, 17);
		style.Colors[ImGuiCol_TitleBgCollapsed]		= (ImVec4)ImColor::ImColor(17, 17, 17);
		style.Colors[ImGuiCol_Header]				= (ImVec4)ImColor::ImColor(90, 90, 90);
		style.Colors[ImGuiCol_HeaderHovered]		= (ImVec4)ImColor::ImColor(50, 50, 50);
		style.Colors[ImGuiCol_HeaderActive]			= (ImVec4)ImColor::ImColor(70, 70, 70);
		style.WindowRounding						= 0;
		style.ScrollbarRounding						= 0;
		style.TabRounding							= 3;
		style.WindowPadding							= ImVec2(5, 5);
		style.ItemSpacing.y							= 2;
		style.ItemInnerSpacing.x					= 4;
		style.WindowMenuButtonPosition				= ImGuiDir_None;
		style.WindowRounding						= 4;
		style.FrameRounding							= 4;
		style.PopupRounding							= 4;
		style.GrabMinSize							= 4;
		style.WindowTitleAlign						= ImVec2(.5f, .5f);

		//keyStrokes
		keybindings::addKeyStroke({'z'}, []() { aven::getProject().getHistory().undo(); aven::getProject().getRenderer().resetIterations(); });
		keybindings::addKeyStroke({'y'}, []() { aven::getProject().getHistory().redo(); aven::getProject().getRenderer().resetIterations(); });
	}	 


	void gui::destroy() {
		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
	}


	void gui::display() {
		startFrame();
		keybindings::handleKeyEvents();

		displayMainMenu();
		displayScene();
		displayColorPicker();
		displayPopup_FilterEdit();
		displayPopup_NewProject();
		displayDevWindow();

		// displayScene() needs to be called before displayCanvas()
		// Starting drawing while the inputfield is still active leads to:
		// if(IsItemDeactivatedAfterEdit) commitHistory();  being called after aven::startOperation(Tool)
		displayCanvas();

		endFrame();
	}



	void gui::displayMainMenu() {
		ImGui::BeginMainMenuBar();	

		//Menu File
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New Project", "ctrl+N", false)) 
				request_modalWindow_newProject = true;
			if (ImGui::MenuItem("Save Project", "", false)) 
				aven::saveProject("scenesFiles/abc_test.vol");
			if (ImGui::MenuItem("Load Project", "", false)) 
				aven::loadProject("scenesFiles/abc_test.vol");
			
			ImGui::EndMenu();
		}

		
		//Menu Edit
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("undo", "ctrl+Z", false, aven::getProject().getHistory().undoable())) {
				aven::getProject().getHistory().undo();
				aven::getProject().getRenderer().resetIterations();
			}
			if (ImGui::MenuItem("redo", "ctrl+shift+Z", false, aven::getProject().getHistory().redoable())){
				aven::getProject().getHistory().redo();
				aven::getProject().getRenderer().resetIterations();
			}
			ImGui::EndMenu();
		}

		//Menu Filter
		if (ImGui::BeginMenu("Filter")) {
			auto& filters = aven::filterManager::getFilters();
			int i = 0;	//for filters with same name
			for (aven::Filter& filter:filters) 
				if (ImGui::MenuItem((filter.getName() + "##" + std::to_string(i++)).c_str())) 
					aven::getProject().startOperation(&filter);
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}



	void gui::displayCanvas() {

		bool open = true;
		auto flags =		ImGuiWindowFlags_NoScrollbar
						|	ImGuiWindowFlags_NoScrollWithMouse
						|	ImGuiWindowFlags_NoTitleBar;
		ImGui::Begin("Canvas", &open, flags);
		
		// resize render texture if needed
		ImVec2 contentRegion = ImGui::GetContentRegionAvail();
		auto rendererSize = aven::getProject().getRenderer().getTexture().getSize();
		if (contentRegion.x != rendererSize.x || contentRegion.y != rendererSize.y) 
			aven::getProject().getRenderer().resize(static_cast<int>(contentRegion.x), static_cast<int>(contentRegion.y));

		auto contentRegionMin			= vec2(ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMin().y);
		auto contentRegionMax			= vec2(ImGui::GetWindowContentRegionMax().x, ImGui::GetWindowContentRegionMax().y);
		auto contentRegionSize			= contentRegionMax - contentRegionMin;
		auto posMouse_absolute_inViewer = vec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y) - vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y) - contentRegionMin;
		auto posMouse_relative_inViewer = posMouse_absolute_inViewer / contentRegionSize;

		posMouse_relative_inViewer.y	= 1.0f - posMouse_relative_inViewer.y;

		auto& project = aven::getProject();

		//mouse wheel
		if (project.getCurrentOperation() == Project::Operation::None && ImGui::IsWindowHovered()) {
			if (float mouseWheel = ImGui::GetIO().MouseWheel) {
				auto& camera = aven::getProject().getRenderer().getCamera();
				camera.fov_degrees += -mouseWheel;
				aven::getProject().getRenderer().resetIterations();
			}
		}


		// mouse press
		if (project.getCurrentOperation() == Project::Operation::None && ImGui::IsWindowHovered()) {
			if (ImGui::IsMouseClicked(2)) 
				project.operation = Project::Operation::RotateCamera;
		

			if (ImGui::IsMouseClicked(0)) {
				auto* tool = aven::toolManager::getSelectedTool();
				if (tool) 
					aven::getProject().startOperation(tool, {posMouse_relative_inViewer, aven::getProject().getRenderer().getCamera().createRay(posMouse_relative_inViewer)});
			}
		}


		//drag: rotate camera
		if (project.operation == Project::Operation::RotateCamera && ImGui::IsMouseDragging(2)) {
			auto dragDelta = ImGui::GetMouseDragDelta(2);
			if ((abs(dragDelta.x) > 0 || abs(dragDelta.y) > 0)) {
				aven::getProject().getRenderer().getCamera().rotateAroundTarget(-dragDelta.x * 0.01f, -dragDelta.y * 0.01f);
				aven::getProject().getRenderer().resetIterations();
				ImGui::ResetMouseDragDelta(2);
			}
		}

		//drag: tool
		else if (project.operation == Project::Operation::Tool && ImGui::IsMouseDragging(0)) {
			auto dragDelta = ImGui::GetMouseDragDelta(0);
			if ((abs(dragDelta.x) > 0 || abs(dragDelta.y) > 0)) {
				aven::getProject().continueToolOperation({ posMouse_relative_inViewer, aven::getProject().getRenderer().getCamera().createRay(posMouse_relative_inViewer) });
				ImGui::ResetMouseDragDelta(0);
			}
				
		}

		//release: rotate camera
		if (project.operation == Project::Operation::RotateCamera && ImGui::IsMouseReleased(2)) {
			project.operation = Project::Operation::None;
		}

		//release: tool
		else if(project.operation == Project::Operation::Tool && ImGui::IsMouseReleased(0)) {
			aven::getProject().endToolOperation({ posMouse_relative_inViewer, aven::getProject().getRenderer().getCamera().createRay(posMouse_relative_inViewer) });
		}
		

		// RenderPanel
		auto& renderer = aven::getProject().getRenderer();
		renderer.render();
		auto& texture = renderer.getTexture();
		ImGui::Image(ImTextureID(ImTextureID(texture.getTextureName())), ImVec2(texture.getWidth(), texture.getHeight()));
		
		displayChildWindow_ToolsProperties();
		displayChildWindow_Tools();

		ImGui::End();
	}


	void gui::displayColorPicker() {
		ImGui::Begin("Color Chooser");
		auto color = aven::getForegroundColor();

		ImGui::SetNextItemWidth(-1);
		if (ImGui::ColorPicker3("##Color", &color[0], ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview))
			aven::setForegroundColor(color);
		ImGui::End();
	}


	void gui::displayScene() {

		bool open = true;
		if (!ImGui::Begin("scene", &open)) {
			ImGui::End();
			return;
		}

		//Camera 
		if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Columns(2, nullptr, false);
			
			auto& renderer	= aven::getProject().getRenderer();
			auto& cam		= renderer.getCamera();

			ImGui::Text("fov");	
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(-1);
			if(display_imgui("##fov", cam.fov_degrees))
				renderer.resetIterations();
			ImGui::NextColumn();

			ImGui::Text("gamma");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(-1);
			if (display_imgui("##gamma", cam.gamma))
				renderer.resetIterations();
			ImGui::NextColumn();

			ImGui::Text("maxIterations");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(-1);
			if (display_imgui("##maxIterations", cam.maxSamples))
				renderer.resetIterations();
			ImGui::NextColumn();

			ImGui::Text("nbrSamplesPerIteration");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(-1);
			if (display_imgui("##nbrSamplesPerIteration", cam.nbrSamplesPerIteration))
				renderer.resetIterations();
			ImGui::NextColumn();

			ImGui::Text("nbrBounces");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(-1);
			if (display_imgui("##nbrBounces", cam.nbrBounces))
				renderer.resetIterations();
			ImGui::NextColumn();

			ImGui::Text("background sky");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(-1);
			if (ImGui::ColorEdit3("##background sky", &cam.backgroundColor_sky[0]))
				renderer.resetIterations();
			ImGui::NextColumn();

			ImGui::Text("background ground");
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(-1);
			if (ImGui::ColorEdit3("##background ground", &cam.backgroundColor_ground[0]))
				renderer.resetIterations();
			ImGui::NextColumn();

			ImGui::Columns(1);
		}


		//Scene
		if (ImGui::CollapsingHeader("Volume", ImGuiTreeNodeFlags_DefaultOpen)){
			ImGui::Columns(2, nullptr, false);
			
			{
				ImGui::Text("pos");
				ImGui::NextColumn();

				auto& scene = aven::getProject().getScene();
				auto pos = scene.volume->pos;
				
				ImGui::SetNextItemWidth(-1);
				if (ImGui::DragFloat3("##pos", &pos[0])) {
					Volume newVolume = *scene.volume.get();
					newVolume.pos = pos;
					scene.volume = std::make_shared<Volume>(newVolume);
					aven::getProject().getRenderer().resetIterations();
				}
				if (ImGui::IsItemDeactivatedAfterEdit())
					aven::getProject().getHistory().commit();
				ImGui::NextColumn();
			}


			{
				ImGui::Text("stepSize");
				ImGui::NextColumn();
				auto& scene = aven::getProject().getScene();	//previous commitHistory will change scene
				auto stepSize = scene.volume->stepSize.getValue();
				ImGui::SetNextItemWidth(-1);
				if (ImGui::DragFloat("##stepSize", &stepSize, 1, scene.volume->stepSize.getMin(), scene.volume->stepSize.getMax())) {
					Volume newVolume = *scene.volume.get();
					newVolume.stepSize = stepSize;
					scene.volume = std::make_shared<Volume>(newVolume);
					aven::getProject().getRenderer().resetIterations();
				}
				if (ImGui::IsItemDeactivatedAfterEdit())
					aven::getProject().getHistory().commit();
				ImGui::NextColumn();
			}	
			
			{
				ImGui::Text("sigma_t");
				ImGui::NextColumn();
				auto& scene = aven::getProject().getScene();	//previous commitHistory will change scene
				auto sigma_a = scene.volume->sigma_t.getValue();
				ImGui::SetNextItemWidth(-1);
				if (ImGui::DragFloat("##sigma_a", &sigma_a, 1, scene.volume->sigma_t.getMin(), scene.volume->sigma_t.getMax())) {
					Volume newVolume = *scene.volume.get();
					newVolume.sigma_t.setValue(sigma_a);
					scene.volume = std::make_shared<Volume>(newVolume);
					aven::getProject().getRenderer().resetIterations();
				}
				if (ImGui::IsItemDeactivatedAfterEdit())
					aven::getProject().getHistory().commit();
				ImGui::NextColumn();
			}

			{
				ImGui::Text("density");
				ImGui::NextColumn();
				auto& scene = aven::getProject().getScene();	//previous commitHistory will change scene
				auto density = scene.volume->density.getValue();
				ImGui::SetNextItemWidth(-1);
				if (ImGui::DragFloat("##density", &density, 1, scene.volume->density.getMin(), scene.volume->density.getMax())) {
					Volume newVolume = *scene.volume.get();
					newVolume.density.setValue(density);
					scene.volume = std::make_shared<Volume>(newVolume);
					aven::getProject().getRenderer().resetIterations();
				}
				if (ImGui::IsItemDeactivatedAfterEdit())
					aven::getProject().getHistory().commit();
				ImGui::NextColumn();
			}


			{
				ImGui::Text("renderMode: Hybrid");
				ImGui::NextColumn();
				auto& scene = aven::getProject().getScene();	//previous commitHistory will change scene
				auto renderingMode_Hybrid = scene.volume->renderingMode_Hybrid;
				if (ImGui::Checkbox("##renderMode: Hybrid", &renderingMode_Hybrid)) {
					Volume newVolume = *scene.volume.get();
					newVolume.renderingMode_Hybrid = renderingMode_Hybrid;
					scene.volume = std::make_shared<Volume>(newVolume);
					aven::getProject().getRenderer().resetIterations();
					aven::getProject().getHistory().commit();
				}
				ImGui::NextColumn();
			}

			{
				ImGui::Text("display bounding box");
				ImGui::NextColumn();
				auto& scene = aven::getProject().getScene();	//previous commitHistory will change scene
				auto renderingBoundingBox = scene.volume->isRendering_BondingBox;
				if (ImGui::Checkbox("##display bounding box", &renderingBoundingBox)) {
					Volume newVolume = *scene.volume.get();
					newVolume.isRendering_BondingBox = renderingBoundingBox;
					scene.volume = std::make_shared<Volume>(newVolume);
					aven::getProject().getRenderer().resetIterations();
					aven::getProject().getHistory().commit();
				}	
				ImGui::NextColumn();
			}
			ImGui::Columns(1);
		}
		ImGui::End();
	}




	void gui::displayChildWindow_Tools() {
		const float iconSize = 30;
		ImGui::SetNextWindowPos(ImGui::GetWindowContentRegionMin() + ImGui::GetWindowPos() + ImVec2(5,40));
		ImGui::BeginChild("Tools", ImVec2(100, 0));
		
		auto& tools				= aven::toolManager::getTools();
		int selectedTool_index	= aven::toolManager::getSelectedIndex();

		ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg) + ImVec4(0.1f, 0.1f, 0.1f, 0));
		for (size_t i = 0; i < tools.size(); i++) {
			if (selectedTool_index == i) 
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0.4f, 0.4f, 1));
			
			std::string stringId = tools[i].getName() + "##" + std::to_string(i);
			auto& icon = tools[i].getIcon();
			if (icon.has_value()) {
				if (ImGui::ImageButton(stringId.c_str(), ImTextureID(icon->getTextureName()), {iconSize,iconSize}))
					aven::toolManager::select(i);
			} else {
				if (ImGui::Button(stringId.c_str(), ImVec2(iconSize, iconSize)+ImGui::GetStyle().FramePadding * 2.0f))
					aven::toolManager::select(i);
			}

			if (selectedTool_index == i) 
				ImGui::PopStyleColor();
		}
		ImGui::PopStyleColor(2);
		ImGui::EndChild();
	}



	void gui::displayChildWindow_ToolsProperties() {
		if (Tool_Brush* tool = aven::toolManager::getSelectedTool()) {
			ImGui::SetNextWindowPos(ImGui::GetWindowContentRegionMin() + ImGui::GetWindowPos() + ImVec2(60,10));
			ImGui::BeginChild("ToolsProperties", ImVec2(0, 100));
			tool->displayImGui();
			ImGui::EndChild();
		}
	}



	void gui::displayPopup_FilterEdit() {
		if (aven::getProject().getCurrentOperation() != aven::Project::Operation::Filter) {
			wasFilterPopupOpenLastFrame = false;
			return;
		}

		aven::Filter* filter = aven::getProject().getFilterOperation();
		assert(filter);
		std::string title = "Filter: " + filter->getName();
		if(!wasFilterPopupOpenLastFrame){	
			ImGui::OpenPopup(title.c_str());
		}
		wasFilterPopupOpenLastFrame = true;		


		if (ImGui::BeginPopupModal(title.c_str())) {

			auto& properties = filter->getProperties();
			bool atLeastOnePropertyChanged = false;
			for (auto& p:properties) {
				if (display_imgui(p))
					atLeastOnePropertyChanged = true;
			}
			if (atLeastOnePropertyChanged) {
				aven::getProject().executeFilterOperation();
			}


			//cancel Button
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0, 0.75f, 0.75f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0, 0.8f, 0.8f));
			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				aven::getProject().cancelOperation();
				ImGui::CloseCurrentPopup();
			}
			ImGui::PopStyleColor(3);

			ImGui::SameLine();

			//apply Button
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(120.0f / 360, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(120.0f / 360, 0.75f, 0.75f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(120.0f / 360, 0.8f, 0.8f));
			if (ImGui::Button("Apply", ImVec2(120, 0))) {
				aven::getProject().commitOperation();
				ImGui::CloseCurrentPopup();

			}
			ImGui::PopStyleColor(3);
			ImGui::EndPopup();

		}
	}




	void gui::displayPopup_NewProject()	{
		if (request_modalWindow_newProject) {
			request_modalWindow_newProject = false;
			ImGui::OpenPopup("Create New Project");	
		}

		if (ImGui::BeginPopupModal("Create New Project")) {
			assert(aven::getProject().getCurrentOperation() == aven::Project::Operation::None);

			static aven::clamped<ivec3, 1, 256> size = ivec3( 128, 128, 128);

			//cancel Button
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0, 0.75f, 0.75f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0, 0.8f, 0.8f));
			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::PopStyleColor(3);

			ImGui::SameLine();

			//apply Button
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(120.0f / 360, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(120.0f / 360, 0.75f, 0.75f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(120.0f / 360, 0.8f, 0.8f));
			if (ImGui::Button("Apply", ImVec2(120, 0))) {
				aven::newProject(size);
				ImGui::CloseCurrentPopup();
			}
			ImGui::PopStyleColor(3);
			ImGui::EndPopup();
		}
	}



	void gui::displayDevWindow() {
		bool open = true;
		ImGui::Begin("DevWindow", &open);

		static FpsCounter fpsConunter;
		auto passedTime = fpsConunter.tick();
		ImGui::Text(("fps:  " + std::to_string((int)(1000.0f / (float)(passedTime)))
			+ " (" + std::to_string(passedTime) + " ms" + ")").c_str());
		

		struct StatEntry {
			std::string name;
			std::string cache = "";
			std::function<std::string()> f;
		};


		static std::vector<StatEntry> stats
			= { {.name="State", .f=[]() {	auto op = aven::getProject().getCurrentOperation();
												switch (op) {
												case Project::Operation::Tool:		return "tool";
												case Project::Operation::Filter:	return "filter";
												case Project::Operation::ValueEdit:	return "valueEdit";
												default: assert(false);	// falls through
												case Project::Operation::None:		return "default";
												}}},
				{.name = "History: nbr undos",		.f = []() {	return std::to_string(aven::getProject().getHistory().getUndoCount()); }},
				{.name="BrickPool: nrbFreeBricks",	.f= []() {	return std::to_string(brickPool::nbrFreeBricks()); }},
				{.name = "BrickPool: capacity",		.f = []() {	return std::to_string(brickPool::CAPACITY); }},
		};


		if (ImGui::Button("Refresh")) 
			for (auto& entry : stats) 
				entry.cache = entry.f();
			
		
		
		ImGui::Columns(2, "Columns_Stats"); // 4-ways, with border
		ImGui::Separator();
		ImGui::Text("name"); ImGui::NextColumn();
		ImGui::Text("value"); ImGui::NextColumn();
		ImGui::Separator();
		
		for (auto entry : stats) {
			ImGui::Text(entry.name.c_str());	ImGui::NextColumn();
			ImGui::Text(entry.cache.c_str());	ImGui::NextColumn();
			ImGui::Separator();
		}

		ImGui::Columns(1);

		ImGui::End();
	}


	bool gui::isOpen() {
		return !glfwWindowShouldClose(glfwWindow);
	}


}	//namespace aven