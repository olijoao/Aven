
#include <acore/GL/SSBO.h>
#include <acore/GL/ShaderLoader.h>
#include <assert.h>
#include <aven/objects/BrickPool.h>
#include <memory>
#include <stdexcept>


namespace aven {
	namespace {
		std::unique_ptr<gl::SSBO> ssbo_brickPool;
		std::unique_ptr<gl::Program> program_dtor;
		std::unique_ptr<gl::Program> program_transphere;
	}


	void brickPool::init() {
		assert(!ssbo_brickPool);
		ssbo_brickPool	= std::make_unique<gl::SSBO>(getSizeOfAllocatorInBytes());

		program_dtor = std::make_unique<gl::Program>(gl::loadProgram({ {gl::ShaderType::Compute, "shader/brickPool/volume_dtor.glsl"}}));
		program_transphere = std::make_unique<gl::Program>(gl::loadProgram({ {gl::ShaderType::Compute, "shader/brickPool/BrickPool_transphere.glsl"}}));

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


	brickPool::AllocatedBricks brickPool::alloc_cpu_bulk(unsigned int count)	{
		if (count == 0)
			return brickPool::AllocatedBricks({});
		if (count > nbrFreeBricks())
			throw std::runtime_error("brickPool out of memory.");

		gl::SSBO ssbo(count * 4);
		program_transphere->setUint("count", count);
		program_transphere->setUint("isAllocating", 1);
		bindSSBO_toBufferBase0();
		ssbo.bindBufferBase(1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		gl::dispatch(*program_transphere);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		std::vector<uint32_t> data;
		data.resize(count);
		ssbo.getData(&data[0]);
		return AllocatedBricks(std::move(data));
	}


	void brickPool::free_cpu_bulk(std::vector<uint32_t> bricks) {
		if (bricks.size() == 0)
			return;
		gl::SSBO ssbo(bricks.size()* 4);
		ssbo.setData(&bricks[0], bricks.size()*4);
	
		program_transphere->setUint("count", bricks.size());
		program_transphere->setUint("isAllocating", 0);
		bindSSBO_toBufferBase0();
		ssbo.bindBufferBase(1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		gl::dispatch(*program_transphere);
	}



	void brickPool::getBrickData(uint32_t brickIndex, std::array<uint32_t, BRICK_SIZE_IN_4BYTES>& data) {
		ssbo_brickPool->getDataRange(&data, 16U + brickIndex*BRICK_SIZE_IN_4BYTES*4, BRICK_SIZE_IN_4BYTES*4 );
	}

	void brickPool::setBrickData(uint32_t brickIndex, std::array<uint32_t, BRICK_SIZE_IN_4BYTES>& data) {
		ssbo_brickPool->setSubData(&data, 16U + brickIndex * BRICK_SIZE_IN_4BYTES * 4, BRICK_SIZE_IN_4BYTES * 4);
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
