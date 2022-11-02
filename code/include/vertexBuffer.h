/* \file VertexBuffer.h */
#pragma once

#include "bufferLayout.h"

class VertexBuffer
{
public:
	VertexBuffer(void* vertices, uint32_t size, VertexBufferLayout layout);
	~VertexBuffer();
	void bind(); //!< Bind so that the currently bound vertex buffer is this one
	void unbind(); //!< Unbind so that no vertex buffer is currently bound
	void edit(void* vertices, uint32_t size, uint32_t offset);
	inline uint32_t getID() const { return m_OpenGL_ID; }
	inline const VertexBufferLayout& getLayout() const  { return m_layout; }
private:
	uint32_t m_OpenGL_ID; //!< Render ID
	VertexBufferLayout m_layout; //!< Buffer layout
};
