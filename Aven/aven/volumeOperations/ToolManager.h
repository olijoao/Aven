#pragma once

#include <aven/volumeOperations/Tool_Brush.h>
#include <vector>

namespace aven {
	namespace toolManager {
		void init();
		void destroy();

		Tool_Brush* getSelectedTool();
		int getSelectedIndex();
		void select(int);
		std::vector<Tool_Brush>& getTools();
	}
}