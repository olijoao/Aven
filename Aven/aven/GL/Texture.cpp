
#include <aven/GL/Texture.h>
#include <stdexcept>

namespace aven {
namespace gl {


	// --------- Texture_Base

	Texture_Base::Texture_Base(GLint internalformat)
	:internalFormat(internalformat)
	{
		// returns n previously unused texture names
		glGenTextures(1, &textureName);
		assert(!glGetError());
	}


	Texture_Base::~Texture_Base() {
		glDeleteTextures(1, &textureName);	//silently ignores 0's
	}


	Texture_Base::Texture_Base(Texture_Base&& other) noexcept{
		textureName = other.textureName;
		other.textureName = 0;

		internalFormat = other.internalFormat;
		other.internalFormat = 0;
	}

	Texture_Base& Texture_Base::operator=(Texture_Base&& other) noexcept {
		std::swap(textureName,		other.textureName);
		std::swap(internalFormat,	other.internalFormat);
		return *this;
	}

	GLuint Texture_Base::getTextureName()	const {
		return textureName; 
	}


	void Texture_Base::setBorderColor(vec4 color) {
		glTextureParameterfv(textureName, GL_TEXTURE_BORDER_COLOR, &color[0]);
		assert(!glGetError());
	}
	void Texture_Base::setMinFilter(Tex_Min_Filter filter) {
		glTextureParameteri(textureName, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(filter));
		assert(!glGetError());
	}

	void Texture_Base::setMagFilter(Tex_Mag_Filter filter) {
		glTextureParameteri(textureName, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(filter));
		assert(!glGetError());
	}

	void Texture_Base::bindToTextureUnit(unsigned int unit) const {
		glBindTextureUnit(unit, getTextureName());
		assert(!glGetError());
	}

	void Texture_Base::bindToImageTextureUnit(unsigned int imageBindingPoint) {
		glBindImageTexture(imageBindingPoint, getTextureName(), 0, GL_FALSE, 0, GL_READ_WRITE, internalFormat);
		assert(!glGetError());
	}

	void Texture_Base::bindToImageTextureUnit_readOnly(unsigned int imageBindingPoint) const {
		glBindImageTexture(imageBindingPoint, getTextureName(), 0, GL_FALSE, 0, GL_READ_ONLY, internalFormat);
		assert(!glGetError());
	}

	void Texture_Base::bindToImageTextureUnit_writeOnly(unsigned int imageBindingPoint) {
		glBindImageTexture(imageBindingPoint, getTextureName(), 0, GL_FALSE, 0, GL_WRITE_ONLY, internalFormat);
		assert(!glGetError());
	}



	// --------- Texture_Base_2D

	Texture_Base_2D::Texture_Base_2D(ivec2 size, GLint internalFormat)
		:Texture_Base(internalFormat), size(size)
	{
		if(size.x <= 0 || size.y <= 0)
			throw std::invalid_argument("Texture_Base_2D size components has to be greater than 0");
		
		//A new texture object is created by binding an unused name to one of these	texture targets
		glBindTexture(GL_TEXTURE_2D, textureName);
		assert(!glGetError());
	}

	Texture_Base_2D& Texture_Base_2D::operator=(Texture_Base_2D&& other) noexcept {
		Texture_Base::operator=(std::move(other));
		std::swap(other.size, size);
		return *this;
	}
	
	ivec2 Texture_Base_2D::getSize()	const	{ return size;}
	int Texture_Base_2D::getWidth()		const	{ return size.x; }
	int Texture_Base_2D::getHeight()	const	{ return size.y; }

	void Texture_Base_2D::setWrap_X(Tex_Wrap wrap) {
		glTextureParameteri(textureName, GL_TEXTURE_WRAP_S, static_cast<GLenum>(wrap));
		assert(!glGetError());
	}

	void Texture_Base_2D::setWrap_Y(Tex_Wrap wrap) {
		glTextureParameteri(textureName, GL_TEXTURE_WRAP_T, static_cast<GLenum>(wrap));
		assert(!glGetError());
	}


	// --------- Texture_Base_3D

	Texture_Base_3D::Texture_Base_3D(ivec3 size, GLint internalFormat)
		:Texture_Base(internalFormat), size(size)
	{
		if (size.x <= 0 || size.y <= 0 || size.z <= 0)
			throw std::invalid_argument("Texture_Base_3D size components has to be greater than 0");

		//A new texture object is created by binding an unused name to one of these	texture targets
		glBindTexture(GL_TEXTURE_3D, textureName);
		assert(!glGetError());
	}

	Texture_Base_3D& Texture_Base_3D::operator=(Texture_Base_3D&& other) noexcept {
		Texture_Base::operator=(std::move(other));
		std::swap(other.size, size);
		return *this;
	}

	ivec3 Texture_Base_3D::getSize()	const { return size; }
	int Texture_Base_3D::getWidth()		const { return size.x; }
	int Texture_Base_3D::getHeight()	const { return size.y; }
	int Texture_Base_3D::getDepth()		const { return size.z; }

	void Texture_Base_3D::setWrap_X(Tex_Wrap wrap) {
		glTextureParameteri(textureName, GL_TEXTURE_WRAP_S, static_cast<GLenum>(wrap));
		assert(!glGetError());
	}

	void Texture_Base_3D::setWrap_Y(Tex_Wrap wrap) {
		glTextureParameteri(textureName, GL_TEXTURE_WRAP_T, static_cast<GLenum>(wrap));
		assert(!glGetError());
	}

	void Texture_Base_3D::setWrap_Z(Tex_Wrap wrap) {
		glTextureParameteri(textureName, GL_TEXTURE_WRAP_R, static_cast<GLenum>(wrap));
		assert(!glGetError());
	}



	// --------- Texture2D_r8u
	Texture2D_r8u::Texture2D_r8u(
		ivec2 size,
		uint8_t const* data,
		Tex_Mag_Filter	magFilter,
		Tex_Min_Filter	minFilter,
		Tex_Wrap		wrapX,
		Tex_Wrap		wrapY,
		vec4			borderColor)
		:Texture_Base_2D(size, GL_R8)
	{
		//A new texture object is created by binding an unused name to one of these	texture targets
		glBindTexture(GL_TEXTURE_2D, textureName);
		assert(!glGetError());

		setBorderColor(borderColor);
		setMagFilter(magFilter);
		setMinFilter(minFilter);
		setWrap_X(wrapX);
		setWrap_Y(wrapY);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		assert(!glGetError());

		//glClearTexImage causes erro if any size component equal to 0
		if (data == nullptr && size.x > 0 && size.y > 0) {
			uint8_t color[] = { 0,0,0,0 };
			glClearTexImage(textureName, 0, GL_RED, GL_UNSIGNED_BYTE, &color[0]);
		}

		assert(!glGetError());
	}


	Texture2D_r8u& Texture2D_r8u::operator=(Texture2D_r8u&& other) noexcept {
		Texture_Base_2D::operator=(std::move(other));
		return *this;
	}



	//loses all previous pixel:data: unspecified state?
	void Texture2D_r8u::resize(ivec2 size) {
		if (size.x <= 0 || size.y <= 0)
			throw std::invalid_argument("Texture2D_r8u::resize() size components has to be greater than 0");

		this->size = size;

		glBindTexture(GL_TEXTURE_2D, textureName);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		assert(!glGetError());	
	}



	// --------- Texture2D_rgba8u

	Texture2D_rgba8u::Texture2D_rgba8u(
		ivec2 size,
		uint8_t const*	data,
		Tex_Mag_Filter	magFilter,
		Tex_Min_Filter	minFilter,
		Tex_Wrap		wrapX,
		Tex_Wrap		wrapY,
		vec4			borderColor) 
		:Texture_Base_2D(size, GL_RGBA8)
	{
		//A new texture object is created by binding an unused name to one of these	texture targets
		glBindTexture(GL_TEXTURE_2D, textureName);

		assert(!glGetError());

		setBorderColor(borderColor);
		setMagFilter(magFilter);
		setMinFilter(minFilter);
		setWrap_X(wrapX);
		setWrap_Y(wrapY);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		assert(!glGetError()); 

		//glClearTexImage causes erro if any size component equal to 0
		if (data == nullptr && size.x > 0 && size.y > 0) {
			uint8_t color[] = { 0,0,0,0 };
			glClearTexImage(textureName, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color[0]);
		}

		assert(!glGetError());
	}


	Texture2D_rgba8u& Texture2D_rgba8u::operator=(Texture2D_rgba8u&& other) noexcept {
		Texture_Base_2D::operator=(std::move(other));
		return *this;
	}



	//loses all previous pixel:data: unspecified state?
	void Texture2D_rgba8u::resize(ivec2 size) {
		if (size.x <= 0 || size.y <= 0)
			throw std::invalid_argument("Texture2D_rgba8u::resize() size components has to be greater than 0");

		this->size = size;

		glBindTexture(GL_TEXTURE_2D, textureName);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		assert(!glGetError());
	}




	Texture2D_rgba32f::Texture2D_rgba32f(
		ivec2 size,
		float const*		data,
		Tex_Mag_Filter		magFilter,
		Tex_Min_Filter		minFilter,
		Tex_Wrap			wrapX,
		Tex_Wrap			wrapY,
		vec4				borderColor)
		:Texture_Base_2D(size, GL_RGBA32F)
	{
		setBorderColor(borderColor);
		setMagFilter(magFilter);
		setMinFilter(minFilter);
		setWrap_X(wrapX);
		setWrap_Y(wrapY);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, GL_RGBA, GL_FLOAT, data);

		//glClearTexImage causes erro if any size component equal to 0
		if (data == nullptr && size.x > 0 && size.y > 0) {
			float color[] = { 0,0,0,0 };
			glClearTexImage(textureName, 0, GL_RGBA, GL_FLOAT, &color[0]);
		}

		assert(!glGetError());
	}


	Texture2D_rgba32f& Texture2D_rgba32f::operator=(Texture2D_rgba32f&& other) noexcept {
		Texture_Base_2D::operator=(std::move(other));
		return *this;
	}



	//loses all previous pixel:data: unspecified state?
	void Texture2D_rgba32f::resize(ivec2 size) {
		if (size.x <= 0 || size.y <= 0)
			throw std::invalid_argument("Texture2D_rgba32f::resize() size components has to be greater than 0");

		this->size = size;

		glBindTexture(GL_TEXTURE_2D, textureName);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, GL_RGBA, GL_FLOAT, nullptr);
		assert(!glGetError());
	}



	Texture3D_rgba8u::Texture3D_rgba8u(
		ivec3 size,
		uint8_t const*	data,
		Tex_Mag_Filter	magFilter,
		Tex_Min_Filter	minFilter,
		Tex_Wrap		wrapX,
		Tex_Wrap		wrapY,
		Tex_Wrap		wrapZ,
		vec4			borderColor)
		:Texture_Base_3D(size, GL_RGBA8)
	{

		// returns n previously unused texture names
		glGenTextures(1, &textureName);
		//A new texture object is created by binding an unused name to one of these	texture targets
		glBindTexture(GL_TEXTURE_3D, textureName);

		assert(!glGetError());

		setBorderColor(borderColor);
		setMagFilter(magFilter);
		setMinFilter(minFilter);
		setWrap_X(wrapX);
		setWrap_Y(wrapY);
		setWrap_Z(wrapZ);

		glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, size.x, size.y, size.z, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		//glClearTexImage causes erro if any size component equal to 0
		if (data == nullptr && size.x > 0 && size.y > 0 && size.z > 0) {
			uint8_t color[] = { 0,0,0,0 };
			glClearTexImage(textureName, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color[0]);
		}

		assert(!glGetError());
	}


	Texture3D_rgba8u& Texture3D_rgba8u::operator=(Texture3D_rgba8u&& other) noexcept {
		Texture_Base_3D::operator=(std::move(other));
		return *this;
	}


	//loses all previous pixel:data: unspecified state?
	void Texture3D_rgba8u::resize(ivec3 size) {
		assert(size.x >= 0 && size.y >= 0 && size.z >=0);

		this->size = size;

		glBindTexture(GL_TEXTURE_3D, textureName);
		glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, size.x, size.y, size.z, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		assert(!glGetError());
	}



	Texture3D_r8u::Texture3D_r8u(
		ivec3 size,
		uint8_t const* data,
		Tex_Mag_Filter	magFilter,
		Tex_Min_Filter	minFilter,
		Tex_Wrap		wrapX,
		Tex_Wrap		wrapY,
		Tex_Wrap		wrapZ,
		vec4			borderColor)
		:Texture_Base_3D(size, GL_R8)
	{
		// returns n previously unused texture names
		glGenTextures(1, &textureName);
		//A new texture object is created by binding an unused name to one of these	texture targets
		glBindTexture(GL_TEXTURE_3D, textureName);

		assert(!glGetError());

		setBorderColor(borderColor);
		setMagFilter(magFilter);
		setMinFilter(minFilter);
		setWrap_X(wrapX);
		setWrap_Y(wrapY);
		setWrap_Z(wrapZ);

		glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, size.x, size.y, size.z, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		//glClearTexImage causes erro if any size component equal to 0
		if (data == nullptr && size.x > 0 && size.y > 0) {
			uint8_t color[] = { 0,0,0,0 };
			glClearTexImage(textureName, 0, GL_RED, GL_UNSIGNED_BYTE, &color[0]);
		}

		assert(!glGetError());
	}


	Texture3D_r8u& Texture3D_r8u::operator=(Texture3D_r8u&& other) noexcept {
		Texture_Base_3D::operator=(std::move(other));
		return *this;
	}


	//loses all previous pixel:data: unspecified state?
	void Texture3D_r8u::resize(ivec3 size) {
		assert(size.x >= 0 && size.y >= 0 && size.z >= 0);

		this->size = size;

		glBindTexture(GL_TEXTURE_3D, textureName);
		glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, size.x, size.y, size.z, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
		assert(!glGetError());
	}


}	// namespace gl
}	// namespace aven