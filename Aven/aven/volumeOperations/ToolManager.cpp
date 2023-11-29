

#include <aven/volumeOperations/Tool_Paint.h>
#include <aven/volumeOperations/Tool_Eraser.h>
#include <aven/volumeOperations/ToolManager.h>
#include <memory>
#include <vector>


namespace aven {
	namespace toolManager {

		namespace {
			std::vector<std::unique_ptr<Tool>> tools;
			int selectionIndex = -1;	// -1: no selection
		}


		void init() {
			tools.emplace_back(std::make_unique<Tool_Paint>());
			tools.emplace_back(std::make_unique<Tool_Eraser>());
		}

		void destroy() {
			tools.clear();
		}


		Tool* getSelectedTool() {
			assert(selectionIndex >= -1 && selectionIndex < static_cast<int>(tools.size()));
			if (selectionIndex == -1)
				return nullptr;
			return tools[selectionIndex].get();
		}

		int getSelectedIndex() {
			return selectionIndex;
		}

		void select(int index) {
			assert(index >= -1 && index < tools.size());
			if (index >= -1 && index < tools.size())
				selectionIndex = index;
		}


		std::vector<std::unique_ptr<Tool>>& getTools() {
			return tools;
		}


	}
}