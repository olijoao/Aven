
#include <iostream>
#include <aven/gui/GUI.h>
#include <aven/Aven.h>


int main() {
	try {
		aven::gui::init("Aven");
		aven::init();
	}
	catch (std::exception const& se) {
		std::cout << std::endl;
		std::cout << "--- Error in initialization of program " << std::endl;
		std::cout << se.what() << std::endl;
		std::cout << "press Enter to exit" << std::endl;
		std::cin.get();
		return 0;
	}


	while (aven::gui::isOpen()) {
		try {
			aven::gui::display();
		}
		catch (std::exception const& se) {
			std::cout << std::endl;
			std::cout << "Exception caught in main loop." << std::endl;
			std::cout << se.what() << std::endl;
		}
		catch (...) {
			std::cout << "undefined exception caught" << std::endl;
		}
	}


	try {
		aven::destroy();
		aven::gui::destroy();
	}
	catch (std::exception const& se) {
		std::cout << std::endl;
		std::cout << "--- Error in deconstruction of program " << std::endl;
		std::cout << se.what() << std::endl;
		std::cout << "press Enter to exit" << std::endl;
		std::cin.get();
		return 0;
	}

}