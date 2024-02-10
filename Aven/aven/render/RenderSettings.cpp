#include "RenderSettings.h"

namespace aven {

	void RenderSettings::serialize(std::ofstream& out, RenderSettings const& rs) {
		int version = 1;
		out.write((char*) &version,						sizeof(version));

		out.write((char*) &rs.gamma,					sizeof(rs.gamma));
		out.write((char*) &rs.maxSamples,				sizeof(rs.maxSamples));
		out.write((char*) &rs.nbrSamplesPerIteration,	sizeof(rs.nbrSamplesPerIteration));
		out.write((char*) &rs.nbrBounces,				sizeof(rs.nbrBounces));
		out.write((char*) &rs.backgroundColor_sky,		sizeof(rs.backgroundColor_sky));
		out.write((char*) &rs.backgroundColor_ground,	sizeof(rs.backgroundColor_ground));
	}


	std::expected<RenderSettings, std::string> RenderSettings::deserialize(std::ifstream& in) {
		int version;
		if(!in.read((char*)&version, sizeof(version)))
			return std::unexpected("error occured while deserializing RenderSettings.");
		if (version != 1)
			return std::unexpected("Could not deserialiize RenderSettings: version has to be 1");

		float gamma;
		int maxSamples, nbrSamplesPerIteration, nbrBounces;
		vec3 bg_sky, bg_ground;

		if (   !in.read((char*)&gamma, sizeof(gamma))
			|| !in.read((char*)&maxSamples, sizeof(maxSamples))
			|| !in.read((char*)&nbrSamplesPerIteration, sizeof(nbrSamplesPerIteration))
			|| !in.read((char*)&nbrBounces, sizeof(nbrBounces))
			|| !in.read((char*)&bg_sky, sizeof(bg_sky))
			|| !in.read((char*)&bg_ground, sizeof(bg_ground))) 
		{
			return std::unexpected("error occured while deserializing RenderSettings.");
		}

		return RenderSettings{	.gamma = gamma,
								.maxSamples = maxSamples,
								.nbrSamplesPerIteration = nbrSamplesPerIteration,
								.nbrBounces	= nbrBounces,
								.backgroundColor_sky = bg_sky,
								.backgroundColor_ground = bg_ground};
	}


}
