
#include <aven/volumeOperations/ToolManager.h>


namespace aven {
	namespace toolManager {

		namespace {
			std::vector<Tool_Brush> tools;
			int selectionIndex = -1;	// -1: no selection
		}


		void init() {
			tools.emplace_back(Tool_Brush("paint", volumeOps::BlendMode::Normal, "img/icon_tool_brush_paint.png"));
			tools.emplace_back(Tool_Brush("eraser", volumeOps::BlendMode::Erase, "img/icon_tool_brush_eraser.png"));
			tools.emplace_back(Tool_Brush("paint_internal", volumeOps::BlendMode::Inside, "img/icon_tool_brush_paintInside.png"));
			tools.emplace_back(Tool_Brush("paint_internal", volumeOps::BlendMode::Color, "img/icon_tool_brush_paintInsideColor.png"));
		}

		void destroy() {
			tools.clear();
		}


		Tool_Brush* getSelectedTool() {
			assert(selectionIndex >= -1 && selectionIndex < static_cast<int>(tools.size()));
			if (selectionIndex == -1)
				return nullptr;
			return &tools[selectionIndex];
		}

		int getSelectedIndex() {
			return selectionIndex;
		}

		void select(int index) {
			assert(index >= -1 && index < tools.size());
			if (index >= -1 && index < tools.size())
				selectionIndex = index;
		}


		std::vector<Tool_Brush>& getTools() {
			return tools;
		}


	}
}