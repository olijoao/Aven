

#include <aven/objects/Scene.h>
#include <fstream>
  

namespace aven {

	Scene::Scene(c_ivec3<1, Volume::MAX_VOLUME_LENGTH> size)
		: volume(std::make_shared<Volume const>(Volume(size.getValue(), Transformation(vec3(0,size.getValue().y/2,0)), 1, 1, true, false))),
		renderSettings(std::make_shared<RenderSettings const>())
	{
		push_light({.intensity = 10});
	}
	

	Scene::Scene(RenderSettings&& rs, Volume&& vol, std::vector<std::shared_ptr<Light const>>&& lights)
		:volume(std::make_shared<Volume>(vol)),
		renderSettings(std::make_shared<RenderSettings>(rs)),
		lights(std::move(lights))
	{
		//..	
	}


	void Scene::serialize(std::ofstream& out, Scene const& scene) {
		RenderSettings::serialize(out, *scene.renderSettings);

		// lights
		int version = 1;
		out.write((char*)&version, sizeof(version));
		int numberOfLights = scene.lights.size();
		out.write((char*)&numberOfLights, sizeof(int));
		for (auto const& light : scene.lights) 
			Light::serialize(out, *light);

		// volume
		Volume::serialize(out, *scene.volume);
	}


	std::expected<Scene, std::string> Scene::deserialize(std::ifstream& in ) {	
		// rendersettings
		auto rs = RenderSettings::deserialize(in);
		if (!rs.has_value())
			return std::unexpected(rs.error());

		// lights
		std::vector<std::shared_ptr<Light const>> lights;
		int version;
		int nbrOfLights;
		if(	   !in.read((char*)&version, sizeof(version))
			|| !in.read((char*)&nbrOfLights, sizeof(int)))
			return std::unexpected("error occured while deserializing Light.");
		if (version != 1)
			return std::unexpected("Could not deserialiize Lights: version has to be 1");
		if (nbrOfLights > MAX_NUMBER_LIGHTS)
			return std::unexpected("Could not deserialiize Lights: nbrOfLights exceeds maximum number of lights allowed.");
		for (int i = 0; i < nbrOfLights; i++) {
			auto light = Light::deserialize(in);
			if (light.has_value())
				lights.push_back(std::make_shared<Light>(light.value()));
			else
				return std::unexpected(light.error());
		}

		// volume
		auto vol = Volume::deserialize(in);
		if (!vol.has_value())
			return std::unexpected(vol.error());

		return Scene(std::move(rs.value()), std::move(vol.value()), std::move(lights));
	}


	void Scene::replaceLight(int index, Light light) {
		if (index < 0 or index >= lights.size())
			throw std::out_of_range("tried to replace lights out of range");
		lights[index] = std::make_shared<Light>(light);
	}
	
	void Scene::cloneLights() {
		lights = lights;
	}

	void Scene::push_light(Light light) {
		if(lights.size() < MAX_NUMBER_LIGHTS)
			lights.push_back(std::make_shared<Light>(light));
	}

	void Scene::removeLight(int index) {
		if (index < 0 or index >= lights.size())
			return;
		lights.erase(lights.begin() + index);
	}

	std::vector<std::shared_ptr<Light const>> const Scene::getLights() const {
		return lights;
	}

}




