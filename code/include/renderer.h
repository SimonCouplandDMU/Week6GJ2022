/* \file renderer.h*/
#pragma once

#include <glm/glm.hpp>
#include <array>
#include <memory>
#include "texture.h"
#include "shader.h"
#include "VertexArray.h"

class Quad
{
public:
	Quad() = default;
	static Quad createCentreHalfExtents(const glm::vec2& centre, const glm::vec2& halfExtents, float angle, bool degrees = true);
private:
	glm::vec2 m_position;
	glm::vec2 m_halfExtents;
	float m_angle;
	friend class Renderer;
};

class Renderer
{
public:
	// Init
	static void init();
	// Basic commands
	static void clearScreen();
	static void setClearColour(const glm::vec4& colour);
	static void enableStandardBlend();
	static void enableAdditiveBlend();
	static void disableBlend();
	static void enableDepthTest();
	static void disableDepthTest();
	// Draw command
	static void begin(const glm::mat4& view, const glm::mat4& projection);
	static void end();
	static void drawQuad(const Quad& quad, const Texture& texture, const glm::vec4& tint);
private:
	struct RendererData
	{
		RendererData() {};
		glm::vec3 clearColour = glm::vec3(1.f);
		std::shared_ptr<Shader> shader;
		std::shared_ptr<VertexArray> VAO;
	};

	static RendererData s_data;
};