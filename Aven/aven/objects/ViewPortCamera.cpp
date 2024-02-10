
#include <aven/objects/ViewPortCamera.h>


namespace aven {
	ViewPortCamera::ViewPortCamera(vec3 pos, vec3 target, c_float<1.0f, 179.0f> fov)
		:pos(pos), target(target), fov_degrees(fov), aspectRatio(16.0f/9.0f)
	{
		//...
	}


	void ViewPortCamera::serialize(std::ofstream& out, ViewPortCamera const& cam) {
		int version = 1;
		out.write((char*) &version,						sizeof(version));
		out.write((char*) &cam.pos,						sizeof(cam.pos));
		out.write((char*) &cam.target,					sizeof(cam.target));
		out.write((char*) &cam.fov_degrees,				sizeof(cam.fov_degrees));
	}


	std::expected<ViewPortCamera, std::string> ViewPortCamera::deserialize(std::ifstream& in) {
		int version;
		if (!in.read((char*)&version, sizeof(version)))
			return std::unexpected("error occured while deserializing ViewPortCamera.");
		if (version != 1)
			return std::unexpected("Could not deserialiize ViewPortCamera: version has to be 1");
		
		vec3 pos, target;
		float fov;

		if(    !in.read((char*)&pos, sizeof(pos))
			|| !in.read((char*)&target, sizeof(target))
			|| !in.read((char*)&fov, sizeof(fov))) 
		{
			return std::unexpected("error occured while deserializing ViewPortCamera.");
		}

		return ViewPortCamera (pos, target, fov);
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



	std::tuple<vec3, vec3, vec3> ViewPortCamera::getCoordSystemofImagePlane() const{
		auto vup		= vec3(0,1,0);
		auto forward	= normalize(target-pos);
		auto right		= normalize(cross(forward, vup));
		auto up			= normalize(cross(right, forward));

		// center of imagePlane is 1 unit away from camera.pos
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
	Ray	ViewPortCamera::createRay(vec2 pos) const{
		pos = pos * 2 - 1;	// in [-1,1]

		auto coordSystem = this->getCoordSystemofImagePlane();

		vec3 posOnImagePlane	=	std::get<0>(coordSystem) * pos.x
								+	std::get<1>(coordSystem) * pos.y
								+	std::get<2>(coordSystem);
											
		return Ray(this->pos, normalize(posOnImagePlane));
	}
}



