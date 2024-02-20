
#include <aven/aven.h>
#include <aven/gui/ImGui_ToolProperties.h>
#include <aven/util/ImageIO.h>
#include <aven/volumeOperations/Tool_Brush.h>


namespace aven {

	Tool_Brush::Tool_Brush(std::string name, volumeOps::BlendMode blendMode, std::string const& path_icon)
		:name(name), blendMode(blendMode)
	{
		if (path_icon != "") {
			icon = imageIO::load(path_icon);
			icon->setMinFilter(gl::Tex_Min_Filter::LINEAR);
		}
	}


	std::string const& Tool_Brush::getName() const {
		return name;
	}

	std::optional<gl::Texture2D_rgba8u> const& Tool_Brush::getIcon() const {
		return icon;
	}

	std::unique_ptr<OperationTool> Tool_Brush::start(Scene& scene) {
		return  std::make_unique<OperationTool>(this, scene.volume->getSize(), properties.opacity, blendMode);
	}


	void Tool_Brush::end(Scene& scene, MouseInput const&, std::unique_ptr<OperationTool> op) {
		auto& volume = scene.volume;
		auto result = volumeOps::blend(volume->getVolumeData(),
			std::move(op->volumeData),
			op->blendMode,
			static_cast<float>(op->opacity.getValue()) / 255.0f);

		scene.volume = std::make_shared<Volume>(
			volume->getVolumeData().getSize(),
			volume->transformation,
			volume->density,
			volume->stepSize,
			volume->isRendering_BondingBox,
			volume->renderingMode_Hybrid,
			std::make_shared<VolumeData>(std::move(result))
		);
	}



	void Tool_Brush::apply(Scene& scene, MouseInput const& mouseInput) {
		assert(aven::getProject().currentToolOperation != nullptr);
		
		auto& op = aven::getProject().currentToolOperation;
		assert(op && op->tool == this);
		
		auto& volume = scene.volume;
		
		std::optional<ivec3> optional_pos = volume->intersect(mouseInput.ray);
		if (!optional_pos.has_value())
			return;
		auto pos = optional_pos.value();

		float spacing_absolute = static_cast<float>(properties.radius.getValue()) * 2.0f * properties.spacing.getValue();
		
		ivec3 from, to;
		int nbrIterations;
		if (op->brushStroke_lastPos.has_value()) {
			from			= op->brushStroke_lastPos.value();
			to				= pos;
			float	distanceToCover = distance(from, to) + op->brushStroke_distanceRemaining;
			assert(spacing_absolute != 0);
			nbrIterations	= static_cast<int>(distanceToCover/spacing_absolute);
			op->brushStroke_distanceRemaining = distanceToCover - static_cast<float>(nbrIterations) * spacing_absolute;
		}else {
			from = pos;
			to	 = pos;
			nbrIterations = 1;
			op->brushStroke_distanceRemaining = 0;
		}
		op->brushStroke_lastPos = to;
	
		vec3 color = aven::getForegroundColor();
		if (blendMode == volumeOps::BlendMode::Erase)
			color = vec3(0, 0, 0);

		volumeOps::paintStroke(op->volumeData, from, to, nbrIterations, vec4(color, static_cast<float>(properties.flow.getValue())/255), properties);

	}


	void Tool_Brush::displayImGui() {
		display_imgui(*this);
	}


}