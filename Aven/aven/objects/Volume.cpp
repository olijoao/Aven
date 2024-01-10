
#include <aven/objects/Volume.h>
#include <aven/objects/BrickPool.h>
#include <aven/util/geo3d.h>
#include <aven/volumeOperations/VolumeOps.h>

namespace aven {

	Volume::Volume(	ivec3 size, 
					vec3 pos,
					clamped<float, 0.001f, 100000.0f>	sigma_t,
					clamped<float, 0.001f, 100000.0f>	density,
					clamped<float, 0.25f, 5.0f>			stepSize,
					std::shared_ptr<VolumeData> volumeData):
		sigma_t(sigma_t),
		density(density),
		stepSize(stepSize),
		pos(pos)
	{
		if (volumeData == nullptr)
			data = std::make_shared<VolumeData>(size);
		else
			data = volumeData;
	}


	ivec3 Volume::getSize() const {
		return data->getSize();
	}


	gl::SSBO const& Volume::getSSBO() const {
		return data->getSSBO();
	}

	VolumeData const& Volume::getVolumeData() const {
		return *data;
	}

	void Volume::setVolumeData(VolumeData&& data) {
		this->data = std::make_shared<VolumeData>(std::move(data));
	}

	AABB<float> Volume::getBoundingBox() const {
		vec3 halfSize = vec3(getSize()) / 2;
		return { pos-halfSize, {pos+halfSize}};
	}


	std::optional<ivec3> Volume::intersect(Ray const& ray) const {
		auto intersection = aven::intersect(ray, getBoundingBox());
		if (!intersection)
			return {};

		vec3 intersection_pos = ray.pos + ray.dir * intersection.value().x - this->pos;
		intersection_pos = intersection_pos + vec3(this->getSize()) / 2;
		Ray rayInternal = Ray(intersection_pos, ray.dir);
		return volumeOps::raycast(this->getVolumeData(), rayInternal);
	}

}