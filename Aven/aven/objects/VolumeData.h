#pragma once

#include <aven/GL/SSBO.h>
#include <aven/util/math.h>
#include <memory> 


namespace aven {
	class VolumeData {
	public:
		VolumeData(ivec3 size);
		~VolumeData();
		VolumeData(VolumeData&&) = default;

		ivec3 getSize() const;
		gl::SSBO const& getSSBO() const;

	private:
		ivec3 size;
		gl::SSBO ssbo;
	};
}