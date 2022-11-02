/* \file IndexBuffer.h */
#pragma once

#include <inttypes.h>

class IndexBuffer
{
public:
	IndexBuffer(uint32_t* indices, uint32_t count);
	~IndexBuffer();
	void bind(); //!< Bind so that the currently bound index buffer is this one
	void unbind(); //!< Unbind so that no index buffer is currently bound
	inline uint32_t getID() const { return m_OpenGL_ID; }
	inline uint32_t getCount() const { return m_count; }
	void edit(uint32_t* indices, uint32_t count, uint32_t offset);
private:
	uint32_t m_OpenGL_ID; //!< Render ID
	uint32_t m_count; //!< Effective draw count
};

