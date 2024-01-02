#pragma once

#include <aven/volumeOperations/Tool.h>
#include <vector>

namespace aven {
	namespace toolManager {
		void init();
		void destroy();

		Tool* getSelectedTool();
		int getSelectedIndex();
		void select(int);
		std::vector<std::unique_ptr<Tool>>& getTools();
	}
}