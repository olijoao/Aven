

#include <aven/Aven.h>
#include <aven/objects/BrickPool.h>
#include <aven/render/Renderer.h>

namespace aven{


	Renderer::Renderer(float cameraDistance):
		camera(vec3(0, 0, cameraDistance), vec3(0, 0, 0), 90.0f, {32,32}),
		framebuffer_renderer(uvec2(32, 32)),
		framebuffer_result(uvec2(32, 32)),
		program_render(gl::loadProgram({	
									{ gl::ShaderType::Vertex,	"shader/render/renderer_vert.glsl"}, 
									{ gl::ShaderType::Fragment,	"shader/render/renderer_frag.glsl" }})),
		program_tonemap(gl::loadProgram({	
									{ gl::ShaderType::Vertex,	"shader/render/tonemap_vert.glsl"}, 
									{ gl::ShaderType::Fragment,	"shader/render/tonemap_frag.glsl" }})),

		vao(std::initializer_list<vec3>({	vec3(1.0f, 1.0f, 0.0f),
											vec3(-1.0f, -1.0f, 0.0f),
											vec3(1.0f, -1.0f, 0.0f),
											vec3(-1.0f, -1.0f, 0.0f),
											vec3(-1.0f, 1.0f, 0.0f),
											vec3(1.0f, 1.0f, 0.0f) })), 
		rng(std::random_device{}()),
		distribution(0, 1)
												
	{
		//...
	}




	void Renderer::render() {
		if (renderIteration >= camera.maxSamples.getValue())
			return;

		//change viewport
		GLint size_viewport[4];
		glGetIntegerv(GL_VIEWPORT, size_viewport);

		glViewport(0,0, framebuffer_renderer.getWidth(), framebuffer_renderer.getHeight());

		// render
		bind(framebuffer_renderer);
		use(program_render);
		updateUniforms();
		vao.renderTriangles();

		// tone map
		bind(framebuffer_result);
		use(program_tonemap);
		program_tonemap.setFloat("gamma", camera.gamma.getValue());
		vao.renderTriangles();

		//restore viewport
		glViewport(size_viewport[0], size_viewport[1], size_viewport[2], size_viewport[3]);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		renderIteration++;
	}



	void Renderer::resetIterations() {
		renderIteration = 0;
	}



	void Renderer::resize(int width, int height) {
		framebuffer_result.resize({ width, height });
		framebuffer_renderer.resize({ width, height });
		camera.setFilmSize({width, height});
		resetIterations();
	}


	gl::Texture2D_rgba8u const& Renderer::getTexture() const {
		return framebuffer_result.getTexture();
	}


	aven::ViewPortCamera& Renderer::getCamera() {
		return camera;
	}


	gl::Program const& Renderer::getRenderProgram() const {
		return program_render;
	}


	void Renderer::updateUniforms() {
		program_render.setUint("renderIteration", renderIteration);
		framebuffer_renderer.getTexture().bindToTextureUnit(0);
		program_render.setInt("textureTarget", 0);

		// rand
		if (renderIteration == 0) {
			rng.seed(0);
			distribution.reset();
		}

		program_render.setVec2("rand_seed", (distribution(rng)), 
											(distribution(rng)));

		auto coordSystem	= camera.getCoordSystemofImagePlane();
		auto filmSize		= camera.getFilmSize();
		program_render.setVec3("camera.pos",						camera.pos);
		program_render.setVec2("camera.invFilmSize",				vec2(1.0f / filmSize.x, 1.0f / filmSize.y));
		program_render.setVec3("camera.right",						std::get<0>(coordSystem));
		program_render.setVec3("camera.up",							std::get<1>(coordSystem));
		program_render.setVec3("camera.forward",					std::get<2>(coordSystem));
		program_render.setVec3("camera.color_background_sky",		camera.backgroundColor_sky);
		program_render.setVec3("camera.color_background_ground",	camera.backgroundColor_ground);
		program_render.setInt("camera.nbrSamplesPerIteration",		camera.nbrSamplesPerIteration.getValue());
		program_render.setInt("camera.nbrBounces",					camera.nbrBounces.getValue());

		auto volume = aven::getProject().getScene().volume;
		//volume.glsl
		program_render.setInt3("volume_size",					volume->getSize());

		program_render.setVec3("invVolumeSize",					vec3(1.0f) / vec3(volume->getSize()));
		program_render.setFloat("volume_stepSize",				volume->stepSize.getValue());
		program_render.setFloat("volume_sigma_t",				volume->sigma_t.getValue());
		program_render.setFloat("volume_density",				volume->density.getValue());
		program_render.setVec3("volume_pos",					volume->pos);
		program_render.setInt("volune_isDisplayingBoundingBox",	volume->isRendering_BondingBox?1:0);
		program_render.setInt("volume_renderModeHybrid",		volume->renderingMode_Hybrid?1:0);

		brickPool::bindSSBO_toBufferBase0();
		auto& op= aven::getProject().getCurrentToolOperation();
		if(!op){
			program_render.setInt("volume_type", 0);
			volume->getSSBO().bindBufferBase(1);
		}else{

			switch (op->blendMode) {
			case volumeOps::BlendMode::Normal:
				program_render.setInt("volume_type", 1);
				break;
			case volumeOps::BlendMode::Erase:
				program_render.setInt("volume_type", 2);
				break;
			}
			
			volume->getSSBO().bindBufferBase(1);
			op->volumeData.getSSBO().bindBufferBase(2);
			program_render.setFloat("volume_opacity", static_cast<float>(op->opacity.getValue()) / 255.0f);
		}
	}

}