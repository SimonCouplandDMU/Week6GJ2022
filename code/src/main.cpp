#include "window.h"
#include "renderer.h"
#include "timer.h"
#include <glm/gtc/matrix_transform.hpp>

int main()
{
	glfwInit();
	Window window("Test Window", 1024, 800);

	Renderer::init();
	Renderer::setClearColour({ 1.f, 1.f, 1.f, 1.f });
	Quad q1 = Quad::createCentreHalfExtents({ 4.f, 0.5f }, { 3.f, 2.f });
	Quad q2 = Quad::createCentreHalfExtents({ 4.f, 7.f }, { 0.5f, 0.5f });

	Texture letterCube("../assets/textures/letterCube.png");
	unsigned char whitePx[3] = { 255,255,255 };
	Texture plainWhite(1, 1, 3, whitePx, 1);

	Timer timer;
	float frameTime = 0.f;

	glm::mat4 cameraTransform = glm::mat4(1.0f);
	glm::vec2 viewPort = { window.getWidthF() * 0.01f, window.getHeightF() * 0.01f };
	glm::mat4 view = glm::inverse(cameraTransform);
	glm::mat4 proj = glm::ortho(0.f, viewPort.x, 0.f, viewPort.y);

	while (!glfwWindowShouldClose(window.getNativeWindow()))
	{
		frameTime = timer.reset();
		Renderer::clearScreen();

		Renderer::begin(view, proj);
		Renderer::drawQuad(q1, letterCube, glm::vec4(1.f));
		Renderer::drawQuad(q2, plainWhite, glm::vec4(1.f, 0.f, 0.f, 1.f));
		Renderer::end();
		
		window.onUpdate(frameTime);
	}

	window.close();
	glfwTerminate();
	return 0;
}