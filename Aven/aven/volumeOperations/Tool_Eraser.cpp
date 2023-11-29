
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
		op = std::make_unique<OperationTool>(this, scene.volume->getSize(), vec3(0,0,0), properties.opacity, BlendMode::Erase);
		apply(scene, mouseInput);
	}


	void Tool_Eraser::end(Scene& scene, MouseInput const& mouseInput) {
		auto& op = aven::getProject().getCurrentToolOperation();
		assert(op != nullptr);
		assert(op->tool == this);

		auto& volume = scene.volume;
		auto blended = volumeOps::erase_mask(	volume->getTexture(),
												op->texture_mask,
												static_cast<float>(op->opacity.getValue()) / 255.0f);

		Volume newVolume = *volume.get();
		newVolume.setTexture(std::move(blended));
		scene.volume = std::make_shared<Volume>(newVolume);
		op = nullptr;
		assert(aven::getProject().getCurrentToolOperation() == nullptr);
	}

	
	void Tool_Eraser::apply(Scene& scene, MouseInput const& mouseInput) {
		assert(aven::getProject().getCurrentToolOperation() != nullptr);

		auto& op = aven::getProject().getCurrentToolOperation();
		assert(op);
		assert(op->tool == this);

		auto& volume = scene.volume;

		auto intersection = intersect(mouseInput.ray, volume->getBoundingBox());
		if (!intersection)
			return;
		vec3 intersection_pos = mouseInput.ray.pos + mouseInput.ray.dir * intersection.value().x - volume->pos;
		intersection_pos = intersection_pos + vec3(volume->getSize()) / 2;
		Ray ray = Ray(intersection_pos, mouseInput.ray.dir);

		ivec3 pos = volumeOps::raycast(volume->getTexture(), ray);
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


		volumeOps::paintStroke_Mirror(op->texture_mask, from, to, nbrIterations, properties.radius.getValue(), properties.brush, properties.flow.getValue(), properties.mirrored);

	}


	void Tool_Eraser::displayImGui() {
		display_imgui(*this);
	}

}