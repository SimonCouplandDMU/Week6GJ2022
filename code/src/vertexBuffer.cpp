/* \file VertexBuffer.cpp */

#include <glad/glad.h>
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(void* vertices, uint32_t size, VertexBufferLayout layout) : m_layout(layout)
{
	glCreateBuffers(1, &m_OpenGL_ID);
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_OpenGL_ID);
}

void VertexBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_OpenGL_ID);
}

void VertexBuffer::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::edit(void* vertices, uint32_t size, uint32_t offset)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_OpenGL_ID);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
}
