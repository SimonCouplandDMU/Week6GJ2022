/** \file shaderDataType.h */
#pragma once

#include<inttypes.h>

enum class ShaderDataType
{
	None = 0, Float, Float2, Float3, Float4, Int, Mat4, FlatInt, FlatByte, Byte4
};

namespace SDT
{
	static uint32_t size(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Byte4: return 1 * 4;
		case ShaderDataType::FlatByte: return 1;
		case ShaderDataType::FlatInt: return 4;
		case ShaderDataType::Int: return 4;
		case ShaderDataType::Float: return 4;
		case ShaderDataType::Float2: return 4 * 2;
		case ShaderDataType::Float3: return 4 * 3;
		case ShaderDataType::Float4: return 4 * 4;
		case ShaderDataType::Mat4: return 4 * 4 * 4;
		default: return 0;
		}
	}

	static uint32_t componentCount(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Byte4: return 4;
		case ShaderDataType::FlatByte: return 1;
		case ShaderDataType::FlatInt: return 1;
		case ShaderDataType::Int: return 1;
		case ShaderDataType::Float: return 1;
		case ShaderDataType::Float2: return 2;
		case ShaderDataType::Float3: return 3;
		case ShaderDataType::Float4: return 4;
		case ShaderDataType::Mat4: return 4;
		default: return 0;
		}
	}

	static uint32_t STD140Alignment(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Byte4:	  return 4;
		case ShaderDataType::FlatByte:return 4; //?
		case ShaderDataType::FlatInt: return 4;
		case ShaderDataType::Int:     return 4;
		case ShaderDataType::Float:   return 4;
		case ShaderDataType::Float2:  return 8;
		case ShaderDataType::Float3:  return 16;
		case ShaderDataType::Float4:  return 16;
		case ShaderDataType::Mat4:    return 16 * 4;
		}
		return 0;
	}
}

