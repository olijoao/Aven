#pragma once

#include <aven/objects/Volume.h>
#include <aven/render/RenderSettings.h>
#include <expected>
#include <fstream>
#include <memory>

namespace aven {

	class Scene {
	public:
		Scene(c_ivec3<1, Volume::MAX_VOLUME_LENGTH> size);
		Scene(RenderSettings&& renderSettings, Volume&&);
		
		static void serialize(std::ofstream&, Scene const&);
		static std::expected <Scene, std::string> deserialize(std::ifstream&);
		
		std::shared_ptr<RenderSettings const> renderSettings;
		std::shared_ptr<Volume const> volume;
	private:
		//..
	};


}