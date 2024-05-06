
#include <acore/GL/ShaderLoader.h>
#include <aven/volumeOperations/Filter.h>
#include <aven/objects/BrickPool.h>


namespace aven {

	Filter::Filter(std::string name, std::string path, std::vector<Property> properties)
		:name(name),
		program(gl::loadProgram({ {gl::ShaderType::Compute, path} })),
		properties(std::move(properties))
	{
		//...
	}

	VolumeData Filter::operator()(VolumeData const& src) const{
		auto size	= src.getSize();
		auto dst	= VolumeData(size);

		program.setInt3("volume_size", size);
		brickPool::bindSSBO_toBufferBase0();
		src.getSSBO().bindBufferBase(1);
		dst.getSSBO().bindBufferBase(2);
	
		for (Property const& p : properties)
			p.updateUniform(program);

		ivec3 nbrGroups = (size + ivec3(7, 7, 7)) / ivec3(8, 8, 8);
		program.setInt3("nbrGroups", nbrGroups);

		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		gl::dispatch(program, 1, 1, 1);	
		return dst;
	}


	std::string Filter::getName() const {
		return name;
	}


	bool Filter::hasProperties() const {
		return properties.size()>0;
	}

	std::vector<Property>& Filter::getProperties() {
		return properties;
	}



}