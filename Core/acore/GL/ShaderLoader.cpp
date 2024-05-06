

#include <acore/GL/ShaderLoader.h>
#include <fstream>
#include <regex>
#include <unordered_set>

namespace aven {
namespace gl {

	namespace {
		// parses #include "filename"
		std::string load_and_preprocess(std::string path, std::unordered_set<std::string>& alreadyIncludedPaths) {
			std::regex const rgx(R"(#[\s\t]*include[\s\t]*\"((?:.)*)\")");

			alreadyIncludedPaths.insert(path);

			std::string line;
			std::string shadersource;
			std::ifstream file(path);

			if(!file.is_open())
				throw ShaderLoaderException("Could not open file: \"" + path + "\". \n");

			while (getline(file, line)) {
				std::smatch smatch;
				std::string::const_iterator itSearch(line.cbegin());

				if (regex_search(itSearch, line.cend(), smatch, rgx)) {
					if (alreadyIncludedPaths.find(smatch[1].str()) == alreadyIncludedPaths.end()) {
						try {
							shadersource += load_and_preprocess(smatch[1].str(), alreadyIncludedPaths) + "\n";
						}
						catch (ShaderLoaderException& e) {
							e.prepend("Could not include \""+ smatch[1].str() +"\" into \"" + path + "\" \n");
							throw;
						}
					}
				}else {
					shadersource += line + "\n";
				}
			}
	
			return shadersource;

		}
	}


	Program loadProgram(std::vector<std::pair<ShaderType, std::string>> const& shaderPaths) {
		std::vector<Shader> shaders;

		for (auto path : shaderPaths) {
			try {
				shaders.push_back(std::move(loadShader(path.first, path.second)));
			}catch (ShaderException& except) {
				std::string msg = "Error loading Shader: "+ path.second+" \n";
				except.appendFront(msg);
				throw;
			}
		}

		try {
			return Program(shaders);
		}
		catch (ShaderException& except) {
			std::string msg = "Error loading Program with following shaders: \n";
			
			for (auto const& path: shaderPaths) 
				msg += "  -"+path.second + "\n";
			
			except.appendFront(msg);
			throw;
		}
	}

	Shader loadShader(ShaderType type, std::string path) {
		std::unordered_set<std::string> alreadyIncludedPaths;
		auto code = load_and_preprocess(path, alreadyIncludedPaths);
		return Shader(type, code);
	}
}

}	//namespace aven