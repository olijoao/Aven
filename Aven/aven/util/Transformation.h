#pragma once

#include <aven/util/math.h>
#include <fstream>
#include <expected>

namespace aven {
	class Transformation {
	public:
		
		Transformation() = default;
		Transformation(vec3 pos, Vector3<degrees> rotation = { 0 }, vec3 scale = {1});

		void setPos(vec3 pos);
		void setRotation(Vector3<degrees> rotation);
		void setScale(vec3 scale);

		vec3				getPos()		const;
		Vector3<degrees>	getRotation()	const;
		vec3				getScale()		const;

		mat4 getMatrix() const;

		static void serialize(std::ofstream& out, Transformation const& light);
		static std::expected<Transformation, std::string> deserialize(std::ifstream& in);

	private:
		vec3				pos			{ 0, 0, 0 };
		Vector3<degrees>	rotation	{ 0, 0, 0 };
		vec3				scale		{ 1, 1, 1 };

		mat4				cachedMat;

		void calcMatrix();	
	};
}
