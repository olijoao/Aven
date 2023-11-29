

#include <aven/GL/Shader.h>
#include <memory>


namespace aven {
namespace gl {

	std::string toString(ShaderType type) {
		switch (type) {
			case ShaderType::Vertex:					return "Vertex";					
			case ShaderType::Tessellation_Control:		return "Tessellation_Control";	
			case ShaderType::Tessellation_Evaluation:	return "Tessellation_Evaluation";	
			case ShaderType::Geometry:					return "Geometry";				
			case ShaderType::Fragment:					return "Fragment";				
			case ShaderType::Compute:					return "Compute";					
			default:									return "UndefinedShaderType";
		}
	}


	Shader::Shader(ShaderType type, std::string const& code) {
		name = glCreateShader(static_cast<GLenum>(type));
		assert(!glGetError());

		//GLchar const*;
		auto c_str = code.c_str();
		glShaderSource(name, 1, &c_str, NULL);
		assert(!glGetError());

		glCompileShader(name);
		assert(!glGetError());

		GLint compilation_success;
		glGetShaderiv(name, GL_COMPILE_STATUS, &compilation_success);
		assert(!glGetError());

		if (!compilation_success) {
			GLint log_length;
			glGetShaderiv(name, GL_INFO_LOG_LENGTH, &log_length);		
			assert(!glGetError());
			auto infoLog = std::make_unique<char[]>(log_length);
			glGetShaderInfoLog(name, log_length, NULL, infoLog.get());	
			assert(!glGetError());
			glDeleteShader(name);
			assert(!glGetError());
			throw(ShaderException( toString(type) + " Shader compilation failed. " + "\n" + std::string(infoLog.get())));
		}
	}

	Shader::~Shader() {
		//silently ignores value 0
		glDeleteShader(name);
	}


	Shader::Shader(Shader&& other) noexcept{
		this->name = other.name;
		other.name = 0;		// glDeleteShader silently ignores value 0
	}


	Shader& Shader::operator=(Shader&& other) noexcept {
		std::swap(name, other.name);
		return *this;
	}


	GLuint Shader::getName() const {
		return name;
	}

}
}	// namespace aven