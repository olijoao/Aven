#pragma once

#include <memory>
#include <aven/objects/VolumeData.h>

namespace aven {

	namespace brickPool {
		const unsigned int BRICK_NULL				= 1 << 31;
		const unsigned int BRICK_SIZE_IN_4BYTES		= 8 * 8 * 8;
		const unsigned int CAPACITY					= 1024 * 1024 * 1024 / (BRICK_SIZE_IN_4BYTES*4);	// 1 Gib

		void init();
		void destroy();
	
		void bindSSBO_toBufferBase0();

		constexpr unsigned int getSizeOfAllocatorInBytes();
		int nbrFreeBricks();

		void dealloc(VolumeData& data);
	};
}