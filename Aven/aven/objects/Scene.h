#pragma once

#include <aven/objects/Light.h>	
#include <aven/objects/Volume.h>
#include <aven/render/RenderSettings.h>
#include <expected>
#include <fstream>
#include <memory>
#include <vector>


namespace aven {
	
	class Scene {
	public:
		static const int MAX_NUMBER_LIGHTS = 5;	// don't forget to change inside of renderer_frag.glsl as well

	public:
		Scene(c_ivec3<1, Volume::MAX_VOLUME_LENGTH> size);
		Scene(RenderSettings&& renderSettings, Volume&&, std::vector<std::shared_ptr<Light const>>&& lights);
		
		static void serialize(std::ofstream&, Scene const&);
		static std::expected <Scene, std::string> deserialize(std::ifstream&);

		//lights related
		void replaceLight(int index, Light light);
		void cloneLights();
		void push_light(Light light);
		void removeLight(int index);
		std::vector<std::shared_ptr<Light const>> const getLights() const;
	
		std::shared_ptr<RenderSettings const> renderSettings;
		std::shared_ptr<Volume const> volume;

	private:
		std::vector<std::shared_ptr<Light const>> lights;	//private because only allow MAX_NUMBER_LIGHTS
	};

}