#pragma once

#include <aven/GL/Program.h>
#include <aven/util/Property.h>
#include <aven/scene/Volume.h>
#include <string>


namespace aven {
	class Filter {
	public:
		Filter(std::string name, std::string path, std::vector<Property> properties = {});

		std::shared_ptr<Volume const> operator()(std::shared_ptr<Volume const>) const;		

		std::string getName() const;
		bool hasProperties() const;
		std::vector<Property>& getProperties();

	private:
		gl::Program				program;
		std::string				name;
		std::vector<Property>	properties;
	};
}