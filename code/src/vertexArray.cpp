/* \file VertexArray.cpp*/
#include <glad/glad.h>
#include <iostream>
#include "VertexArray.h"



namespace SDT
{
	static GLenum toGLType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Byte4: return GL_UNSIGNED_BYTE;
		case ShaderDataType::FlatByte: return GL_BYTE;
		case ShaderDataType::FlatInt: return GL_INT;
		case ShaderDataType::Int: return GL_INT;
		case ShaderDataType::Float: return GL_FLOAT;
		case ShaderDataType::Float2: return GL_FLOAT;
		case ShaderDataType::Float3: return GL_FLOAT;
		case ShaderDataType::Float4: return GL_FLOAT;
		case ShaderDataType::Mat4: return GL_FLOAT;
		default: return GL_INVALID_ENUM;
		}
	}
}
VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_OpenGL_ID);
	bind();
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_OpenGL_ID);
}

void VertexArray::bind()
{
	glBindVertexArray(m_OpenGL_ID);
}

void VertexArray::unbind()
{
	glBindVertexArray(0);
}

void VertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	m_vertexBuffers.push_back(vertexBuffer);

	glBindVertexArray(m_OpenGL_ID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->getID());

	const auto& layout = vertexBuffer->getLayout();
	for (const auto& element : layout)
	{
		uint32_t normalised = GL_FALSE;
		if (element.m_normalised) {
			normalised = GL_TRUE;
		}

		if (element.m_dataType == ShaderDataType::FlatInt || element.m_dataType == ShaderDataType::FlatByte)
		{
			glEnableVertexAttribArray(m_attributeIndex);
			glVertexAttribIPointer(m_attributeIndex,
				SDT::componentCount(element.m_dataType),
				SDT::toGLType(element.m_dataType),
				layout.getStride(),
				(const void*)element.m_offset);
			glVertexAttribDivisor(m_attributeIndex, element.m_instanceDivisor);
			m_attributeIndex++;
		}
		else if (element.m_dataType == ShaderDataType::Mat4)
		{
			uint8_t count = SDT::componentCount(element.m_dataType);
			for (uint8_t i = 0; i < count; i++)
			{
				glEnableVertexAttribArray(m_attributeIndex);
				glVertexAttribPointer(m_attributeIndex,
					count,
					SDT::toGLType(element.m_dataType),
					element.m_normalised ? GL_TRUE : GL_FALSE,
					layout.getStride(),
					(const void*)(sizeof(float) * count * i));
				glVertexAttribDivisor(m_attributeIndex, element.m_instanceDivisor);
				m_attributeIndex++;
			}
		}
		else
		{
			glEnableVertexAttribArray(m_attributeIndex);
			glVertexAttribPointer(
				m_attributeIndex,
				SDT::componentCount(element.m_dataType),
				SDT::toGLType(element.m_dataType),
				normalised,
				layout.getStride(),
				(void*)element.m_offset); // position
			glVertexAttribDivisor(m_attributeIndex, element.m_instanceDivisor);
			m_attributeIndex++;
		}


	}
}

void VertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	m_indexBuffer = indexBuffer;
}
