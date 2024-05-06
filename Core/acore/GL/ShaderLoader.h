#pragma once

#include <acore/GL/Program.h>


namespace aven {
	namespace gl {


		class ShaderLoaderException : public std::exception {
		public:
			ShaderLoaderException(std::string s)	{ this->s = s; }
			char const* what()	const				{ return s.c_str(); }
			void append(std::string s)				{ this->s += s; }
			void prepend(std::string s)				{ this->s = s + this->s; }
		private:
			std::string s;
		};


		Program loadProgram(std::vector<std::pair<ShaderType, std::string>> const& shaderPaths);
		Shader loadShader(ShaderType type, std::string path);

	}
}