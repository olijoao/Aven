#pragma once

#include <3rdParty/GL/glad.h>


namespace aven {
	namespace gl {

		class SSBO {
		public:
			SSBO(GLsizeiptr size_bytes);
			~SSBO();
			SSBO(SSBO&&) noexcept;
			SSBO& operator= (SSBO&& other) noexcept;
			SSBO(SSBO const&)			= delete;
			SSBO operator=(SSBO const&) = delete;

			GLsizeiptr getSizeInBytes() const;

			void bindBufferBase(GLuint bindingPoint) const;

			void setSubData(void const* data , GLintptr offset_bytes, GLsizeiptr  size_bytes) const;
			void setData(void const* data, GLsizeiptr size_bytes) const;

			void getData(void* dataOut) const;
			void getDataRange(void* dataOut, GLintptr offset, GLsizeiptr length) const;

		private:
			GLuint name;
			GLsizeiptr size_bytes;
		};

	}
}