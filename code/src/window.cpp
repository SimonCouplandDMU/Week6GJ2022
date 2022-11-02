/* \file window.cpp*/

#include <glad/glad.h>
#include "window.h"
#include <iostream>

Window::Window(const char* title, uint32_t width, uint32_t height)
{
	m_width = width;
	m_height = height;
	
	m_native = glfwCreateWindow(m_width, m_height, title, nullptr, nullptr);

	glfwMakeContextCurrent(m_native);
	auto result = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	if (!result) std::cout << "Could not create OpenGL context for current GLFW window: " << result;

	// Enable OpenGL debug with a callback
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(
		[](
			GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam
			)
		{
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:
			case GL_DEBUG_SEVERITY_MEDIUM:
			case GL_DEBUG_SEVERITY_LOW:
				std::cout << message;
				break;
			}
		}
	, nullptr);

}

void Window::close()
{
	glfwDestroyWindow(m_native);
}

void Window::onUpdate(float timestep)
{
	glfwPollEvents();
	glfwSwapBuffers(m_native);
}
