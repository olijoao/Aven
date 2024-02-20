#include <aven/objects/Light.h>

namespace aven {

	void Light::serialize(std::ofstream& out, Light const& light) {
		Transformation::serialize(out, light.transformation);
		out.write((char*) &light.type,					sizeof(light.type));
		out.write((char*) &light.color,					sizeof(light.color));
		out.write((char*) &light.intensity,				sizeof(light.intensity));
		out.write((char*) &light.falloff_angle_degrees,	sizeof(light.falloff_angle_degrees));

	}


	std::expected<Light, std::string> Light::deserialize(std::ifstream& in) {
		auto transformation = Transformation::deserialize(in);
		if (!transformation.has_value())
			return std::unexpected("Error occured while deserializing Light." + transformation.error());

		vec3 color;
		float intensity, falloff_angle_degrees;
		Type type;
		if (   !in.read((char*)&type,					sizeof(type))
			|| !in.read((char*)&color,					sizeof(color))
			|| !in.read((char*)&intensity,				sizeof(intensity))
			|| !in.read((char*)&falloff_angle_degrees,	sizeof(falloff_angle_degrees))
			) 
		{
			return std::unexpected("Error occured while deserializing Light.");
		}

		return Light{	.transformation			= transformation.value(),
						.type					= type,
						.color					= color,
						.intensity				= intensity,
						.falloff_angle_degrees	= falloff_angle_degrees	};
	}



}