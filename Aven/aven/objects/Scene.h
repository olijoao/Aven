#pragma once

#include <aven/objects/Volume.h>
#include <fstream>
#include <memory>

namespace aven {

	class Scene {
	public:
		Scene(clamped<ivec3, 1, 256> size);
		Scene(Volume&&);
		
		static void serialize(std::ofstream&, Scene const&);
		static Scene deserialize(std::ifstream&);
		
		std::shared_ptr<Volume const> volume;
	private:
		//..
	};


}