#pragma once

#include <aven/util/geo3d.h>
#include <aven/gui/MouseInput.h>
#include <aven/scene/Scene.h>
#include <string>


namespace aven {
	class Tool {
	public:
		Tool(std::string name);

		virtual void start(Scene&,	MouseInput const&)	= 0;
		virtual void end(Scene&,	MouseInput const&)	= 0;
		virtual void apply(Scene&,	MouseInput const&)	= 0;

		virtual void displayImGui() = 0;

		std::string const& getName() const;

	protected:
		std::string name;
	};
}