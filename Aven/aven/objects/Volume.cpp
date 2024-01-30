
#include <aven/objects/BrickPool.h>
#include <aven/objects/Volume.h>
#include <aven/util/geo3d.h>
#include <aven/volumeOperations/VolumeOps.h>
#include <vector>



namespace aven {

	Volume::Volume(	ivec3 size, 
					vec3 pos,
					clamped<float, 0.001f, 100000.0f>	sigma_t,
					clamped<float, 0.001f, 100000.0f>	density,
					clamped<float, 0.25f, 5.0f>			stepSize,
					bool isRenderingBoundingBox,
					bool renderingMode_Hybrid, 
					std::shared_ptr<VolumeData> volumeData):
		sigma_t(sigma_t),
		density(density),
		stepSize(stepSize),
		pos(pos),
		isRendering_BondingBox(isRenderingBoundingBox),
		renderingMode_Hybrid(renderingMode_Hybrid)
	{
		if (volumeData == nullptr)
			data = std::make_shared<VolumeData>(size);
		else
			data = volumeData;
	}


	void Volume::serialize(std::ofstream& out, Volume const& volume) {
		auto size = volume.getSize();
		out.write((char *) &size, sizeof(ivec3));
		out.write((char *) &volume.pos, sizeof(vec3));
		out.write((char *) &volume.stepSize.getValue(), sizeof(float));
		out.write((char *) &volume.sigma_t.getValue(), sizeof(float));
		out.write((char *) &volume.density.getValue(), sizeof(float)); 
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



	Volume Volume::deserialize(std::ifstream& in) {
		ivec3 size;
		in.read((char*)&size, sizeof(ivec3));

		vec3 pos;
		in.read((char*)&pos, sizeof(vec3));
	
		float stepSize;
		in.read((char*)&stepSize, sizeof(float));

		float sigma_t;	
		in.read((char*)&sigma_t, sizeof(float));
		
		float density;
		in.read((char*)&density, sizeof(float));
	
		bool isRendering_BoundingBox = in.get();
		bool isRenderingMode_Hybrid = in.get();

		std::vector<uint32_t> data_volumeBricks;

		ivec3 sizeInBricks = (size + brickPool::BRICK_LENGTH-1) / brickPool::BRICK_LENGTH;
		auto nbrBricks = sizeInBricks.x * sizeInBricks.y * sizeInBricks.z;
		data_volumeBricks.reserve(nbrBricks);
		for (int i = 0; i < nbrBricks; i++) {
			uint32_t brick;
			in.read((char*)&brick, sizeof(uint32_t));
			data_volumeBricks.push_back(brick);
		}
		
		unsigned int brickCount;
		in.read((char*)&brickCount, sizeof(brickCount));
	
		brickPool::AllocatedBricks allocatedBricks = brickPool::alloc_cpu_bulk(brickCount);	
		for (int i = 0; i < nbrBricks; i++) {
			if (data_volumeBricks[i] == brickPool::BRICK_NULL) 
				continue;
					
			std::array<uint32_t, brickPool::BRICK_SIZE_IN_4BYTES> data;
			in.read((char*)&data[0], sizeof(data));
			uint32_t brickIndex = allocatedBricks.pop();
			brickPool::setBrickData(brickIndex, data);
			data_volumeBricks[i] = brickIndex;	//carefull, I am changing what I am iterating over
		}

		assert(allocatedBricks.size() == 0);

		VolumeData volumeData(size);
		volumeData.getSSBO().setData(&data_volumeBricks[0], nbrBricks * 4);
		auto volume = Volume(size, pos, sigma_t, density, stepSize, isRendering_BoundingBox, isRenderingMode_Hybrid, std::make_shared<VolumeData>(std::move(volumeData))); 
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
		return { pos-halfSize, {pos+halfSize}};
	}


	std::optional<ivec3> Volume::intersect(Ray const& ray) const {
		auto intersection = aven::intersect(ray, getBoundingBox());
		if (!intersection)
			return {};

		vec3 intersection_pos = ray.pos + ray.dir * intersection.value().x - this->pos;
		intersection_pos = intersection_pos + vec3(this->getSize()) / 2;
		Ray rayInternal = Ray(intersection_pos, ray.dir);
		return volumeOps::raycast(this->getVolumeData(), rayInternal);
	}

}