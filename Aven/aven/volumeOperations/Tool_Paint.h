#pragma once

#include <aven/volumeOperations/Tool.h>
#include <aven/volumeOperations/ToolBrushProperties.h>


namespace aven {
	class Tool_Paint: public Tool{
	public:
		Tool_Paint();

		void start(Scene& scene,	MouseInput const&)	override;
		void end(Scene& scene,		MouseInput const&)	override;
		void apply(Scene& scene,	MouseInput const&)	override;

		void displayImGui() override;

		ToolBrushProperties properties;

	private:
		//...
	};
}


