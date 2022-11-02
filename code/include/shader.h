/* \file shader.h*/
#pragma once

#include <glm/glm.hpp>


class Shader
{
public:
	Shader(const char* vertexFilepath, const char* fragmentFilepath); //!< Constructor with two files
	~Shader(); //!< Destructor
	uint32_t getID() const { return m_OpenGL_ID; } //!< Get openGL ID
	void use(); //!< Use this shader, make it the currently bound shader program 
	void stopUsing(); //!< Stop using this shader
	void uploadInt(const char* name, int value); //!< Upload an integer
	void uploadIntArray(const char* name, int32_t* values, uint32_t count); //!< Upload an array of integers
	void uploadFloat(const char* name, float value); //!< Upload a float
	void uploadFloat2(const char* name, const glm::vec2& value); //!< Upload a vec2
	void uploadFloat3(const char* name, const glm::vec3& value); //!< Upload a vec3
	void uploadFloat4(const char* name, const glm::vec4& value); //!< Upload a vec4
	void uploadMat4(const char* name, const glm::mat4& value); //!< Upload a mat4
private:
	uint32_t m_OpenGL_ID = 0;
	void compileAndLink(const char* vertexShaderSrc, const char* fragmentShaderSrc);
};

