#pragma once

#include <aven/GL/Texture.h>
#include <optional>
#include <string>


namespace aven {
	namespace imageIO {
		std::optional<gl::Texture2D_rgba8u> load(std::string const& path);
	}
}