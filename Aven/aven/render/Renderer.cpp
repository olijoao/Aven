

#include <aven/Aven.h>
#include <aven/objects/BrickPool.h>
#include <aven/render/Renderer.h>

namespace aven{


	Renderer::Renderer():
		framebuffer_renderer(uvec2(32, 32)),
		framebuffer_result(uvec2(32, 32)),
		program_render(gl::loadProgram({	{ gl::ShaderType::Vertex,	"shader/render/renderer_vert.glsl"}, 
											{ gl::ShaderType::Fragment,	"shader/render/renderer_frag.glsl" }})),
		program_tonemap(gl::loadProgram({	{ gl::ShaderType::Vertex,	"shader/render/tonemap_vert.glsl"}, 
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
		if(hasReachedFinalIteration())
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
		program_tonemap.setFloat("gamma", aven::getProject().getScene().renderSettings->gamma.getValue());
		vao.renderTriangles();

		//restore viewport
		glViewport(size_viewport[0], size_viewport[1], size_viewport[2], size_viewport[3]);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		renderIteration++;
	}



	void Renderer::resetIterations() {
		renderIteration = 0;
	}

	bool Renderer::hasReachedFinalIteration() {
		auto camera = aven::getProject().getScene().renderSettings;
		return renderIteration >= camera->maxSamples.getValue();
	}



	void Renderer::resize(int width, int height) {
		framebuffer_result.resize({ width, height });
		framebuffer_renderer.resize({ width, height });
		aven::getProject().camera.aspectRatio = (float)width / (float)height;
		resetIterations();
	}


	gl::Texture2D_rgba8u const& Renderer::getTexture() const {
		return framebuffer_result.getTexture();
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

		auto rs = aven::getProject().getScene().renderSettings;
		auto cam = aven::getProject().camera;
		auto coordSystem	= cam.getCoordSystemofImagePlane();
		program_render.setVec3("camera.pos",						cam.pos);
		program_render.setVec2("camera.invFilmSize",				vec2(1.0f) / vec2(framebuffer_renderer.getSize()));
		program_render.setVec3("camera.right",						std::get<0>(coordSystem));
		program_render.setVec3("camera.up",							std::get<1>(coordSystem));
		program_render.setVec3("camera.forward",					std::get<2>(coordSystem));
		program_render.setInt("nbrSamplesPerIteration",				rs->nbrSamplesPerIteration);


		//volume.glsl
		auto volume = aven::getProject().getScene().volume;
		program_render.setInt3("volume_size",					volume->getSize());
		program_render.setVec3("invVolumeSize",					vec3(1.0f) / vec3(volume->getSize()));
		program_render.setFloat("volume_stepSize",				volume->stepSize);
		program_render.setFloat("volume_density",				volume->density);
		program_render.setVec3("volume_pos",					volume->transformation.getPos());
		program_render.setInt("volune_isDisplayingBoundingBox",	volume->isRendering_BondingBox?1:0);
		program_render.setInt("volume_renderModeHybrid",		volume->renderingMode_Hybrid?1:0);

		//lights
		auto& lights = aven::getProject().getScene().getLights();
		program_render.setInt("nbrOfLights", lights.size());
		for (int i = 0; i < lights.size(); i++) {
			std::string s_i = std::to_string(i);
			program_render.setMat4x4("lights[" + s_i + "].transformation",			lights[i]->transformation.getMatrix());
			program_render.setVec3("lights[" + s_i + "].color",						lights[i]->color.getValue() * lights[i]->intensity);
			program_render.setInt("lights[" + s_i + "].type",						static_cast<int>(lights[i]->type));
			program_render.setFloat("lights[" + s_i  + "].falloff_inCosRadians",	std::cos(toRadians(lights[i]->falloff_angle_degrees)));
		}

		program_render.setVec3("background_sky",		rs->backgroundColor_sky);
		program_render.setVec3("background_ground",		rs->backgroundColor_ground);
		program_render.setInt("background_useAsLight",	rs->useBackgroundAsLight ? 1 : 0);
		program_render.setFloat("background_intensity", rs->backgroundIntensity);
			

		brickPool::bindSSBO_toBufferBase0();
		auto& op= aven::getProject().currentToolOperation;
		if(!op){
			program_render.setInt("volume_type", 0);
			volume->getSSBO().bindBufferBase(1);
		}else{
			program_render.setInt("volume_type", static_cast<int>(op->blendMode));

			volume->getSSBO().bindBufferBase(1);
			op->volumeData.getSSBO().bindBufferBase(2);
			program_render.setFloat("volume_opacity", static_cast<float>(op->opacity.getValue()) / 255.0f);
		}
	}

}



