#pragma once

#include <acore/GL/Program.h>
#include <acore/Property.h>
#include <aven/objects/VolumeData.h>
#include <string>


namespace aven {
	class Filter {
	public:
		Filter(std::string name, std::string path, std::vector<Property> properties = {});

		VolumeData operator()(VolumeData const&) const;

		std::string getName() const;
		bool hasProperties() const;
		std::vector<Property>& getProperties();

	private:
		std::string				name;
		gl::Program				program;
		std::vector<Property>	properties;
	};
}