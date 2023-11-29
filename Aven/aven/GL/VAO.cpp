
#include <assert.h>
#include <aven/GL/VAO.h>


namespace aven {
namespace gl {
	Vao::Vao(std::vector<vec3> const &verts) {
		this->nbrVertecies = verts.size();

		//vao
		glGenVertexArrays(1, &id);
		glBindVertexArray(id);

		//vbo
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * 3 * sizeof(float), &verts[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		assert(!glGetError());
	}



	Vao::Vao(Vao&& other) {
		this->id			= other.id;
		this->vbo			= other.vbo;
		this->nbrVertecies	= other.nbrVertecies;

		other.id			= 0;
		other.vbo			= 0;
		other.nbrVertecies	= 0;
	}


	Vao::~Vao() {
		glDeleteVertexArrays(1, &id);	// id=0 is silently ignored
		glDeleteBuffers(1, &vbo);		// vbo=0 is silently ignored
	}



	void Vao::renderTriangles() const {
		assert(nbrVertecies % 3 == 0);
		glBindVertexArray(id);
		glDrawArrays(GL_TRIANGLES, 0, nbrVertecies);
		assert(!glGetError());
	}

	void Vao::renderLines() const {
		assert(nbrVertecies % 2 == 0);
		glBindVertexArray(id);
		glDrawArrays(GL_LINES, 0, nbrVertecies);
		assert(!glGetError());
	}

	void Vao::renderLineLoop() const {
		glBindVertexArray(id);
		glDrawArrays(GL_LINE_LOOP, 0, nbrVertecies);
		assert(!glGetError());
	}


	GLuint Vao::getId()					const { return id; }
	unsigned int Vao::getNbrVertecies() const { return nbrVertecies; }

}
}