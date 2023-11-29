#pragma once

#include <aven/GL/Texture.h>
#include <aven/util/clamped.h>
#include <aven/util/geo3d.h>
#include <aven/util/math.h>
#include <memory> 

namespace aven {
	class Volume{
	public:
		Volume(	gl::Texture3D_rgba8u texture, 
				vec3 pos,
				clamped<float, 0.001f, 100000.0f>	sigma_t, 
				clamped<float, 0.001f, 100000.0f>	density,
				clamped<float, 0.25f,	5.0f>		stepSize);

		Volume(	ivec3 size, 
				vec3 pos,
				clamped<float, 0.001f, 100000.0f>	sigma_t,
				clamped<float, 0.001f, 100000.0f>	density,
				clamped<float, 0.25f,	5.0f>		stepSize);


		~Volume()								= default;
		Volume(Volume const&)					= default;
		Volume(Volume &&)						= default;
		Volume& operator=(Volume const& other)	= default;
		Volume& operator=(Volume && other)		= default;

		void setTexture(gl::Texture3D_rgba8u&&);
		
		ivec3 getSize() const;
	
		gl::Texture3D_rgba8u const& getTexture() const;

		AABB<float> getBoundingBox() const;


	public:
		vec3 pos;
		clamped<float, 0.25f,	5.0f>		stepSize;
		clamped<float, 0.001f,	100000.0f>	sigma_t;	
		clamped<float, 0.001f,	100000.0f>	density;	
		bool isRendering_BondingBox = true;
		bool renderingMode_Hybrid	= false;

	private:
		ivec3 size;
		std::shared_ptr<gl::Texture3D_rgba8u const> texture;
	};




}