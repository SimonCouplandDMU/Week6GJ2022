/* \file VertexArray */
#pragma once

#include <memory>
#include "vertexBuffer.h"
#include "indexBuffer.h"


class VertexArray
{
private:
	std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers; //!< Container for vertex buffers
	std::shared_ptr<IndexBuffer> m_indexBuffer; //!< Index buffer for all vertex buffers
	uint32_t m_OpenGL_ID; //!< Id of the buffer in OpenGL
	uint32_t m_attributeIndex = 0; //!< Index of next buffer attribute to be set
public:
	VertexArray(); //!< Constructor
	~VertexArray(); //!< Destructor;
	void bind(); //!< Bind so that the currently bound vertex array is this one
	void unbind(); //!< Unbind so that no vertex array is currently bound
	inline uint32_t getID() const { return m_OpenGL_ID; } //!<Get the render ID of this vertex array
	void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer); //!< Add a vertex buffer
	void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer); //!< Set the index buffer for all vertex buffer
	inline std::vector<std::shared_ptr<VertexBuffer>> getVertexBuffers() const //!< Get the vertex buffers
	{
		return m_vertexBuffers;
	}
	inline std::shared_ptr<IndexBuffer> getIndexBuffer() const //!< Get the index buffer
	{
		return m_indexBuffer;
	}
	inline uint32_t getDrawCount() const 
	{
		return m_indexBuffer->getCount();
	}; //!< returns the number of primitives to draw
};

