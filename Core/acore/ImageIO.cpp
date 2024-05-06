

#include <acore/ImageIO.h> 
#include <iostream>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include <3rdParty/stb/stb_image.h>


namespace aven {
	namespace imageIO {

		std::optional<gl::Texture2D_rgba8u> load(std::string const& path) {
			int x = 0;
			int y = 0;
			int n = 0;

			std::unique_ptr<unsigned char, decltype(&stbi_image_free)> data(
							stbi_load(path.c_str(), &x, &y, &n, 4), 
							stbi_image_free);

			if (data.get() == NULL) {
				std::cout << "Error in imageIO: coult not load image\"" + path +"\"" << std::endl;
				return {};
			}
			return gl::Texture2D_rgba8u ({ x, y }, data.get());	

		}

	}
}