#pragma once

#include <acore/GL/Framebuffer.h>
#include <acore/GL/ShaderLoader.h>
#include <acore/GL/VAO.h>
#include <random>

namespace aven {
	class Renderer {
	public:
		Renderer();

		void render();
		void resetIterations();
		bool hasReachedFinalIteration();

		void resize(int width, int height);

		gl::Texture2D_rgba8u const& getTexture() const;

		gl::Program const& getRenderProgram() const;

	private:
		std::mt19937 rng;
		std::uniform_real_distribution<float> distribution;

		gl::Framebuffer_rgba32f	framebuffer_renderer;
		gl::Framebuffer_rgba8u	framebuffer_result;

		gl::Program				program_render;
		gl::Program				program_tonemap;
		gl::Vao					vao;

		int renderIteration = 0;

		void updateUniforms();
	};
}