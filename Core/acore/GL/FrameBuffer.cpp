#include <acore/GL/Framebuffer.h>
#include <stdexcept>

namespace aven {
namespace gl {

	Framebuffer_rgba8u::Framebuffer_rgba8u(ivec2 size)
		: texture(size)
	{
		if (size.x <= 0 || size.y <= 0)
			throw std::invalid_argument("Framebuffer_rgba8u size components has to be greater than 0");

		//from opengl specs:
		// A framebuffer object is created by binding a name returned by GenFramebuffers to DRAW_FRAMEBUFFER or READ_FRAMEBUFFER*/
		// Calling BindFramebuffer with target set to FRAMEBUFFER binds	framebuffer to both the drawand read targets
		glGenFramebuffers(1, &framebufferName);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferName);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 0, GL_TEXTURE_2D, texture.getTextureName(), 0);
		assert(!glGetError());	
	}


	Framebuffer_rgba8u::Framebuffer_rgba8u(Framebuffer_rgba8u&& other) noexcept
		:texture(std::move(other.texture))
	{
		framebufferName = other.framebufferName;
		other.framebufferName = 0;
	}


	Framebuffer_rgba8u& Framebuffer_rgba8u::operator=(Framebuffer_rgba8u&& other) noexcept {
		std::swap(framebufferName, other.framebufferName);
		std::swap(texture, other.texture);
		return *this;
	}

	Framebuffer_rgba8u::~Framebuffer_rgba8u() {
		glDeleteFramebuffers(1, &framebufferName);	//silently ignores 0's
	}


	void Framebuffer_rgba8u::resize(ivec2 size) {
		texture.resize(size);
	}

	ivec2 Framebuffer_rgba8u::getSize() const { 
		return texture.getSize(); 
	}

	int	Framebuffer_rgba8u::getWidth() const {
		return texture.getWidth(); 
	}

	int	Framebuffer_rgba8u::getHeight()	const {
		return texture.getHeight();
	}

	Texture2D_rgba8u const& Framebuffer_rgba8u::getTexture() const { 
		return texture; 
	}

	GLuint Framebuffer_rgba8u::getTextureName() const { 
		return texture.getTextureName(); 
	}

	void bind(Framebuffer_rgba8u const& framebuffer) {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.framebufferName);
		assert(!glGetError());
	}





	Framebuffer_rgba32f::Framebuffer_rgba32f(ivec2 size)
		: texture(size)
	{
		if (size.x <= 0 || size.y <= 0)
			throw std::invalid_argument("Framebuffer_rgba32f size components has to be greater than 0");
		
		//from opengl specs:
		// A framebuffer object is created by binding a name returned by GenFramebuffers to DRAW_FRAMEBUFFER or READ_FRAMEBUFFER*/
		// Calling BindFramebuffer with target set to FRAMEBUFFER binds	framebuffer to both the drawand read targets
		glGenFramebuffers(1, &framebufferName);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferName);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 0, GL_TEXTURE_2D, texture.getTextureName(), 0);
		assert(!glGetError());
	}


	Framebuffer_rgba32f::Framebuffer_rgba32f(Framebuffer_rgba32f&& other) noexcept
		:texture(std::move(other.texture))
	{
		framebufferName = other.framebufferName;
		other.framebufferName = 0;
	}


	Framebuffer_rgba32f& Framebuffer_rgba32f::operator=(Framebuffer_rgba32f&& other) noexcept {
		std::swap(framebufferName, other.framebufferName);
		std::swap(texture, other.texture);
		return *this;
	}


	Framebuffer_rgba32f::~Framebuffer_rgba32f() {
		glDeleteFramebuffers(1, &framebufferName);	//silently ignores 0's
	}


	void Framebuffer_rgba32f::resize(ivec2 size) {
		texture.resize(size);
	}

	ivec2 Framebuffer_rgba32f::getSize() const {
		return texture.getSize();
	}

	int	Framebuffer_rgba32f::getWidth() const {
		return texture.getWidth();
	}

	int	Framebuffer_rgba32f::getHeight()	const {
		return texture.getHeight();
	}

	Texture2D_rgba32f const& Framebuffer_rgba32f::getTexture() const {
		return texture;
	}

	GLuint Framebuffer_rgba32f::getTextureName() const {
		return texture.getTextureName();
	}

	void bind(Framebuffer_rgba32f const& framebuffer) {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.framebufferName);
		assert(!glGetError());
	}
}
}