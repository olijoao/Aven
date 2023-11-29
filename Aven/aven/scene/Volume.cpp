
#include <aven/scene/Volume.h>


namespace aven {

	Volume::Volume(	gl::Texture3D_rgba8u texture, 
					vec3 pos,
					clamped<float, 0.001f, 100000.0f> sigma_t,
					clamped<float, 0.001f, 100000.0f> density,
					clamped<float, 0.25f, 5.0f>			stepSize) :
		texture(std::make_shared<gl::Texture3D_rgba8u>(std::move(texture))),
		size(texture.getSize()), 
		sigma_t(sigma_t), 
		density(density),
		stepSize(stepSize),
		pos(pos)
	{
		//...
	}


	Volume::Volume(	ivec3 size, 
					vec3 pos,
					clamped<float, 0.001f, 100000.0f>	sigma_t,
					clamped<float, 0.001f, 100000.0f>	density,
					clamped<float, 0.25f, 5.0f>			stepSize) :
		texture(std::make_shared<gl::Texture3D_rgba8u>(size)),
		size(size), 
		sigma_t(sigma_t),
		density(density),
		stepSize(stepSize),
		pos(pos)
	{
		//..
	}


	void Volume::setTexture(gl::Texture3D_rgba8u&& tex) {
		texture = std::make_unique<gl::Texture3D_rgba8u>(std::move(tex));
	}


	ivec3 Volume::getSize() const {
		return size;
	}


	gl::Texture3D_rgba8u const& Volume::getTexture() const {
		return *texture.get();
	}


	AABB<float> Volume::getBoundingBox() const {
		vec3 halfSize = vec3(size) / 2;
		return { pos-halfSize, {pos+halfSize}};
	}

}