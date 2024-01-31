
#include <iostream>
#include <aven/gui/GUI.h>
#include <aven/Aven.h>

#include <thread>
using namespace std::chrono_literals;

#include <3rdParty/GL/glfw3.h> // order matters, first included in GUI.h

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


	double lastTime = glfwGetTime();
	while (aven::gui::isOpen()) {
		try {
			aven::gui::display();

			if (aven::getProject().getRenderer().hasReachedFinalIteration()) {
				while (glfwGetTime() < lastTime + 1.0/30.0) {
					std::this_thread::sleep_for(1ms);
				}
			}
			lastTime = glfwGetTime();
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