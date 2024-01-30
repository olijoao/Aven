#pragma once

#include <aven/util/clamped.h>
#include <aven/util/geo3d.h>
#include <aven/util/math.h>
#include <aven/objects/VolumeData.h>
#include <fstream>

namespace aven {

	class Volume{
	public:
		static const int MAX_VOLUME_LENGTH = 512;

	public:
		Volume(	ivec3 size, 
				vec3 pos,
				clamped<float, 0.001f, 100000.0f>	sigma_t,
				clamped<float, 0.001f, 100000.0f>	density,
				clamped<float, 0.25f,	5.0f>		stepSize,
				bool isRenderingBoundingBox,
				bool renderingMode_Hybrid, 
				std::shared_ptr<VolumeData>			data = nullptr);


		~Volume()								= default;
		Volume(Volume const&)					= default;
		Volume(Volume &&)						= default;
		Volume& operator=(Volume const& other)	= default;
		Volume& operator=(Volume && other)		= default;

		static void serialize(std::ofstream&, Volume const&);
		static Volume deserialize(std::ifstream&);

		ivec3 getSize() const;
		ivec3 getSize_inBricks() const;

		gl::SSBO const& getSSBO() const;
		VolumeData const& getVolumeData() const;
		void setVolumeData(VolumeData&& data);

		unsigned int getNumberOfBricks()const;
		AABB<float> getBoundingBox() const;

		std::optional<ivec3> intersect(Ray const& ray) const;

	public:
		vec3 pos;
		clamped<float, 0.25f,	5.0f>		stepSize;
		clamped<float, 0.001f,	100000.0f>	sigma_t;	
		clamped<float, 0.001f,	100000.0f>	density;	
		bool isRendering_BondingBox = true;
		bool renderingMode_Hybrid	= false;

	private:
		std::shared_ptr<VolumeData const> data;
	};


}