#pragma once

#include <aven/scene/Volume.h>
#include <fstream>
#include <memory>

namespace aven {

	class Scene {
	public:
		Scene(clamped<ivec3, 1, 256> size);

		std::shared_ptr<Volume const> volume;
		void serialize(std::ofstream&);
	private:
		//..
	};
}