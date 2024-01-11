#pragma once

#include <aven/gui/MouseInput.h>
#include <aven/objects/Scene.h>
#include <aven/util/geo3d.h>
#include <aven/volumeOperations/ToolBrushProperties.h>
#include <aven/volumeOperations/VolumeOps.h>
#include <string>

namespace aven {
	class Tool_Brush {
	public:
		Tool_Brush(std::string name, volumeOps::BlendMode blendmode);

		void start(Scene&,	MouseInput const&);
		void end(Scene&,	MouseInput const&);
		void apply(Scene&,	MouseInput const&);

		void displayImGui();

		std::string const& getName() const;

		ToolBrushProperties properties;

	protected:
		std::string name;
		volumeOps::BlendMode blendMode;
	};
}