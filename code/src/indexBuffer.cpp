/* \file IndexBuffer.cpp*/

#include <glad/glad.h>
#include "IndexBuffer.h"


IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) : m_count(count)
{
	glCreateBuffers(1, &m_OpenGL_ID);
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indices, GL_DYNAMIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_OpenGL_ID);
}

void IndexBuffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_OpenGL_ID);
}

void IndexBuffer::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::edit(uint32_t* indices, uint32_t count, uint32_t offset)
{
	bind();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * offset, sizeof(uint32_t) * count, indices);
}
