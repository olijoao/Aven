
#include <aven/render/ViewPortCamera.h>

namespace aven {

	ViewPortCamera::ViewPortCamera(vec3 pos, vec3 target, float fov_degrees, ivec2 filmSize)
		:pos(pos), target(target), fov_degrees(fov_degrees), filmSize(filmSize)
	{
		//..
	}

	void ViewPortCamera::rotateAroundTarget(float angle0, float angle1) {
		auto vup = vec3(0, 1, 0);
		auto forward = normalize(target - pos);
		auto right = normalize(cross(forward, vup));

		auto rotationAroundUp		= mat4_rotate_axis(vup, angle0);
		auto rotationAroundRight	= mat4_rotate_axis(right, angle1);
		auto pos4 =  (rotationAroundUp * rotationAroundRight * vec4(pos, 1));
		pos = { pos4.x, pos4.y, pos4.z };
	}


	ivec2 ViewPortCamera::getFilmSize() const{
		return filmSize;
	}

	void ViewPortCamera::setFilmSize(ivec2 size) {
		filmSize = size;
	}



	std::tuple<vec3, vec3, vec3> ViewPortCamera::getCoordSystemofImagePlane() {
		auto vup		= vec3(0,1,0);
		auto forward	= normalize(target-pos);
		auto right		= normalize(cross(forward, vup));
		auto up			= normalize(cross(right, forward));

		// center of imagePlane is 1 unit away from camera.pos
		auto aspectRatio			= static_cast<float>(filmSize.x) / filmSize.y;
		auto tan_HalfFovInRadians	= std::tan(radians(fov_degrees.getValue()) * 0.5f);
		auto imagePlane_halfWidth	= tan_HalfFovInRadians * aspectRatio;
		auto imagePlane_halfHeight	= tan_HalfFovInRadians;

		// right 
		auto imagePlane_right		= right * imagePlane_halfWidth;		// magnitude == width/2  of ImagePlane
		auto imagePlane_up			= up	* imagePlane_halfHeight;	// magnitude == height/2 of ImagePlane
		auto imagePlane_forward		= forward;
	
		return { imagePlane_right , imagePlane_up, imagePlane_forward };
	}



	//pos in [0,1]
	Ray	ViewPortCamera::createRay(vec2 pos) {
		pos = pos * 2 - 1;	// in [-1,1]

		auto coordSystem = this->getCoordSystemofImagePlane();

		vec3 posOnImagePlane	=	std::get<0>(coordSystem) * pos.x
								+	std::get<1>(coordSystem) * pos.y
								+	std::get<2>(coordSystem);
											
		return Ray(this->pos, normalize(posOnImagePlane));
	}
}