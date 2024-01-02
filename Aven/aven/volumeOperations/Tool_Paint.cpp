
#include <aven/Aven.h>
#include <aven/gui/ImGui_ToolProperties.h>
#include <aven/volumeOperations/Tool_Paint.h>
#include <aven/volumeOperations/VolumeOps.h>


namespace aven {
	
	Tool_Paint::Tool_Paint()
		:Tool("paint")
	{
		//..
	}
	
	
	void Tool_Paint::start(Scene& scene, MouseInput const& mouseInput) {
		assert(aven::getProject().getCurrentToolOperation() == nullptr);
		auto& op = aven::getProject().getCurrentToolOperation();
		op = std::make_unique<OperationTool>(this, scene.volume->getSize(), properties.opacity, BlendMode::Normal);
		apply(scene, mouseInput);
	}



	void Tool_Paint::end(Scene& scene, MouseInput const& mouseInput) {
		auto& op = aven::getProject().getCurrentToolOperation();
		assert(op != nullptr);		
		assert(op->tool == this);				
		
		auto& volume	= scene.volume;
		auto result		= volumeOps::blend(	volume->getVolumeData(), 
											std::move(op->volumeData), 
											volumeOps::BlendMode::Normal,
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



	void Tool_Paint::apply(Scene& scene, MouseInput const& mouseInput) {
		assert(aven::getProject().getCurrentToolOperation() != nullptr);
		
		auto& op = aven::getProject().getCurrentToolOperation();
		assert(op && op->tool == this);
		
		auto& volume = scene.volume;
		
		auto intersection = intersect(mouseInput.ray, volume->getBoundingBox());
		if (!intersection)
			return;
		vec3 intersection_pos = mouseInput.ray.pos + mouseInput.ray.dir * intersection.value().x - volume->pos;
		intersection_pos = intersection_pos + vec3(volume->getSize())/2;
		Ray ray = Ray(intersection_pos, mouseInput.ray.dir);
		ivec3 pos = volumeOps::raycast(volume->getVolumeData(), ray);

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
		
		volumeOps::paintStroke_Mirror(op->volumeData, from, to, nbrIterations, properties.radius.getValue(), properties.brush, vec4(aven::getForegroundColor(), properties.flow.getValue()), properties.mirrored);
	}


	void Tool_Paint::displayImGui() {
		display_imgui(*this);
	}

}



