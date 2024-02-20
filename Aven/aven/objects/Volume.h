#pragma once

#include <aven/objects/VolumeData.h>
#include <aven/util/Transformation.h>
#include <aven/util/clamped.h>
#include <aven/util/geo3d.h>
#include <aven/util/math.h>
#include <expected>
#include <fstream>


namespace aven {

	class Volume{
	public:
		static const int MAX_VOLUME_LENGTH = 512;

	public:
		Volume(	c_ivec3<1, MAX_VOLUME_LENGTH> size, 
				Transformation transformation,
				c_float<0.001f, 100000.0f>	density,
				c_float<0.25f,	5.0f>		stepSize,
				bool isRenderingBoundingBox,
				bool renderingMode_Hybrid, 
				std::shared_ptr<VolumeData>			data = nullptr);


		~Volume()								= default;
		Volume(Volume const&)					= default;
		Volume(Volume &&)						= default;
		Volume& operator=(Volume const& other)	= default;
		Volume& operator=(Volume && other)		= default;

		static void serialize(std::ofstream&, Volume const&);
		static std::expected<Volume, std::string> deserialize(std::ifstream&);

		ivec3 getSize() const;
		ivec3 getSize_inBricks() const;

		gl::SSBO const& getSSBO() const;
		VolumeData const& getVolumeData() const;
		void setVolumeData(VolumeData&& data);

		unsigned int getNumberOfBricks()const;
		AABB<float> getBoundingBox() const;

		std::optional<ivec3> intersect(Ray const& ray) const;


	public:
		Transformation transformation;
		c_float<0.25f,	5.0f>		stepSize;
		c_float<0.001f,	100000.0f>	density;	
		bool isRendering_BondingBox = true;
		bool renderingMode_Hybrid	= false;

	private:
		std::shared_ptr<VolumeData const> data;
	};


}