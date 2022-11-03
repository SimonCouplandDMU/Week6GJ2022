#include "window.h"
#include "input.h"
#include "timer.h"
#include "renderer.h"
#include "scene.h"

int main()
{
	glfwInit();
	Window window("Test Window", 1024, 800);
	Input::setCurrentWindow(window.getNativeWindow());

	Scene scene;

	Renderer::init();

	Timer timer;
	float frameTime = 0.f;

	glm::mat4 cameraTransform = glm::mat4(1.0f);
	glm::vec2 viewPort = { window.getWidthF() * 0.01f, window.getHeightF() * 0.01f };
	glm::mat4 view = glm::inverse(cameraTransform);
	glm::mat4 proj = glm::ortho(0.f, viewPort.x, 0.f, viewPort.y);

	while (!glfwWindowShouldClose(window.getNativeWindow()))
	{
		frameTime = timer.reset();

		scene.onUpdate(frameTime);
		
		Renderer::begin(view, proj);
		scene.onRender();
		Renderer::end();
		
		window.onUpdate(frameTime);
	}

	window.close();
	glfwTerminate();
	return 0;
}