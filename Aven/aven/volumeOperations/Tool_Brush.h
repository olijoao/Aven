#pragma once

#include <aven/gui/MouseInput.h>
#include <aven/objects/Scene.h>
#include <aven/util/geo3d.h>
#include <aven/volumeOperations/OperationTool.h>
#include <aven/volumeOperations/ToolBrushProperties.h>
#include <aven/volumeOperations/VolumeOps.h>
#include <memory>
#include <string>


namespace aven {
	class OperationTool;

	class Tool_Brush {
	public:
		Tool_Brush(std::string name, volumeOps::BlendMode blendmode);

		std::unique_ptr<OperationTool>	start(Scene&);
		void							end(Scene&,	MouseInput const&, std::unique_ptr<OperationTool>);
		void							apply(Scene&,	MouseInput const&);

		void displayImGui();

		std::string const& getName() const;

		ToolBrushProperties properties;

	protected:
		std::string name;
		volumeOps::BlendMode blendMode;
	};
}