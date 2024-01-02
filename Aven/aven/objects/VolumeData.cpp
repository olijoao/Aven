
#include <aven/objects/BrickPool.h>
#include <aven/objects/VolumeData.h>

namespace aven {
	VolumeData::VolumeData(ivec3 size)
		:size(size),
		ssbo(size.x* size.y* size.z * 4) 
	{
		glClearNamedBufferData(ssbo.getName(), GL_R32UI, GL_RED, GL_UNSIGNED_INT, &brickPool::BRICK_NULL);
		assert(!glGetError());
	}

	VolumeData::~VolumeData() {
		if(ssbo.getName() != 0)
			brickPool::dealloc(*this);
	}

	ivec3 VolumeData::getSize() const{
		return size;
	}

	gl::SSBO const& VolumeData::getSSBO() const{
		return ssbo;
	}

}