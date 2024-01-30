#pragma once

#include <aven/objects/VolumeData.h>
#include <memory>
#include <vector>


namespace aven {

	namespace brickPool {
		// forward declaration
		void free_cpu_bulk(std::vector<uint32_t>);

		// RAII, so we don't leak bricks in case of exception
		class AllocatedBricks {
		public:
			AllocatedBricks(std::vector<uint32_t>&& bricks) noexcept 
				: bricks(bricks) 
			{ /*...*/  }
			~AllocatedBricks() {
				free_cpu_bulk(bricks);
			}
			uint32_t pop() {
				auto brick = bricks.back();
				bricks.pop_back();
				return brick;
			}
			auto size() {
				return bricks.size();
			}
		private:
			std::vector<uint32_t> bricks;
		};



		const unsigned int BRICK_NULL				= 1 << 31;
		const unsigned int BRICK_LENGTH				= 8;
		const unsigned int BRICK_SIZE_IN_4BYTES		= BRICK_LENGTH * BRICK_LENGTH * BRICK_LENGTH;
		const unsigned int CAPACITY					= 1024 * 1024 * 1024 / (BRICK_SIZE_IN_4BYTES*4);	// 1 Gib (reference count not included)

		void init();
		void destroy();

		void bindSSBO_toBufferBase0();

		AllocatedBricks alloc_cpu_bulk(unsigned int count);
		void free_cpu_bulk(std::vector<uint32_t> bricks);
		
		void getBrickData(uint32_t brickIndex, std::array<uint32_t, BRICK_SIZE_IN_4BYTES>& data);
		void setBrickData(uint32_t brickIndex, std::array<uint32_t, BRICK_SIZE_IN_4BYTES>& data);

		constexpr unsigned int getSizeOfAllocatorInBytes();
		int nbrFreeBricks();

		void dealloc(VolumeData& data);
	};
}