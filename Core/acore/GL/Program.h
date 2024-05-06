#pragma once

#include <acore/GL/Shader.h>
#include <acore/clamped.h>
#include <acore/math.h>
#include <memory>
#include <unordered_map>
#include <vector>

namespace aven {
namespace gl{

	class Program {
	public:
		Program(std::vector<Shader> const& shaders);

		Program(Program const&)				= delete;
		Program& operator=(Program const&)	= delete;

		Program(Program&&) noexcept;
		Program& operator=(Program&&) noexcept;

		~Program();

		GLuint getName() const;

		void setInt		(std::string const& name,	int value)											const;
		void setInt2	(std::string const&	name,	int x, int y)										const;
		void setInt3	(std::string const&	name,	int x, int y, int z)								const;
		void setInt3	(std::string const&	name,	ivec3 v)											const;
		void setUint	(std::string const&	name,	unsigned int value)									const;
		void setUvec2	(std::string const&	name,	uvec2 v)											const;
		void setUvec2	(std::string const&	name,	unsigned int x, unsigned int y)						const;
		void setUvec3	(std::string const&	name,	unsigned int x, unsigned int y, unsigned int z)		const;
		void setUvec3	(std::string const&	name,	uvec3 v)											const;
		void setFloat	(std::string const&	name,	float f)											const;
		void setVec2	(std::string const&	name,	float x, float y)									const;
		void setVec2	(std::string const&	name,	vec2 v)												const;
		void setVec3	(std::string const&	name,	float x, float y, float z)							const;
		void setVec3	(std::string const&	name,	vec3 v)												const;
		void setVec4	(std::string const&	name,	vec4 v)												const;
		void setMat4x4	(std::string const& name,	mat4 const & mat)									const;
	

		template <typename T, T MIN, T MAX>
		void setUniform(std::string const& name, clamped <T, MIN, MAX> value) const {
			setUniform(name, value.getValue());
		}

		void setUniform(std::string const& name, int value)											const;
		void setUniform(std::string const& name, int x, int y)										const;
		void setUniform(std::string const& name, int x, int y, int z)								const;
		void setUniform(std::string const& name, ivec3 v)											const;
		void setUniform(std::string const& name, unsigned int value)								const;
		void setUniform(std::string const& name, uvec2 v)											const;
		void setUniform(std::string const& name, unsigned int x, unsigned int y)					const;
		void setUniform(std::string const& name, unsigned int x, unsigned int y, unsigned int z)	const;
		void setUniform(std::string const& name, uvec3 v)											const;
		void setUniform(std::string const& name, float f)											const;
		void setUniform(std::string const& name, float x, float y)									const;
		void setUniform(std::string const& name, vec2 v)											const;
		void setUniform(std::string const& name, float x, float y, float z)							const;
		void setUniform(std::string const& name, vec3 v)											const;
		void setUniform(std::string const& name, vec4 v)											const;
		void setUniform(std::string const& name, mat4 const& mat)									const;

	private:
		GLuint name;
		std::unordered_map<std::string, GLint> cached_uniformLocations;
		
		void cacheUniformLocations();
		GLint getCachedActiveUniformLocation(std::string const& uniformName) const;

	};


	void use(Program const&);
	void dispatch(Program const&, unsigned int x = 1, unsigned int y = 1, unsigned int z = 1);

}
}	//namespace aven