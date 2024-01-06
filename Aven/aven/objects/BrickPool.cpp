
#include <assert.h>
#include <aven/GL/SSBO.h>
#include <aven/GL/ShaderLoader.h>
#include <aven/objects/BrickPool.h>
#include <memory>

namespace aven {
	namespace {
		std::unique_ptr<gl::SSBO> ssbo_brickPool;
		std::unique_ptr<gl::Program> program_dtor;
	}

	void brickPool::init() {
		assert(!ssbo_brickPool);
		ssbo_brickPool	= std::make_unique<gl::SSBO>(getSizeOfAllocatorInBytes());

		program_dtor = std::make_unique<gl::Program>(gl::loadProgram({ {gl::ShaderType::Compute, "shader/brickPool/volume_dtor.glsl"}}));

		auto program_Ctor = gl::loadProgram({ {gl::ShaderType::Compute, "shader/brickPool/BrickPool_ctor.glsl"}});
		program_Ctor.setUint("capacity", CAPACITY);
		bindSSBO_toBufferBase0();
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		gl::dispatch(program_Ctor, 1, 1, 1);

	}


	void brickPool::destroy() {
		ssbo_brickPool	= nullptr;
	}


	void brickPool::bindSSBO_toBufferBase0() {
		ssbo_brickPool->bindBufferBase(0);
	}

	constexpr unsigned int brickPool::getSizeOfAllocatorInBytes() {
		unsigned int nbrBytes = 4 * 4									// capacity + nbrFreeNodes info
								+ CAPACITY * BRICK_SIZE_IN_4BYTES * 4U
								+ CAPACITY * 4U;	//ref count
		return nbrBytes;
	}


	int brickPool::nbrFreeBricks() {
		assert(ssbo_brickPool);

		unsigned int dataOut;
		ssbo_brickPool->getDataRange(&dataOut, 0, 1*4);
		return dataOut;
	}


	void brickPool::dealloc(VolumeData& data) {
		bindSSBO_toBufferBase0();
		data.getSSBO().bindBufferBase(2);

		ivec3 nbrGroups = (data.getSize() + ivec3(7, 7, 7)) / ivec3(8, 8, 8);
		program_dtor->setUint("nbrBricks", nbrGroups.x * nbrGroups.y * nbrGroups.z);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		gl::dispatch(*program_dtor, 1, 1, 1);
	}
}
