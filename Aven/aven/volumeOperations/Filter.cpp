
#include <aven/GL/ShaderLoader.h>
#include <aven/volumeOperations/Filter.h>



namespace aven {

	Filter::Filter(std::string name, std::string path, std::vector<Property> properties)
		:name(name),
		program(gl::loadProgram({ {gl::ShaderType::Compute, path} })),
		properties(std::move(properties))
	{
		//...
	}


	std::shared_ptr<Volume const> Filter::operator()(std::shared_ptr<Volume const> src) const{
		assert(src);

		auto size		= src->getSize();
		auto texture	= gl::Texture3D_rgba8u(size);

		src->getTexture().bindToImageTextureUnit_readOnly(0);
		texture.bindToImageTextureUnit_writeOnly(1);
			
		program.setInt3("volume_dst_size", size);

		for (Property const& p : properties)
			p.updateUniform(program);

		ivec3 nbrGroups = (size + ivec3(7, 7, 7)) / ivec3(8, 8, 8);
		gl::dispatch(program, nbrGroups.x, nbrGroups.y, nbrGroups.z);

		auto newVolume = std::make_shared<Volume>(*src.get());
		newVolume->setTexture(std::move(texture));
		return newVolume;
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