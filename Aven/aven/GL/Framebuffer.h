#pragma once

#include <assert.h>
#include <3rdParty/GL/glad.h>
#include <aven/GL/Texture.h>


namespace aven {
namespace gl {


	class Framebuffer_rgba8u{
	public:
		Framebuffer_rgba8u(ivec2 size);
		Framebuffer_rgba8u(Framebuffer_rgba8u&& other) noexcept;
		Framebuffer_rgba8u& operator=(Framebuffer_rgba8u&& other) noexcept;
		~Framebuffer_rgba8u();

		void resize(ivec2 size);

		ivec2					getSize()			const;
		int						getWidth()			const;
		int						getHeight()			const;
		Texture2D_rgba8u const&	getTexture()		const;
		GLuint					getTextureName()	const;

		friend void bind(Framebuffer_rgba8u const& framebuffer);

	private:
		GLuint framebufferName;
		Texture2D_rgba8u texture;
	};




	class Framebuffer_rgba32f {
	public:
		Framebuffer_rgba32f(ivec2 size);
		Framebuffer_rgba32f(Framebuffer_rgba32f&& other) noexcept;
		Framebuffer_rgba32f& operator=(Framebuffer_rgba32f&& other) noexcept;
		~Framebuffer_rgba32f();
		void resize(ivec2 size);

		ivec2					getSize()			const;
		int						getWidth()			const;
		int						getHeight()			const;
		Texture2D_rgba32f const& getTexture()		const;
		GLuint					getTextureName()	const;

		friend void bind(Framebuffer_rgba32f const& framebuffer);


	private:
		GLuint framebufferName;
		Texture2D_rgba32f texture;
	};

}	// namespace gl
}	// namespace aven