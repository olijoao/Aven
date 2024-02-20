

#include <aven/GL/Program.h>

namespace aven {
namespace gl {

	Program::Program(std::vector<Shader> const& shaders) {
		name = glCreateProgram();
		assert(!glGetError());

		for (auto const& shader : shaders) {
			glAttachShader(name, shader.getName());
			assert(!glGetError());
		}

		glLinkProgram(name);
		assert(!glGetError());

		GLint success;
		glGetProgramiv(name, GL_LINK_STATUS, &success);

		if (!success) {
			GLint log_length;
			glGetProgramiv(name, GL_INFO_LOG_LENGTH, &log_length);
			assert(!glGetError());	
			std::unique_ptr<char[]> infoLog = std::make_unique<char[]>(log_length);
			glGetProgramInfoLog(name, log_length, NULL, infoLog.get());
			assert(!glGetError());	
			glDeleteProgram(name);
			assert(!glGetError());	
			throw(ShaderException( std::string(" Shader Program linking failed. ") + "\n" + std::string(infoLog.get())));
		}
		cacheUniformLocations();
		assert(!glGetError());
	}




	Program::Program(Program&& other) noexcept{
		this->name = other.name;
		other.name = 0;	// glDeleteShader silently ignores value 0

		this->cached_uniformLocations = std::move(other.cached_uniformLocations);
	}


	Program& Program::operator=(Program&& other) noexcept {
		std::swap(name, other.name);
		std::swap(this->cached_uniformLocations, other.cached_uniformLocations);
		return *this;
	}


	Program::~Program() {
		//silently ignores value 0
		glDeleteProgram(name);
	}



	GLuint Program::getName() const {
		return name;
	}

	void Program::setInt(std::string const& name, int value) const {
		glProgramUniform1i(this->name, getCachedActiveUniformLocation(name), value);
		assert(!glGetError());
	}

	void Program::setUint(std::string const& name, unsigned int value) const {
		glProgramUniform1ui(this->name, getCachedActiveUniformLocation(name), value);
		assert(!glGetError());
	}

	void Program::setVec2(std::string const& name, float x, float y) const {
		glProgramUniform2f(this->name, getCachedActiveUniformLocation(name), x, y);
		assert(!glGetError());
	}

	void Program::setVec2(std::string const& name, vec2 v) const {
		glProgramUniform2f(this->name, getCachedActiveUniformLocation(name), v.x, v.y);
		assert(!glGetError());
	}

	void Program::setVec3(std::string const& name, float x, float y, float z) const {
		glProgramUniform3f(this->name, getCachedActiveUniformLocation(name), x, y, z);
		assert(!glGetError()); 
	}

	void Program::setVec3(std::string const& name, vec3 v) const {
		glProgramUniform3f(this->name, getCachedActiveUniformLocation(name), v.x, v.y, v.z);
		assert(!glGetError());
	}

	void Program::setVec4(std::string const& name, vec4 v) const {
		glProgramUniform4f(this->name, getCachedActiveUniformLocation(name), v.x, v.y, v.z, v.w);
		assert(!glGetError()); 
	}

	void Program::setInt2(std::string const& name, int x, int y) const {
		glProgramUniform2i(this->name, getCachedActiveUniformLocation(name), x, y);
		assert(!glGetError());
	}

	void Program::setInt3(std::string const& name, int x, int y, int z)const {
		glProgramUniform3i(this->name, getCachedActiveUniformLocation(name), x, y, z);
		assert(!glGetError());
	}

	void Program::setInt3(std::string const& name, ivec3 v)	const {
		glProgramUniform3i(this->name, getCachedActiveUniformLocation(name), v.x, v.y, v.z);
		assert(!glGetError());
	}

	void Program::setUvec2(std::string const& name, unsigned int x, unsigned int y) const {
		glProgramUniform2ui(this->name, getCachedActiveUniformLocation(name), x, y);
		assert(!glGetError());
	}

	void Program::setUvec2(std::string const& name, uvec2 v) const {
		glProgramUniform2ui(this->name, getCachedActiveUniformLocation(name), v.x, v.y);
		assert(!glGetError());
	}

	void Program::setUvec3(std::string const& name, unsigned int x, unsigned int y, unsigned int z)const {
		glProgramUniform3ui(this->name, getCachedActiveUniformLocation(name), x, y, z);
		assert(!glGetError()); 
	}

	void Program::setUvec3(std::string const& name, uvec3 v)	const {
		glProgramUniform3ui(this->name, getCachedActiveUniformLocation(name), v.x, v.y, v.z);
		assert(!glGetError()); 
	}

	void Program::setFloat(std::string const& name, float f) const {
		glProgramUniform1f(this->name, getCachedActiveUniformLocation(name), f);
		assert(!glGetError()); 
	}

	void Program::setMat4x4(std::string const& name, mat4 const& mat)const {
		glProgramUniformMatrix4fv(this->name, getCachedActiveUniformLocation(name), 1, true, &mat[0][0]);
		assert(!glGetError()); 
	}

	void Program::setUniform(std::string const& name, int value) const {
		glProgramUniform1i(this->name, getCachedActiveUniformLocation(name), value);
		assert(!glGetError());
	}

	void Program::setUniform(std::string const& name, unsigned int value) const {
		glProgramUniform1ui(this->name, getCachedActiveUniformLocation(name), value);
		assert(!glGetError()); 
	}

	void Program::setUniform(std::string const& name, float x, float y) const {
		glProgramUniform2f(this->name, getCachedActiveUniformLocation(name), x, y);
		assert(!glGetError());
	}

	void Program::setUniform(std::string const& name, vec2 v) const {
		glProgramUniform2f(this->name, getCachedActiveUniformLocation(name), v.x, v.y);
		assert(!glGetError()); 
	}

	void Program::setUniform(std::string const& name, float x, float y, float z) const {
		glProgramUniform3f(this->name, getCachedActiveUniformLocation(name), x, y, z);
		assert(!glGetError()); 
	}

	void Program::setUniform(std::string const& name, vec3 v) const {
		glProgramUniform3f(this->name, getCachedActiveUniformLocation(name), v.x, v.y, v.z);
		assert(!glGetError()); 
	}

	void Program::setUniform(std::string const& name, vec4 v) const {
		glProgramUniform4f(this->name, getCachedActiveUniformLocation(name), v.x, v.y, v.z, v.w);
		assert(!glGetError()); 
	}

	void Program::setUniform(std::string const& name, int x, int y) const {
		glProgramUniform2i(this->name, getCachedActiveUniformLocation(name), x, y);
		assert(!glGetError());
	}

	void Program::setUniform(std::string const& name, int x, int y, int z)const {
		glProgramUniform3i(this->name, getCachedActiveUniformLocation(name), x, y, z);
		assert(!glGetError());
	}

	void Program::setUniform(std::string const& name, ivec3 v)	const {
		glProgramUniform3i(this->name, getCachedActiveUniformLocation(name), v.x, v.y, v.z);
		assert(!glGetError()); 
	}

	void Program::setUniform(std::string const& name, unsigned int x, unsigned int y) const {
		glProgramUniform2ui(this->name, getCachedActiveUniformLocation(name), x, y);
		assert(!glGetError());
	}

	void Program::setUniform(std::string const& name, uvec2 v) const {
		glProgramUniform2ui(this->name, getCachedActiveUniformLocation(name), v.x, v.y);
		assert(!glGetError());
	}

	void Program::setUniform(std::string const& name, unsigned int x, unsigned int y, unsigned int z)const {
		glProgramUniform3ui(this->name, getCachedActiveUniformLocation(name), x, y, z);
		assert(!glGetError()); 
	}

	void Program::setUniform(std::string const& name, uvec3 v)	const {
		glProgramUniform3ui(this->name, getCachedActiveUniformLocation(name), v.x, v.y, v.z);
		assert(!glGetError());
	}

	void Program::setUniform(std::string const& name, float f) const {
		glProgramUniform1f(this->name, getCachedActiveUniformLocation(name), f);
		assert(!glGetError());
	}

	void Program::setUniform(std::string const& name, mat4 const& mat)const {
		glProgramUniformMatrix4fv(this->name, getCachedActiveUniformLocation(name), 1, true, &mat[0][0]);
		assert(!glGetError()); 
	}



	void Program::cacheUniformLocations() {
		assert(cached_uniformLocations.empty());

		GLint maxUniformNameLen, nbrOfUniforms;
		glGetProgramiv(name, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLen);
		assert(!glGetError());
		glGetProgramiv(name, GL_ACTIVE_UNIFORMS, &nbrOfUniforms);
		assert(!glGetError());

		//only needed to have something to pass into glGetActiveUniform(), not used otherwise...
		GLint nameLength, size;
		GLenum type;

		std::vector<GLchar>unifN(maxUniformNameLen, 0);

		for (GLint i = 0; i < nbrOfUniforms; i++) {
			glGetActiveUniform(name, i, maxUniformNameLen, &nameLength, &size, &type, unifN.data());
			assert(!glGetError());
			cached_uniformLocations[unifN.data()] = glGetUniformLocation(name, unifN.data());	
		}
		assert(!glGetError());
	}


	// Returns location of an active uniform
	// If it can't be found either because the uniformName doesnt exist or isn't active 
	//		returns -1, since Uniform* commands will silently ignore the
	//		data passed in, and the current uniform values will not be changed.

	GLint Program::getCachedActiveUniformLocation(std::string const& uniformName) const {
		auto it = cached_uniformLocations.find(uniformName);
		if (it != cached_uniformLocations.end())
			return it->second;
		return -1;
	}



	void use(Program const& program) {
		glUseProgram(program.getName());
	}


	void dispatch(Program const& program, unsigned int x, unsigned int y, unsigned int z) {
		glUseProgram(program.getName());
		glDispatchCompute(x, y, z);
		assert(!glGetError());
	}




}	// namespace gl
}	// namespace aven