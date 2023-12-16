
#include <aven/util/clamped.h>
#include <aven/volumeOperations/FilterManager.h>
#include <memory>



namespace aven {
	namespace filterManager {

		namespace {
			std::vector<Filter> filters;
		}

		void init() {
			filters.push_back({ "clear","shader/filter/clear.glsl" });
			filters.push_back({ "render_random","shader/filter/renderRandom.glsl" });

			{
				std::vector<Property> props;
				props.push_back({ "mask", "mask", true });
				props.push_back({ "color", "color", vec3(1, 1, 1) });
				filters.push_back({ "fill color", "shader/filter/point_fillColor.glsl", std::move(props) });
			}

			{
				std::vector<Property> props;
				props.push_back({ "contrast", "contrast", 1.0f });
				props.push_back({ "brightness", "brightness", clamped<float, -1.0f, 1.0f>(0.0f) });
				filters.push_back({ "contrast brightness", "shader/filter/brightness_contrast.glsl", std::move(props) });
			}

			{
				std::vector<Property> props;
				props.push_back({ "scale", "scale", vec3(1, 1, 1) });
				props.push_back({ "translate", "translate", ivec3(0,0,0) });
				filters.push_back({ "transform",	"shader/filter/transform.glsl", std::move(props) });
			}

			filters.push_back({ "invert rgb","shader/filter/point_invertRGB.glsl" });

			{
				std::vector<Property> props;
				props.push_back({ "hue",		"offset_hue",			0 });
				props.push_back({ "saturation",	"offset_saturation",	0 });
				props.push_back({ "value",		"offset_value",			0 });
				filters.push_back({ "adjust hsv",	"shader/filter/adjustHSV.glsl", std::move(props) });
			}

			{
				std::vector<Property> props;
				props.push_back({ "offset", "offset", ivec3(0,0,0) });
				filters.push_back({ "adjust hsv",	"shader/filter/adjustRGB.glsl", std::move(props) });
			}

		}


		void destroy() {
			filters.clear();
		}

		std::vector<Filter>& getFilters() {
			return filters;
		}


	}
}