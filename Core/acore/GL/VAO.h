#pragma once

#include <3rdParty/GL/glad.h>
#include <acore/math.h>
#include <vector>


namespace aven {
namespace gl {
	class Vao {
	public:
		Vao(std::vector<vec3> const &verts);

		Vao(Vao const&) = delete;
		Vao(Vao&&);

		~Vao();

		void renderTriangles()	const;
		void renderLines()		const;
		void renderLineLoop()	const;

		GLuint getId() const;
		unsigned int getNbrVertecies() const;
	private:
		GLuint id;
		GLuint vbo;
		unsigned int nbrVertecies;
	};
}
}