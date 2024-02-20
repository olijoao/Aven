
#include <aven/objects/BrickPool.h>
#include <aven/objects/Volume.h>
#include <aven/util/geo3d.h>
#include <aven/volumeOperations/VolumeOps.h>
#include <vector>

namespace aven {

	Volume::Volume(	c_ivec3<1, MAX_VOLUME_LENGTH>size, 
					Transformation transformation,
					c_float<0.001f, 100000.0f>	density,
					c_float<0.25f, 5.0f>		stepSize,
					bool isRenderingBoundingBox,
					bool renderingMode_Hybrid, 
					std::shared_ptr<VolumeData> volumeData):
		density(density),
		stepSize(stepSize),
		transformation(transformation),
		isRendering_BondingBox(isRenderingBoundingBox),
		renderingMode_Hybrid(renderingMode_Hybrid)
	{
		if (volumeData == nullptr)
			data = std::make_shared<VolumeData>(size);
		else
			data = volumeData;
	}


	void Volume::serialize(std::ofstream& out, Volume const& volume) {
		int version = 1;
		out.write((char*) &version,	sizeof(version));
	
		Transformation::serialize(out, volume.transformation);

		auto size = volume.getSize();
		out.write((char *) &size, sizeof(size));
		out.write((char *) &volume.stepSize, sizeof(volume.stepSize));
		out.write((char *) &volume.density, sizeof(volume.density)); 
		out.put(volume.isRendering_BondingBox ? 1 : 0);
		out.put(volume.renderingMode_Hybrid ? 1 : 0);

		//voxel data 
		assert(volume.getSSBO().getSizeInBytes() == volume.getNumberOfBricks()*4);
		std::vector<uint32_t> data_volumeBricks;
		data_volumeBricks.resize(volume.getNumberOfBricks());
		volume.getSSBO().getData(&data_volumeBricks[0]);

		unsigned int brickCount = 0;
		std::vector<uint32_t> bricksToDownload;
		for (uint32_t brick : data_volumeBricks ) {
			if (brick == brickPool::BRICK_NULL) {
				out.write((char*) &brickPool::BRICK_NULL, sizeof(uint32_t));
			} else {
				out.write((char*)&brickCount, sizeof(uint32_t));
				brickCount++;
				bricksToDownload.push_back(brick);
			}
		}
		out.write((char*)&brickCount, sizeof(brickCount));

		for (uint32_t brickIndex : bricksToDownload) {
			std::array<uint32_t, brickPool::BRICK_SIZE_IN_4BYTES> data;
			brickPool::getBrickData(brickIndex, data);
			out.write((char*)&data[0], brickPool::BRICK_SIZE_IN_4BYTES*4);
		}
	}



	std::expected<Volume, std::string> Volume::deserialize(std::ifstream& in) {

		int version;
		ivec3 size;
		float stepSize, density;
		bool isRendering_BoundingBox, isRenderingMode_Hybrid;  

		if ( !in.read((char*)&version, sizeof(version)))
			return std::unexpected("Error occured while deserializing Volume.");

		auto transformation = Transformation::deserialize(in);
		if(!transformation.has_value())
			return std::unexpected("Error occured while deserializing Volume." + transformation.error());

		if(    !in.read((char*)&size, sizeof(size))
			|| !in.read((char*)&stepSize, sizeof(stepSize))
			|| !in.read((char*)&density, sizeof(density)) 
			|| !in.read((char*)&isRendering_BoundingBox, sizeof(isRendering_BoundingBox))
			|| !in.read((char*)&isRenderingMode_Hybrid, sizeof(isRenderingMode_Hybrid)))
		{
			return std::unexpected("Error occured while deserializing Volume.");
		}

		if (any(greaterThan(size, ivec3(Volume::MAX_VOLUME_LENGTH)))) 
			return std::unexpected("error occured while deserializing Volume. Volume size too big.");

		if(version != 1)
			return std::unexpected("error occured while deserializing Volume. Version must be 1.");
		
		// load in brick pointers 
		std::vector<uint32_t> data_volumeBricks;
		ivec3 sizeInBricks = (size + brickPool::BRICK_LENGTH-1) / brickPool::BRICK_LENGTH;
		auto nbrBricks = sizeInBricks.x * sizeInBricks.y * sizeInBricks.z;
		data_volumeBricks.resize(nbrBricks);
		if(!in.read((char*)&data_volumeBricks[0], nbrBricks * sizeof(uint32_t))) {
			return std::unexpected("error occured while deserializing Volume. Failure to read bricks pointers.");
		}
		
		unsigned int brickCount;
		if(!in.read((char*)&brickCount, sizeof(brickCount)))
			return std::unexpected("error occured while deserializing Volume.");
		if(brickCount > nbrBricks)
			return std::unexpected("error occured while deserializing Volume. Given brickCount higher than volume size.");

		if (brickCount > 0) {
			brickPool::AllocatedBricks allocatedBricks = brickPool::alloc_cpu_bulk(brickCount);	
			for (int i = 0; i < nbrBricks; i++) {
				if (data_volumeBricks[i] == brickPool::BRICK_NULL) 
					continue;
						
				std::array<uint32_t, brickPool::BRICK_SIZE_IN_4BYTES> data;
				if(!in.read((char*)&data[0], sizeof(data)))
					return std::unexpected("error occured while deserializing Volume.");

				uint32_t brickIndex = allocatedBricks.pop();
				brickPool::setBrickData(brickIndex, data);
				data_volumeBricks[i] = brickIndex;	//carefull, I am changing what I am iterating over
			}
			if(allocatedBricks.size() > 0)
				return std::unexpected("error occured while deserializing Volume.");
		}

		VolumeData volumeData(size);
		volumeData.getSSBO().setData(&data_volumeBricks[0], nbrBricks * 4);
		auto volume = Volume(size, transformation.value(), density, stepSize, isRendering_BoundingBox, isRenderingMode_Hybrid, std::make_shared<VolumeData>(std::move(volumeData)));
		return volume;
	}



	ivec3 Volume::getSize() const {
		return data->getSize();
	}

	ivec3 Volume::getSize_inBricks() const {
		return (getSize() + brickPool::BRICK_LENGTH-1) / brickPool::BRICK_LENGTH;
	}


	gl::SSBO const& Volume::getSSBO() const {
		return data->getSSBO();
	}


	VolumeData const& Volume::getVolumeData() const {
		return *data;
	}

	void Volume::setVolumeData(VolumeData&& data) {
		this->data = std::make_shared<VolumeData>(std::move(data));
	}

	unsigned int Volume::getNumberOfBricks() const {
		auto sizeInBricks = getSize_inBricks();
		return sizeInBricks.x * sizeInBricks.y * sizeInBricks.z;
	}

	AABB<float> Volume::getBoundingBox() const {
		vec3 halfSize = vec3(getSize()) / 2;
		return { transformation.getPos() - halfSize, {transformation.getPos() + halfSize}};
	}


	std::optional<ivec3> Volume::intersect(Ray const& ray) const {
		auto intersection = aven::intersect(ray, getBoundingBox());
		if (!intersection)
			return {};

		vec3 intersection_pos = ray.pos + ray.dir * intersection.value().x - this->transformation.getPos();
		intersection_pos = intersection_pos + vec3(this->getSize()) / 2;
		Ray rayInternal = Ray(intersection_pos, ray.dir);
		return volumeOps::raycast(this->getVolumeData(), rayInternal);
	}

}