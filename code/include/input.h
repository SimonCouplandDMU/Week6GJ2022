/* \file input.h */
#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>


/* \class Input
*	Input poller for getting current keyboard/mouse state specific to GLFW
*/
class Input
{
public:
	static bool isKeyPressed(int32_t keyCode); //!< is the key pressed?
	static bool isMouseButtonPressed(int32_t mouseButton); // is the mouse button pressed
	static glm::vec2 getMousePosition(); //!< current mouse position
	static void setCurrentWindow(GLFWwindow* newWin) { s_window = newWin; }
private:
	static GLFWwindow* s_window; //!< Current GLFW window
};

