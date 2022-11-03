/* \file Input.cpp */

#include "input.h"

GLFWwindow* Input::s_window = nullptr;

bool Input::isKeyPressed(int32_t keyCode)
{
	if (s_window)
	{
		auto answer = glfwGetKey(s_window, keyCode);
		return answer == GLFW_PRESS || answer == GLFW_REPEAT;
	}
	return false;
}

bool Input::isMouseButtonPressed(int32_t mouseButton)
{
	if (s_window)
	{
		auto answer = glfwGetMouseButton(s_window, mouseButton);
		return answer == GLFW_PRESS;
	}
	return false;
}

glm::vec2 Input::getMousePosition()
{
	if (s_window)
	{
		double x, y;
		glfwGetCursorPos(s_window, &x, &y);
		return glm::vec2(static_cast<float>(x), static_cast<float>(y));
	}
	return { -1.f,1.f };
}
