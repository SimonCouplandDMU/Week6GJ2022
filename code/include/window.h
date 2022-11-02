/** \file window.h */
#pragma once

#include "GLFW/glfw3.h"


/** \class Window
* Implementation of a window using GLFW
*/

class Window
{
public:
	Window(const char * title, uint32_t width, uint32_t height); //!< Constructor
	void close() ; //!< Close the window
	void onUpdate(float timestep);
	inline uint32_t getWidth() const { return m_width; }
	inline uint32_t getHeight() const { return m_height; }
	inline float getWidthF() const { return static_cast<float>(m_width); }
	inline float getHeightF() const { return static_cast<float>(m_height); }
	inline GLFWwindow* getNativeWindow() const {return m_native;}

private:
	GLFWwindow * m_native; //!< Native GLFW window
	uint32_t m_width;
	uint32_t m_height;
};

