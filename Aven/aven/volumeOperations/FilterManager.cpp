
#include <aven/volumeOperations/FilterManager.h>
#include <memory>



namespace aven {
	namespace filterManager {
	
		namespace {
			std::vector<Filter> filters;
		}


		void init() {
			filters.push_back({"clear","shader/filter/clear.glsl"});

			{
				std::vector<Property> properties;	
				properties.push_back({ "mask",			"mask",			true});
				properties.push_back({ "color",			"color",		vec3(1, 1, 1) });
				filters.push_back({ "fill color",		"shader/filter/point_fillColor.glsl", std::move(properties) });
			}

			filters.push_back({"invert rgb","shader/filter/point_invertRGB.glsl"});

		}

		void destroy() {
			filters.clear();
		}

		std::vector<Filter>& getFilters() {
			return filters;
		}


	}
}