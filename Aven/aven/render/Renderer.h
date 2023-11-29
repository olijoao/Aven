#pragma once

#include <aven/GL/Framebuffer.h>
#include <aven/GL/ShaderLoader.h>
#include <aven/GL/VAO.h>
#include <aven/render/ViewPortCamera.h>
namespace aven {
	class Renderer {
	public:
		Renderer(float cameraDistance);

		void render();
		void resetIterations();

		void resize(int width, int height);

		gl::Texture2D_rgba8u const& getTexture() const;
		ViewPortCamera& getCamera();

		gl::Program const& getRenderProgram() const;

	private:
		ViewPortCamera camera;

		gl::Framebuffer_rgba32f	framebuffer_renderer;
		gl::Framebuffer_rgba8u	framebuffer_result;

		gl::Program				program_render;
		gl::Program				program_tonemap;
		gl::Vao					vao;

		unsigned int renderIteration = 0;

		void updateUniforms();
	};
}