#include "window.h"

int main()
{
	glfwInit();
	Window window("Test Window", 1024, 800);

	while (!glfwWindowShouldClose(window.getNativeWindow()))
	{
		window.onUpdate(1.f / 60.f);
	}

	window.close();
	glfwTerminate();
	return 0;
}