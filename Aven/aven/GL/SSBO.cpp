
#include <assert.h>
#include <aven/GL/SSBO.h>
#include <utility>


namespace aven {
	namespace gl{

		SSBO::SSBO(GLsizeiptr size_bytes)
			:size_bytes(size_bytes) 
		{

			glGenBuffers(1, &name);
			glBindBuffer( GL_SHADER_STORAGE_BUFFER, name);
			glBufferData(GL_SHADER_STORAGE_BUFFER, size_bytes, NULL, GL_DYNAMIC_COPY); 
			assert(!glGetError());
		}


		SSBO::~SSBO() {
			glDeleteBuffers(1, &name);	//silently ignores ids that are equal 0
			assert(!glGetError());
		}


		SSBO::SSBO(SSBO&& other) noexcept
			: name(other.name), size_bytes(other.size_bytes)
		{
			other.name			= 0;
			other.size_bytes	= 0;
		}


		SSBO& SSBO::operator= (SSBO&& other) noexcept{
			std::swap(name,			other.name);
			std::swap(size_bytes,	other.size_bytes);
			return *this;
		}


		GLsizeiptr SSBO::getSizeInBytes() const {
			return size_bytes;
		}


		void SSBO::bindBufferBase(GLuint bindingPoint) const {
			assert(name);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, name);
			assert(!glGetError());
		}


		void SSBO::setSubData(void const* data, GLintptr  offset_bytes, GLsizeiptr  size_bytes) const {
			assert(this->size_bytes >= size_bytes + offset_bytes);
			glNamedBufferSubData(name, offset_bytes, size_bytes, data);
			assert(!glGetError());
		}

		void SSBO::setData(void const* data, GLsizeiptr size_bytes) const {
			setSubData(data, 0, size_bytes);
		}

		void SSBO::getData(void* dataOut) const {
			return getDataRange(dataOut, 0, size_bytes);
		}

		void SSBO::getDataRange(void* dataOut, GLintptr offset_bytes, GLsizeiptr length_bytes) const {
			assert(offset_bytes + length_bytes <= size_bytes);
			glGetNamedBufferSubData(name, offset_bytes, length_bytes, dataOut);
			assert(!glGetError());
		}

	}
}