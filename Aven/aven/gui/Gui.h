#pragma once

#include <string>

namespace aven {
	namespace gui {
		void init(std::string const title, bool decorated = true);
		void destroy();

		void display();

		bool isOpen();
	}
}