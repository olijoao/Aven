#pragma once
#include <aven/util/math.h>
#include <aven/util/geo3d.h>
#include <aven/util/clamped.h>

namespace aven {
	class ViewPortCamera {
	public:
		ViewPortCamera(vec3 pos, vec3 target, float fov_degrees, ivec2 viewPortSize);

		void	rotateAroundTarget(float angle0, float angle1);
		
		void setFilmSize(ivec2);
		ivec2 getFilmSize() const;

		std::tuple<vec3, vec3, vec3> getCoordSystemofImagePlane();

		//pos in [0,1]
		Ray		createRay(vec2 pos);

		vec3	pos;
		vec3	target;

		clamped<float,	1.0f,	179.0f>		fov_degrees;
		clamped<float,	0.0001f,10.0f>		gamma					{2};
		clamped<int,	1,		1000>		maxSamples				{100};
		clamped<int,	1,		32>			nbrSamplesPerIteration	{4};
		clamped<int,	0,		16>			nbrBounces				{0};
		vec3								backgroundColor_sky		{1.0f, 1.0f, 1.0f};
		vec3								backgroundColor_ground	{0.0f, 0.0f, 0.0f};


	private:
		ivec2 filmSize;
	};
}
