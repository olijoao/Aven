#pragma once


#include <3rdParty/GL/glad.h>
#include <aven/util/math.h>
#include <string>


namespace aven {

	//C++ wrapper for openglGL 4.6 core shaders
	namespace gl {


		class ShaderException : public std::exception {
		public:
			ShaderException(std::string s) { this->s = s; }
			char const* what()	const { return s.c_str(); }
			void append(std::string s) { this->s += s; }
			void appendFront(std::string s) { this->s = s + this->s; }
		private:
			std::string s;
		};

		enum class ShaderType : GLenum {
			Vertex = GL_VERTEX_SHADER,
			Tessellation_Control = GL_TESS_CONTROL_SHADER,
			Tessellation_Evaluation = GL_TESS_EVALUATION_SHADER,
			Geometry = GL_GEOMETRY_SHADER,
			Fragment = GL_FRAGMENT_SHADER,
			Compute = GL_COMPUTE_SHADER
		};


		class Shader {
		public:
			Shader(ShaderType type, std::string const& code);

			Shader(Shader&&) noexcept;
			Shader& operator=(Shader&&) noexcept;

			Shader(Shader const&) = delete;
			Shader& operator=(Shader const&) = delete;

			~Shader();

			GLuint getName() const;

		private:
			GLuint name;
		};


	} //namespace gl

}	// namespace aven