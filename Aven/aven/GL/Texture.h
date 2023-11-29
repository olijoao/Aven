#pragma once

#include <3rdParty/GL/glad.h>
#include <aven/util/math.h>
#include <vector>

namespace aven {
namespace gl {

	enum class Tex_Wrap : GLenum {
		CLAMP_TO_EDGE			= GL_CLAMP_TO_EDGE,
		REPEAT					= GL_REPEAT,
		CLAMP_TO_BORDER			= GL_CLAMP_TO_BORDER,
		MIRRORED_REPEAT			= GL_MIRRORED_REPEAT,
		MIRROR_CLAMP_TO_EDGE	= GL_MIRROR_CLAMP_TO_EDGE
	};


	enum class Tex_Mag_Filter : GLenum {
		NEAREST = GL_NEAREST,
		LINEAR	= GL_LINEAR
	};


	enum class Tex_Min_Filter : GLenum {
		NEAREST					= GL_NEAREST,
		LINEAR					= GL_LINEAR,
		NEAREST_MIPMAP_NEAREST	= GL_NEAREST_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR	= GL_NEAREST_MIPMAP_LINEAR,	
		LINEAR_MIPMAP_NEAREST	= GL_LINEAR_MIPMAP_NEAREST,	
		LINEAR_MIPMAP_LINEAR	= GL_LINEAR_MIPMAP_LINEAR	
	};



	class Texture_Base {
	public:
		GLuint getTextureName()	const;

		void setBorderColor(vec4 color);
		void setMinFilter(Tex_Min_Filter filter);
		void setMagFilter(Tex_Mag_Filter filter);

		void bindToTextureUnit(unsigned int unit) const;
		
		void bindToImageTextureUnit(unsigned int imageBindingPoint);
		void bindToImageTextureUnit_readOnly(unsigned int imageBindingPoint) const;
		void bindToImageTextureUnit_writeOnly(unsigned int imageBindingPoint);

	protected:
		Texture_Base(GLint internalFormat);
		~Texture_Base();
		Texture_Base(Texture_Base&)				= delete;
		Texture_Base& operator=(Texture_Base&)	= delete;
		Texture_Base(Texture_Base&& other) noexcept;
		Texture_Base& operator=(Texture_Base&& other) noexcept;

		GLuint textureName{ 0 };
		GLint internalFormat;
	};



	class Texture_Base_2D : public Texture_Base{
	public:
		ivec2	getSize()	const;
		int		getWidth()	const;
		int		getHeight()	const;

		void setWrap_X(Tex_Wrap wrap);
		void setWrap_Y(Tex_Wrap wrap);

	protected:
		Texture_Base_2D(ivec2 size, GLint internalFormat);
		Texture_Base_2D(Texture_Base_2D&)				= delete;
		Texture_Base_2D& operator=(Texture_Base_2D&)	= delete;
		Texture_Base_2D(Texture_Base_2D&& other)		= default;
		Texture_Base_2D& operator=(Texture_Base_2D&& other) noexcept;
		ivec2 size;
	};


	class Texture_Base_3D : public Texture_Base {
	public:
		ivec3	getSize()	const;
		int		getWidth()	const;
		int		getHeight()	const;
		int		getDepth()	const;

		void setWrap_X(Tex_Wrap wrap);
		void setWrap_Y(Tex_Wrap wrap);
		void setWrap_Z(Tex_Wrap wrap);

	protected:
		Texture_Base_3D(ivec3 size, GLint internalFormat);
		Texture_Base_3D(Texture_Base_3D&)				= delete;
		Texture_Base_3D& operator=(Texture_Base_3D&)	= delete;
		Texture_Base_3D(Texture_Base_3D&& other)		= default;
		Texture_Base_3D& operator=(Texture_Base_3D&& other) noexcept;

		ivec3 size;
	};




	class Texture2D_r8u : public Texture_Base_2D {
	public:
		Texture2D_r8u(	ivec2 size,
						uint8_t const* data			= nullptr,
						Tex_Mag_Filter	magFilter	= Tex_Mag_Filter::LINEAR,
						Tex_Min_Filter	minFilter	= Tex_Min_Filter::LINEAR,
						Tex_Wrap		wrapX		= Tex_Wrap::REPEAT,
						Tex_Wrap		wrapY		= Tex_Wrap::REPEAT,
						vec4			borderColor = vec4(0, 0, 0, 0));


		Texture2D_r8u(Texture2D_r8u&& other) = default;
		Texture2D_r8u& operator=(Texture2D_r8u&& other) noexcept;
		Texture2D_r8u(Texture2D_r8u&)				= delete;
		Texture2D_r8u& operator=(Texture2D_r8u&)	= delete;
		// loses all previous pixel(data in unspecified state)
		void	resize(ivec2 size);
	};


	class Texture2D_rgba8u : public Texture_Base_2D {
	public:
		Texture2D_rgba8u(	ivec2 size,
							uint8_t const*	data		= nullptr,
							Tex_Mag_Filter	magFilter	= Tex_Mag_Filter::LINEAR,
							Tex_Min_Filter	minFilter	= Tex_Min_Filter::LINEAR,
							Tex_Wrap		wrapX		= Tex_Wrap::REPEAT,
							Tex_Wrap		wrapY		= Tex_Wrap::REPEAT,
							vec4			borderColor = vec4(0, 0, 0, 0))	;


		Texture2D_rgba8u(Texture2D_rgba8u&& other)		= default;
		Texture2D_rgba8u& operator=(Texture2D_rgba8u&& other) noexcept;
		Texture2D_rgba8u(Texture2D_rgba8u&)				= delete;
		Texture2D_rgba8u& operator=(Texture2D_rgba8u&)	= delete;
		// loses all previous pixel(data in unspecified state)
		void	resize(ivec2 size);	
	};


	class Texture2D_rgba32f : public Texture_Base_2D{
	public:
		Texture2D_rgba32f(ivec2 size,
			float const*	data		= nullptr,
			Tex_Mag_Filter	magFilter	= Tex_Mag_Filter::LINEAR,
			Tex_Min_Filter	minFilter	= Tex_Min_Filter::LINEAR,
			Tex_Wrap		wrapX		= Tex_Wrap::REPEAT,
			Tex_Wrap		wrapY		= Tex_Wrap::REPEAT,
			vec4			borderColor = vec4(0, 0, 0, 0));


		Texture2D_rgba32f(Texture2D_rgba32f&& other) = default;
		Texture2D_rgba32f& operator=(Texture2D_rgba32f&& other) noexcept;
		Texture2D_rgba32f(Texture2D_rgba32f&)				= delete;
		Texture2D_rgba32f& operator=(Texture2D_rgba32f&)	= delete;

		// loses all previous pixel(data in unspecified state)
		void	resize(ivec2 size);
	};




	class Texture3D_rgba8u : public Texture_Base_3D{
	public:
		Texture3D_rgba8u(	ivec3 size,
							uint8_t const*	data		= nullptr,
							Tex_Mag_Filter	magFilter	= Tex_Mag_Filter::NEAREST,
							Tex_Min_Filter	minFilter	= Tex_Min_Filter::NEAREST,
							Tex_Wrap		wrapX		= Tex_Wrap::CLAMP_TO_BORDER,
							Tex_Wrap		wrapY		= Tex_Wrap::CLAMP_TO_BORDER,
							Tex_Wrap		wrapZ		= Tex_Wrap::CLAMP_TO_BORDER,
							vec4			borderColor = vec4(0, 0, 0, 0))	;

		Texture3D_rgba8u(Texture3D_rgba8u&& other) = default;
		Texture3D_rgba8u& operator=(Texture3D_rgba8u&& other) noexcept;
		Texture3D_rgba8u(Texture3D_rgba8u&)				= delete;
		Texture3D_rgba8u& operator=(Texture3D_rgba8u&)	= delete;

		// loses all previous pixel(data in unspecified state)
		void resize(ivec3 size);
	};


	class Texture3D_r8u : public Texture_Base_3D {
	public:
		Texture3D_r8u(	ivec3 size,
						uint8_t const* data = nullptr,
						Tex_Mag_Filter	magFilter = Tex_Mag_Filter::NEAREST,
						Tex_Min_Filter	minFilter = Tex_Min_Filter::NEAREST,
						Tex_Wrap		wrapX = Tex_Wrap::CLAMP_TO_BORDER,
						Tex_Wrap		wrapY = Tex_Wrap::CLAMP_TO_BORDER,
						Tex_Wrap		wrapZ = Tex_Wrap::CLAMP_TO_BORDER,
						vec4			borderColor = vec4(0, 0, 0, 0));

		Texture3D_r8u(Texture3D_r8u&& other) = default;
		Texture3D_r8u& operator=(Texture3D_r8u&& other) noexcept;
		Texture3D_r8u(Texture3D_r8u&) = delete;
		Texture3D_r8u& operator=(Texture3D_r8u&) = delete;

		// loses all previous pixel(data in unspecified state)
		void resize(ivec3 size);
	};


}	// namespace gl
}	// namespace aven