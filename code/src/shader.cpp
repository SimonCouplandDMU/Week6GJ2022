/*\file shader.cpp*/

#include <glad/glad.h>
#include <fstream>
#include <string>
#include <array>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "shader.h"



Shader::Shader(const char* vertexFilepath, const char* fragmentFilepath)
{
	std::string line, vertexSrc, fragmentSrc;

	std::fstream handle(vertexFilepath, std::ios::in);
	if (handle.is_open())
	{
		while (getline(handle, line)) { vertexSrc += (line + "\n"); }
	}
	else
	{
		std::cout << "Could not open shader vertex source: " << vertexFilepath;
		return;
	}
	handle.close();

	handle.open(fragmentFilepath, std::ios::in);
	if (handle.is_open())
	{
		while (getline(handle, line)) { fragmentSrc += (line + "\n"); }
	}
	else
	{
		std::cout << "Could not open shader fragment source: " << fragmentFilepath;
		return;
	}
	handle.close();

	compileAndLink(vertexSrc.c_str(), fragmentSrc.c_str()); // Uses code from application.cpp
}


Shader::~Shader()
{
	glDeleteProgram(m_OpenGL_ID);
}

void Shader::use()
{
	glUseProgram(m_OpenGL_ID);
}

void Shader::stopUsing()
{
	glUseProgram(0);
}

void Shader::uploadInt(const char* name, int value)
{
	uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, name);
	glUniform1i(uniformLocation, value);
}

void Shader::uploadIntArray(const char* name, int32_t* values, uint32_t count)
{
	uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, name);
	glUniform1iv(uniformLocation, count, values);
}

void Shader::uploadFloat(const char* name, float value)
{
	uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, name);
	glUniform1f(uniformLocation, value);
}

void Shader::uploadFloat2(const char* name, const glm::vec2& value)
{
	uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, name);
	glUniform2f(uniformLocation, value.x, value.y);
}

void Shader::uploadFloat3(const char* name, const glm::vec3& value)
{
	uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, name);
	glUniform3f(uniformLocation, value.x, value.y, value.z);
}

void Shader::uploadFloat4(const char* name, const glm::vec4& value)
{
	uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, name);
	glUniform4f(uniformLocation, value.x, value.y, value.z, value.w);
}

void Shader::uploadMat4(const char* name, const glm::mat4& value)
{
	uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, name);
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::compileAndLink(const char* vertexShaderSrc, const char* fragmentShaderSrc)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* source = vertexShaderSrc;
	glShaderSource(vertexShader, 1, &source, 0);
	glCompileShader(vertexShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
		std::cout << "Shader compile error: " << std::string(infoLog.begin(), infoLog.end());

		glDeleteShader(vertexShader);
		return;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	source = fragmentShaderSrc;
	glShaderSource(fragmentShader, 1, &source, 0);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
		std::cout << "Shader compile error: " << std::string(infoLog.begin(), infoLog.end());

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);

		return;
	}

	m_OpenGL_ID = glCreateProgram();
	glAttachShader(m_OpenGL_ID, vertexShader);
	glAttachShader(m_OpenGL_ID, fragmentShader);
	glLinkProgram(m_OpenGL_ID);

	GLint isLinked = 0;
	glGetProgramiv(m_OpenGL_ID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_OpenGL_ID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_OpenGL_ID, maxLength, &maxLength, &infoLog[0]);
		std::cout << "Shader linking error: " << std::string(infoLog.begin(), infoLog.end());

		glDeleteProgram(m_OpenGL_ID);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return;
	}

	glDetachShader(m_OpenGL_ID, vertexShader);
	glDetachShader(m_OpenGL_ID, fragmentShader);
}
