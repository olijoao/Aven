
#include <aven/Aven.h>
#include <aven/gui/ImGui_ToolProperties.h>
#include <aven/volumeOperations/Tool_Eraser.h>
#include <aven/volumeOperations/VolumeOps.h>


namespace aven {
	Tool_Eraser::Tool_Eraser()
		:Tool("Eraser") 
	{	
		//...
	}

	void Tool_Eraser::start(Scene& scene, MouseInput const& mouseInput) {
		auto& op = aven::getProject().getCurrentToolOperation();
		assert(op == nullptr);
		op = std::make_unique<OperationTool>(this, scene.volume->getSize(), properties.opacity, BlendMode::Erase);
		apply(scene, mouseInput);
	}


	void Tool_Eraser::end(Scene& scene, MouseInput const& mouseInput) {
		auto& op = aven::getProject().getCurrentToolOperation();
		assert(op != nullptr && op->tool == this);

		auto& volume = scene.volume;
		auto result = volumeOps::blend(volume->getVolumeData(),
			std::move(op->volumeData),
			volumeOps::BlendMode::Erase,
			static_cast<float>(op->opacity.getValue()) / 255.0f);

		scene.volume = std::make_shared<Volume>(
			volume->getVolumeData().getSize(),
			volume->pos,
			volume->sigma_t,
			volume->density,
			volume->stepSize,
			std::make_shared<VolumeData>(std::move(result))
			);

		op = nullptr;
		assert(aven::getProject().getCurrentToolOperation() == nullptr);
	}


	
	void Tool_Eraser::apply(Scene& scene, MouseInput const& mouseInput) {
		assert(aven::getProject().getCurrentToolOperation() != nullptr);
		
		auto& op = aven::getProject().getCurrentToolOperation();
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
			from = op->brushStroke_lastPos.value();	
			to = pos;
			float	distanceToCover = distance(from, to) + op->brushStroke_distanceRemaining;
			assert(spacing_absolute != 0);
			nbrIterations = static_cast<int>(distanceToCover / spacing_absolute);
			op->brushStroke_distanceRemaining = distanceToCover - static_cast<float>(nbrIterations) * spacing_absolute;
		}
		else {
			from = pos;
			to = pos;
			nbrIterations = 1;
			op->brushStroke_distanceRemaining = 0;
		}
		op->brushStroke_lastPos = to;	
		
		volumeOps::paintStroke(op->volumeData, from, to, nbrIterations, vec4(0,0,0, static_cast<float>(properties.flow.getValue())/255), properties);
	}


	void Tool_Eraser::displayImGui() {
		display_imgui(*this);
	}

}