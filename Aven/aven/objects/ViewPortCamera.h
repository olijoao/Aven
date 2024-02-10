#pragma once
#include <aven/util/math.h>
#include <aven/util/geo3d.h>
#include <aven/util/clamped.h>
#include <expected>
#include <fstream>


namespace aven {
	class ViewPortCamera {
	public:
		ViewPortCamera(vec3 pos, vec3 target, c_float<1.0f, 179.0f>);

		static void serialize(std::ofstream&, ViewPortCamera const&);
		static std::expected<ViewPortCamera, std::string> deserialize(std::ifstream&);

		void rotateAroundTarget(float angle0, float angle1);
		
		std::tuple<vec3, vec3, vec3> getCoordSystemofImagePlane() const;

		//pos in [0,1]
		Ray		createRay(vec2 pos) const;

		vec3					pos;	
		vec3					target;	
		c_float<1.0f, 179.0f>	fov_degrees				{90};
	
		float aspectRatio;	//set by Renderer on resize()

	private:
		//..
	};
}
