#pragma once

#include <acore/GL/SSBO.h>
#include <acore/math.h>
#include <memory> 


namespace aven {
	class VolumeData {
	public:
		VolumeData(ivec3 size);
		~VolumeData();
		VolumeData(VolumeData&&) = default;

		ivec3 getSize() const;
		gl::SSBO& getSSBO() ;
		gl::SSBO const& getSSBO() const;

	private:
		ivec3 size;
		gl::SSBO ssbo;
	};
}