#pragma once
#include "OpenGL.h"
#include <iostream>
#include <vector>

// Should be created by loadShader
struct ShaderInfo
{
	std::string filename;
	std::string source;
	GLenum shaderType;
	GLuint shaderHandle;
};

int compileShaderProgram(std::vector<ShaderInfo> & shaders, GLuint& programHandle);
void loadShader(std::string filename, GLenum shaderType, std::vector<ShaderInfo> & shaders);

#define SHADER_HEADER 0x0
#define SHADER_VERSION_HEADER 0x1