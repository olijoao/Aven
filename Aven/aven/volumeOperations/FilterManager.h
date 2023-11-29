#pragma once

#include <aven/volumeOperations/Filter.h>
#include <vector>

namespace aven {

	namespace filterManager {
		void init();
		void destroy();

		std::vector<Filter>& getFilters();
	}

}