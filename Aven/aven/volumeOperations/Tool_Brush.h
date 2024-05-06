#pragma once

#include <acore/GL/Texture.h>
#include <acore/geo3d.h>
#include <aven/objects/Scene.h>
#include <aven/volumeOperations/OperationTool.h>
#include <aven/volumeOperations/ToolBrushProperties.h>
#include <aven/volumeOperations/VolumeOps.h>
#include <memory>
#include <string>


namespace aven {
	class OperationTool;

	class Tool_Brush {
	public:
		Tool_Brush(std::string name, volumeOps::BlendMode blendmode, std::string const& path_icon ="");

		std::unique_ptr<OperationTool>	start(Scene&);
		void							end(Scene&,	vec2 const& mousePos_01, std::unique_ptr<OperationTool>);
		void							apply(Scene&, vec2 const& mousePosition_01);

		void displayImGui();

		std::string const& getName() const;
		std::optional<gl::Texture2D_rgba8u> const& getIcon() const;

		ToolBrushProperties properties;


	protected:
		std::string name;
		std::optional<gl::Texture2D_rgba8u> icon;
		volumeOps::BlendMode blendMode;
	};
}