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

	while (!glfwWindowShouldClose(window.getNativeWindow()))
	{
		frameTime = timer.reset();

		scene.onUpdate(frameTime);
		scene.onRender();
		
		window.onUpdate(frameTime);
	}

	window.close();
	glfwTerminate();
	return 0;
}