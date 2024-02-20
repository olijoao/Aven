#include <aven/util/Transformation.h>

namespace aven {

	Transformation::Transformation(vec3 pos, Vector3<degrees> rotation, vec3 scale) 
		:pos(pos), rotation(rotation), scale(scale)
	{
	
	}

	void Transformation::setPos(vec3 pos) {
		this->pos = pos;
		calcMatrix();
	}

	void Transformation::setRotation(Vector3<degrees> rotation) {
		this->rotation = rotation;
		calcMatrix();
	}

	void Transformation::setScale(vec3 scale) {
		this->scale = scale;
		calcMatrix();
	}

	vec3				Transformation::getPos()		const { return pos; }
	Vector3<degrees>	Transformation::getRotation()	const { return rotation; }
	vec3				Transformation::getScale()		const { return scale; }


	mat4 Transformation::getMatrix() const {
		return cachedMat;
	}	


	void Transformation::serialize(std::ofstream& out, Transformation const& transformation) {
		out.write((char*) &transformation.pos,		sizeof(transformation.pos));
		out.write((char*) &transformation.rotation,	sizeof(transformation.rotation));
		out.write((char*) &transformation.scale,	sizeof(transformation.scale));
	}


	std::expected<Transformation, std::string> Transformation::deserialize(std::ifstream& in) {
		Vector3<degrees> rotation;
		vec3 pos, scale;
		if (   !in.read((char*)&pos,		sizeof(pos))
			|| !in.read((char*)&rotation,	sizeof(rotation))
			|| !in.read((char*)&scale,		sizeof(scale))) 
		{
			return std::unexpected("error occured while deserializing Transformation.");
		}

		return Transformation{ pos, rotation, scale};
	}


	void Transformation::calcMatrix() {
		cachedMat = mat4_translate(pos) * mat4_rotateXYZ(rotation) * mat4_scale(scale);
	}

}