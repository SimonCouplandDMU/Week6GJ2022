/* \file renderer.cpp*/

#include "glad/glad.h"
#include "renderer.h"
#include <glm/gtc/matrix_transform.hpp>

Renderer::RendererData Renderer::s_data;

void Renderer::init()
{
	s_data.clearColour = glm::vec4(0.f, 0.f, 0.f, 1.f);
	
	s_data.shader.reset(new Shader("../assets/shaders/quad.vert", "../assets/shaders/quad.frag"));

	float vertices[4 * 4] = {
				-0.5f, -0.5f, 0.f, 1.f,
				-0.5f,  0.5f, 0.f, 0.f,
				 0.5f,  0.5f, 1.f, 0.f,
				 0.5f, -0.5f, 1.f, 1.f
	};

	uint32_t indices[4] = { 0,1,2,3 };

	std::shared_ptr<VertexBuffer> VBO;
	std::shared_ptr<IndexBuffer> IBO;

	s_data.VAO.reset(new VertexArray());
	VBO.reset(new VertexBuffer(vertices, sizeof(vertices), VertexBufferLayout({ ShaderDataType::Float2, ShaderDataType::Float2 })));
	IBO.reset(new IndexBuffer(indices, 4));

	s_data.VAO->addVertexBuffer(VBO);
	s_data.VAO->setIndexBuffer(IBO);

	// Unbind everything so we can't mess is up
	s_data.VAO->unbind();
	VBO->unbind();
	IBO->unbind();
}

void Renderer::begin(const glm::mat4& view, const glm::mat4& projection)
{
	s_data.shader->use();
	s_data.shader->uploadMat4("u_view", view);
	s_data.shader->uploadMat4("u_projection", projection);

	s_data.VAO->bind();
}

void Renderer::end()
{
	s_data.VAO->unbind();
	s_data.shader->stopUsing();
}

void Renderer::drawQuad(const Quad& quad, const Texture& texture, const glm::vec4& tint)
{
	texture.bindToSlot(0);
	s_data.shader->uploadInt("u_texData", 0); // Tell the shader to use texture at correct unit

	// Do geomertry tranforms
	glm::mat4 model = glm::scale(glm::rotate(glm::translate(glm::mat4(1.f), glm::vec3(quad.m_position, 0.0)),quad.m_angle, { 0.f, 0.f, 1.f }), glm::vec3(quad.m_halfExtents * 2.f, 1.f));
	s_data.shader->uploadFloat4("u_tint", tint);
	s_data.shader->uploadMat4("u_model", model);

	// Issue drawcall
	glDrawElements(GL_QUADS, s_data.VAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::clearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setClearColour(const glm::vec4& colour)
{
	glClearColor(colour.r, colour.g, colour.b, colour.a);
}

void Renderer::enableStandardBlend()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

void Renderer::enableAdditiveBlend()
{
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);
}

void Renderer::disableBlend()
{
	glDisable(GL_BLEND);
}

void Renderer::enableDepthTest()
{
	glEnable(GL_DEPTH_TEST);
}

void Renderer::disableDepthTest()
{
	glDisable(GL_DEPTH_TEST);
}




Quad Quad::createCentreHalfExtents(const glm::vec2& centre, const glm::vec2& halfExtents, float angle, bool degrees)
{
	Quad result;

	result.m_position = centre;
	result.m_halfExtents = halfExtents;
	result.m_angle = degrees ? glm::radians(angle) : angle;

	return result;
}