
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
	

		// windows
		bool window_dev_isOpen = false;

		//
		bool wasFilterPopupOpenLastFrame = false;
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
		style.Colors[ImGuiCol_ButtonHovered]		= (ImVec4)ImColor::ImColor(70, 70, 70);
		style.Colors[ImGuiCol_Button]				= (ImVec4)ImColor::ImColor(50, 50, 50);
		style.Colors[ImGuiCol_CheckMark]			= (ImVec4)ImColor::ImColor(0, 255, 0);
		style.Colors[ImGuiCol_FrameBgHovered]		= (ImVec4)ImColor::ImColor(100, 100, 100);
		style.Colors[ImGuiCol_FrameBg]				= (ImVec4)ImColor::ImColor(50, 50, 50);
		style.Colors[ImGuiCol_HeaderActive]			= (ImVec4)ImColor::ImColor(70, 70, 70);
		style.Colors[ImGuiCol_HeaderHovered]		= (ImVec4)ImColor::ImColor(50, 50, 50);
		style.Colors[ImGuiCol_Header]				= (ImVec4)ImColor::ImColor(64, 64, 64);
		style.Colors[ImGuiCol_ModalWindowDimBg]		= (ImVec4)ImColor::ImColor(0, 0, 0, 0);
		style.Colors[ImGuiCol_SeparatorActive]		= (ImVec4)ImColor::ImColor(185, 185, 185);
		style.Colors[ImGuiCol_SeparatorHovered]		= (ImVec4)ImColor::ImColor(135, 135, 135);
		style.Colors[ImGuiCol_Separator]			= (ImVec4)ImColor::ImColor(0, 0, 0);
		style.Colors[ImGuiCol_Separator]			= (ImVec4)ImColor::ImColor(128, 128, 128);
		style.Colors[ImGuiCol_SliderGrab]			= (ImVec4)ImColor::ImColor(255, 255, 255);
		style.Colors[ImGuiCol_TabActive]			= (ImVec4)ImColor::ImColor(70, 70, 70);
		style.Colors[ImGuiCol_TabUnfocusedActive]	= (ImVec4)ImColor::ImColor(28, 28, 28);
		style.Colors[ImGuiCol_Text]					= (ImVec4)ImColor::ImColor(230, 230, 230);
		style.Colors[ImGuiCol_TitleBgActive]		= (ImVec4)ImColor::ImColor(0, 0, 0);
		style.Colors[ImGuiCol_TitleBgActive]		= (ImVec4)ImColor::ImColor(17, 17, 17);
		style.Colors[ImGuiCol_TitleBgCollapsed]		= (ImVec4)ImColor::ImColor(17, 17, 17);
		style.Colors[ImGuiCol_TitleBg]				= (ImVec4)ImColor::ImColor(17, 17, 17);
		style.Colors[ImGuiCol_WindowBg]				= (ImVec4)ImColor::ImColor(28, 28, 28);
		style.FrameRounding							= 4;
		style.GrabMinSize							= 4;
		style.IndentSpacing							= 12;
		style.ItemInnerSpacing.x					= 4;
		style.ItemSpacing.y							= 2;
		style.PopupRounding							= 4;
		style.ScrollbarRounding						= 0;
		style.TabRounding							= 3;
		style.WindowMenuButtonPosition				= ImGuiDir_None;
		style.WindowPadding							= ImVec2(5, 5);
		style.WindowRounding						= 0;
		style.WindowRounding						= 4;
		style.WindowTitleAlign						= ImVec2(.5f, .5f);

		//keyStrokes
		keybindings::addKeyStroke({'z'}, []() { aven::getProject().history.undo(); aven::getProject().renderer.resetIterations(); });
		keybindings::addKeyStroke({'y'}, []() { aven::getProject().history.redo(); aven::getProject().renderer.resetIterations(); });
	}	 


	void gui::destroy() {
		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
	}


	void gui::display() {
		startFrame();

		try {
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

			//ImGui::ShowDemoWindow();
		}
		catch (std::exception const& se) {
			std::cout << std::endl;
			std::cout << "Exception caught in gui loop: " << std::endl;
			std::cout << se.what() << std::endl;
			ImGui::ErrorCheckEndFrameRecover(nullptr, nullptr);
		}

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
			if (ImGui::MenuItem("undo", "ctrl+Z", false, aven::getProject().history.undoable())) {
				aven::getProject().history.undo();
				aven::getProject().renderer.resetIterations();
			}
			if (ImGui::MenuItem("redo", "ctrl+shift+Z", false, aven::getProject().history.redoable())){
				aven::getProject().history.redo();
				aven::getProject().renderer.resetIterations();
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

		// Menu View
		if (ImGui::BeginMenu("View")) {
			if (ImGui::MenuItem("Dev Window", "", window_dev_isOpen)) {
				window_dev_isOpen = !window_dev_isOpen;
			}
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
		auto rendererSize = aven::getProject().renderer.getTexture().getSize();
		if (contentRegion.x != rendererSize.x || contentRegion.y != rendererSize.y) 
			aven::getProject().renderer.resize(static_cast<int>(contentRegion.x), static_cast<int>(contentRegion.y));

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
				auto& camera = aven::getProject().camera;
				auto dir =  camera.pos - camera.target;
				auto dist = distance(camera.pos, camera.target);
				assert(dist > 0.0001f);
				dist -= mouseWheel*5;
				dist = std::max(dist, 0.01f);
				camera.pos = camera.target + normalize(dir) * dist;
				aven::getProject().renderer.resetIterations();
			}
		}


		// mouse press
		if (project.getCurrentOperation() == Project::Operation::None && ImGui::IsWindowHovered()) {
			if (ImGui::IsMouseClicked(2)) 
				project.operation = Project::Operation::RotateCamera;
		

			if (ImGui::IsMouseClicked(0)) {
				auto* tool = aven::toolManager::getSelectedTool();
				if (tool) 
					aven::getProject().startOperation(tool, {posMouse_relative_inViewer, aven::getProject().camera.createRay(posMouse_relative_inViewer)});
			}
		}


		//drag: rotate camera
		if (project.operation == Project::Operation::RotateCamera && ImGui::IsMouseDragging(2)) {
			auto dragDelta = ImGui::GetMouseDragDelta(2);
			if ((abs(dragDelta.x) > 0 || abs(dragDelta.y) > 0)) {
				aven::getProject().camera.rotateAroundTarget(-dragDelta.x * 0.01f, -dragDelta.y * 0.01f);
				aven::getProject().renderer.resetIterations();
				ImGui::ResetMouseDragDelta(2);
			}
		}

		//drag: tool
		else if (project.operation == Project::Operation::Tool && ImGui::IsMouseDragging(0)) {
			auto dragDelta = ImGui::GetMouseDragDelta(0);
			if ((abs(dragDelta.x) > 0 || abs(dragDelta.y) > 0)) {
				aven::getProject().continueToolOperation({ posMouse_relative_inViewer, aven::getProject().camera.createRay(posMouse_relative_inViewer) });
				ImGui::ResetMouseDragDelta(0);
			}
				
		}

		//release: rotate camera
		if (project.operation == Project::Operation::RotateCamera && ImGui::IsMouseReleased(2)) {
			project.operation = Project::Operation::None;
		}

		//release: tool
		else if(project.operation == Project::Operation::Tool && ImGui::IsMouseReleased(0)) {
			aven::getProject().endToolOperation({ posMouse_relative_inViewer, aven::getProject().camera.createRay(posMouse_relative_inViewer) });
		}
		

		// RenderPanel
		auto& renderer = aven::getProject().renderer;
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

		if (ImGui::Button("+light")) {
			auto& project = aven::getProject();
			project.getScene().cloneLights();
			project.getScene().push_light(Light());
			project.commit();
			project.renderer.resetIterations();
		}

		ImGui::Separator();

		//RenderSettings
		if (ImGui::TreeNodeEx("RenderSettings", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed)) {
			ImGui::Columns(2, nullptr, false);

			{
				ImGui::Text("gamma");
				ImGui::NextColumn();

				auto& renderSettings = aven::getProject().getScene().renderSettings;
				float gamma = renderSettings->gamma;
				
				ImGui::SetNextItemWidth(-1);
				if (ImGui::DragFloat("##gamma", &gamma)) {
					RenderSettings rs = *renderSettings;
					rs.gamma = gamma;
					aven::getProject().getScene().renderSettings = std::make_shared<RenderSettings>(rs);
					aven::getProject().renderer.resetIterations();
				}
				if (ImGui::IsItemDeactivatedAfterEdit())
					aven::getProject().history.commit();
				ImGui::NextColumn();
			}


			{
				ImGui::Text("maxIterations");
				ImGui::NextColumn();

				auto& renderSettings = aven::getProject().getScene().renderSettings;
				int maxIterations = renderSettings->maxSamples;
				
				ImGui::SetNextItemWidth(-1);
				if (ImGui::DragInt("##maxIterations", &maxIterations)) {
					RenderSettings rs = *renderSettings;
					rs.maxSamples.setValue(maxIterations);
					aven::getProject().getScene().renderSettings = std::make_shared<RenderSettings>(rs);
					aven::getProject().renderer.resetIterations();
				}
				if (ImGui::IsItemDeactivatedAfterEdit())
					aven::getProject().history.commit();
				ImGui::NextColumn();
			}


			{
				ImGui::Text("samplesPerIteration");
				ImGui::NextColumn();

				auto& renderSettings = aven::getProject().getScene().renderSettings;
				int samplesPerIteration = renderSettings->nbrSamplesPerIteration;
				
				ImGui::SetNextItemWidth(-1);
				if (ImGui::DragInt("##samplesPerIteration", &samplesPerIteration)) {
					RenderSettings rs = *renderSettings;
					rs.nbrSamplesPerIteration = samplesPerIteration;
					aven::getProject().getScene().renderSettings = std::make_shared<RenderSettings>(rs);
					aven::getProject().renderer.resetIterations();
				}
				if (ImGui::IsItemDeactivatedAfterEdit())
					aven::getProject().history.commit();
				ImGui::NextColumn();
			}



			{
				ImGui::Text("nbrBounces");
				ImGui::NextColumn();

				auto& renderSettings = aven::getProject().getScene().renderSettings;
				int nbrBounces = renderSettings->nbrBounces;
				
				ImGui::SetNextItemWidth(-1);
				if (ImGui::DragInt("##nbrBounces", &nbrBounces)) {
					RenderSettings rs = *renderSettings;
					rs.nbrBounces = nbrBounces;
					aven::getProject().getScene().renderSettings = std::make_shared<RenderSettings>(rs);
					aven::getProject().renderer.resetIterations();
				}
				if (ImGui::IsItemDeactivatedAfterEdit())
					aven::getProject().history.commit();
				ImGui::NextColumn();
			}


			{
				ImGui::Text("background sky");
				ImGui::NextColumn();

				auto& renderSettings = aven::getProject().getScene().renderSettings;
				vec3 sky = renderSettings->backgroundColor_sky;
				
				ImGui::SetNextItemWidth(-1);
				if (ImGui::ColorEdit3("##background sky", &sky[0])) {
					RenderSettings rs = *renderSettings;
					rs.backgroundColor_sky = sky;
					aven::getProject().getScene().renderSettings = std::make_shared<RenderSettings>(rs);
					aven::getProject().renderer.resetIterations();
				}
				if (ImGui::IsItemDeactivatedAfterEdit())
					aven::getProject().history.commit();
				ImGui::NextColumn();
			}

			{
				ImGui::Text("background ground");
				ImGui::NextColumn();

				auto& renderSettings = aven::getProject().getScene().renderSettings;
				vec3 ground = renderSettings->backgroundColor_ground;
				
				ImGui::SetNextItemWidth(-1);
				if (ImGui::ColorEdit3("##background ground", &ground[0])) {
					RenderSettings rs = *renderSettings;
					rs.backgroundColor_ground = ground;
					aven::getProject().getScene().renderSettings = std::make_shared<RenderSettings>(rs);
					aven::getProject().renderer.resetIterations();
				}
				if (ImGui::IsItemDeactivatedAfterEdit())
					aven::getProject().history.commit();
				ImGui::NextColumn();
			}

			{
				ImGui::Text("light from background");
				ImGui::NextColumn();

				auto& renderSettings = aven::getProject().getScene().renderSettings;
				bool light = renderSettings->useBackgroundAsLight;
				
				ImGui::SetNextItemWidth(-1);
				if (ImGui::Checkbox("##light from background", &light)) {
					RenderSettings rs = *renderSettings;
					rs.useBackgroundAsLight = light;
					aven::getProject().getScene().renderSettings = std::make_shared<RenderSettings>(rs);
					aven::getProject().renderer.resetIterations();
				}
				if (ImGui::IsItemDeactivatedAfterEdit())
					aven::getProject().history.commit();
				ImGui::NextColumn();
			}

			{
				ImGui::Text("background intenstiy");
				ImGui::NextColumn();

				auto& renderSettings = aven::getProject().getScene().renderSettings;
				float intensity = renderSettings->backgroundIntensity;
				
				ImGui::SetNextItemWidth(-1);
				if (ImGui::DragFloat("##background multi", &intensity)) {
					RenderSettings rs = *renderSettings;
					rs.backgroundIntensity = intensity;
					aven::getProject().getScene().renderSettings = std::make_shared<RenderSettings>(rs);
					aven::getProject().renderer.resetIterations();
				}
				if (ImGui::IsItemDeactivatedAfterEdit())
					aven::getProject().history.commit();
				ImGui::NextColumn();
			}

			ImGui::TreePop();
			ImGui::Columns(1);
		}


		//Scene
		if (ImGui::TreeNodeEx("Volume", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed)) {
			ImGui::Columns(2, nullptr, false);

			{
				ImGui::Text("pos");
				ImGui::NextColumn();

				auto& scene = aven::getProject().getScene();
				auto pos = scene.volume->transformation.getPos();
				
				ImGui::SetNextItemWidth(-1);
				if (ImGui::DragFloat3("##pos", &pos[0])) {
					Volume newVolume = *scene.volume.get();
					newVolume.transformation.setPos(pos);
					scene.volume = std::make_shared<Volume>(newVolume);
					aven::getProject().renderer.resetIterations();
				}
				if (ImGui::IsItemDeactivatedAfterEdit())
					aven::getProject().history.commit();
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
					aven::getProject().renderer.resetIterations();
				}
				if (ImGui::IsItemDeactivatedAfterEdit())
					aven::getProject().history.commit();
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
					newVolume.density = density;
					scene.volume = std::make_shared<Volume>(newVolume);
					aven::getProject().renderer.resetIterations();
				}
				if (ImGui::IsItemDeactivatedAfterEdit())
					aven::getProject().history.commit();
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
					aven::getProject().renderer.resetIterations();
					aven::getProject().history.commit();
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
					aven::getProject().renderer.resetIterations();
					aven::getProject().history.commit();
				}	
				ImGui::NextColumn();
			}
			ImGui::Columns(1);
			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Lights", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed)) {
			int index_lightToRemove = -1;
			int i = 0;
			while (i < aven::getProject().getScene().getLights().size()) {


				auto flags	= ImGuiTreeNodeFlags_DefaultOpen 
							| ImGuiTreeNodeFlags_Framed 
							| ImGuiTreeNodeFlags_AllowOverlap 
							| ImGuiTreeNodeFlags_ClipLabelForTrailingButton;
				std::string label = "Light " + std::to_string(i);
				if (ImGui::TreeNodeEx(label.c_str(), flags)) {

					// button to remove light
					// semi-copied from imgui_widget::CollapsingHeader
					{
						ImGuiWindow* window = ImGui::GetCurrentWindow();
						ImGuiID id = window->GetID(label.c_str());
						ImGuiContext& g = *GImGui;
						ImGuiLastItemData last_item_backup = g.LastItemData;
						float button_size = g.FontSize;
						float button_x = ImMax(g.LastItemData.Rect.Min.x, g.LastItemData.Rect.Max.x - g.Style.FramePadding.x - button_size);
						float button_y = g.LastItemData.Rect.Min.y + g.Style.FramePadding.y;
						ImGuiID close_button_id = ImGui::GetIDWithSeed("#CLOSE", NULL, id);
						if (ImGui::CloseButton(close_button_id, ImVec2(button_x, button_y)))
							index_lightToRemove = i;
						g.LastItemData = last_item_backup;
					}


					ImGui::Columns(2, nullptr, false);

					//light type
					{
						ImGui::Text("type");
						ImGui::NextColumn();

						auto& scene = aven::getProject().getScene();

						auto type = aven::getProject().getScene().getLights()[i]->type;
						char const* items[] = { "Direction", "Point", "Spotlight" };
						int type_i = static_cast<int>(type);
						ImGui::SetNextItemWidth(-1);
						if (ImGui::Combo("##Type", &type_i, items, IM_ARRAYSIZE(items))) {
							Light light = *scene.getLights()[i];
							light.type = static_cast<Light::Type>(type_i);
							scene.replaceLight(i, light);
							aven::getProject().history.commit();
							aven::getProject().renderer.resetIterations();
						}

						ImGui::NextColumn();
					}

					auto lightType = aven::getProject().getScene().getLights()[i]->type;

					if (lightType == Light::Type::Point || lightType == Light::Type::SpotLight) {
						ImGui::Text("pos");
						ImGui::NextColumn();

						auto& scene = aven::getProject().getScene();
						vec3 pos = (vec3) scene.getLights()[i]->transformation.getPos();

						ImGui::SetNextItemWidth(-1);
						if (ImGui::DragFloat3(("##pos"+std::to_string(i)).c_str(), &pos[0])) {
							Light light = *scene.getLights()[i];
							light.transformation.setPos(pos);
							scene.replaceLight(i, light);
							aven::getProject().renderer.resetIterations();
						}
						if (ImGui::IsItemDeactivatedAfterEdit())
							aven::getProject().history.commit();
						ImGui::NextColumn();
					}

					if (lightType == Light::Type::Direction || lightType == Light::Type::SpotLight) {
						ImGui::Text("rotation");
						ImGui::NextColumn();

						auto& scene = aven::getProject().getScene();
						vec3 rotation = (vec3) scene.getLights()[i]->transformation.getRotation();

						ImGui::SetNextItemWidth(-1);
						if (ImGui::DragFloat3(("##rotation"+std::to_string(i)).c_str(), &rotation[0])) {
							Light light = *scene.getLights()[i];
							light.transformation.setRotation((Vector3<degrees>) rotation);
							scene.replaceLight(i, light);
							aven::getProject().renderer.resetIterations();
						}
						if (ImGui::IsItemDeactivatedAfterEdit())
							aven::getProject().history.commit();
						ImGui::NextColumn();
					}

					if (lightType == Light::Type::SpotLight) {
						ImGui::Text("falloff angle");
						ImGui::NextColumn();

						auto& scene = aven::getProject().getScene();
						float falloff_angle = scene.getLights()[i]->falloff_angle_degrees;

						ImGui::SetNextItemWidth(-1);
						if (ImGui::DragFloat(("##falloff angle"+std::to_string(i)).c_str(), &falloff_angle)) {
							Light light = *scene.getLights()[i];
							light.falloff_angle_degrees = falloff_angle;
							scene.replaceLight(i, light);
							aven::getProject().renderer.resetIterations();
						}
						if (ImGui::IsItemDeactivatedAfterEdit())
							aven::getProject().history.commit();
						ImGui::NextColumn();
					}

					{
						ImGui::Text("color");
						ImGui::NextColumn();

						auto& scene = aven::getProject().getScene();
						auto col = scene.getLights()[i]->color.getValue();
						ImGui::SetNextItemWidth(-1);
						if (ImGui::ColorEdit3(("##color"+std::to_string(i)).c_str(), &col[0])) {
							Light light = *scene.getLights()[i];
							light.color = col;
							scene.replaceLight(i, light);
							aven::getProject().renderer.resetIterations();
						}
						if (ImGui::IsItemDeactivatedAfterEdit())
							aven::getProject().history.commit();
						ImGui::NextColumn();
					}

					{
						ImGui::Text("intensity");
						ImGui::NextColumn();

						auto& scene = aven::getProject().getScene();
						auto inten = scene.getLights()[i]->intensity.getValue();

						ImGui::SetNextItemWidth(-1);
						if (ImGui::DragFloat(("##intensity"+std::to_string(i)).c_str(), &inten)) {
							Light light = *scene.getLights()[i];
							light.intensity = inten;
							scene.replaceLight(i, light);
							aven::getProject().renderer.resetIterations();
						}
						if (ImGui::IsItemDeactivatedAfterEdit())
							aven::getProject().history.commit();
						ImGui::NextColumn();
					}

					ImGui::Columns(1);
					ImGui::TreePop();
				}
				i++;
			}
			ImGui::TreePop();
			if (index_lightToRemove != -1) {
				aven::getProject().getScene().cloneLights();
				aven::getProject().getScene().removeLight(index_lightToRemove);
				aven::getProject().history.commit();
				aven::getProject().renderer.resetIterations();
			
			}
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

			static c_ivec3<1, Volume::MAX_VOLUME_LENGTH> size = ivec3( 128, 128, 128);
			display_imgui("size", size);

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
		if (!window_dev_isOpen)
			return;

		if (!ImGui::Begin("DevWindow", &window_dev_isOpen)) {
			ImGui::End();
			return;
		}

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
				{.name = "History: nbr undos",		.f = []() {	return std::to_string(aven::getProject().history.getUndoCount()); }},
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