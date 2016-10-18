#include "Shader.h"
#include <fstream>
#include <sstream>
using namespace std;

// Issue: longer shaders are so long they don't fit inside the console window.
//#define PRINT_SHADER_SRC_ON_ERROR

void loadShader(std::string filename, GLenum shaderType, vector<ShaderInfo> & shaders)
{
	ifstream file(filename);
	if(!file)
	{
		fprintf(stderr, "Unable to open file %s", filename.c_str());
		return;
	}
	
	stringstream buffer;
	buffer << file.rdbuf();
	
	ShaderInfo shaderInfo;
	shaderInfo.source = buffer.str();
	shaderInfo.shaderType = shaderType;
	shaderInfo.filename = filename;
	shaders.push_back(shaderInfo);
}

int compileShaderProgram(vector<ShaderInfo> & shaders, GLuint& programHandle)
{	
	GLuint pHandle = glCreateProgram();
	if( pHandle == 0 )
	{
		fprintf(stderr, "Error creating program object.\n");
		exit(1);
	}
	vector<const GLchar *> shaderSrcPtr;

	vector<string> lineNumbers(shaders.size());

	for (unsigned int i = 0; i < shaders.size(); i++)
	{		
		lineNumbers[i] += "\n#line 1 ";
		lineNumbers[i] += to_string(i);
		lineNumbers[i] += " \n";
	}
	bool useLineNumberDefine = shaders[0].shaderType == SHADER_VERSION_HEADER;

	for(unsigned int i = 0; i < shaders.size(); i++)
	{		
		if(shaders[i].shaderType != SHADER_VERSION_HEADER && useLineNumberDefine)
		{
			shaderSrcPtr.push_back(lineNumbers[i].c_str());
		}

		shaderSrcPtr.push_back(shaders[i].source.c_str());

		// If it's a shader header, it contains code that should 
		// be added to the next shader that isn't a shader header
		if (shaders[i].shaderType == SHADER_HEADER || shaders[i].shaderType == SHADER_VERSION_HEADER) {
			continue;
		}

		GLuint shaderHandle = glCreateShader(shaders[i].shaderType);
		shaders[i].shaderHandle = shaderHandle;		
		glShaderSource(shaderHandle, shaderSrcPtr.size(), shaderSrcPtr.data(), 0);	
		glCompileShader(shaderHandle);

		GLint result;
		glGetShaderiv( shaderHandle, GL_COMPILE_STATUS, &result );
		if( result == GL_FALSE )
		{
			fprintf( stderr, "Shader compilation failed!\n");
			for (int filenr = 0; filenr <= i; filenr++)
			{
				fprintf(stderr, "File %i: %s\n", filenr, shaders[filenr].filename.c_str());
			}			

#ifdef PRINT_SHADER_SRC_ON_ERROR
			string s;
			unsigned int totalLen = 0;
			for (unsigned int j = 0; j < shaderSrcPtr.size(); j++)
			{
				totalLen += strlen(shaderSrcPtr[j]);
			}

			s.resize(totalLen + 1);
			GLsizei length;
			glGetShaderSource(shaderHandle, s.size(), &length, (GLchar *)s.c_str());
			fprintf(stderr, "%s\n", s.c_str());
			if (s != shaders[i].source) {
				fprintf(stderr, "Warning: Original shader source code and source retrieved from the driver differs.");				
			}
#endif
			GLint logLen;
			glGetShaderiv( shaderHandle, GL_INFO_LOG_LENGTH, &logLen );
			if( logLen > 0 )
			{
				char * log = (char *)malloc(logLen);
				GLsizei written;
				glGetShaderInfoLog(shaderHandle, logLen, &written, log);
				fprintf(stderr, "Shader log:\n%s", log);
				free(log);				
			}

			// Incase the filename contains ../ or ./, skip it. It causes the errdump to be written.
			std::string filename; 
			for (int j = 0; j < shaders[i].filename.size(); j++)
			{
				char c = shaders[i].filename[j];				
				if (c != '.' && c != '/')
				{
					filename += c;
				}
				if (c == '.' && shaders[i].filename[j + 1] == 'g') // Don't remove the .glsl ending.
				{
					filename += c;
				}
			}
			
			string errFileName = "errdump_" + filename;
			ofstream f(errFileName.c_str());
			if(f.is_open())
			{
				for (auto j = 0; j < shaderSrcPtr.size(); j++)
				{
					f << shaderSrcPtr[j];
				}
				f.close();
				fprintf(stderr, "Shader written to %s\n", errFileName.c_str());
			} 
			else
			{
				std::cerr << "Unable to write " << errFileName << std::endl;				
			}
			
			return -1;
		}

		glAttachShader(pHandle, shaderHandle);
		shaderSrcPtr.clear();
	}

	glLinkProgram( pHandle);
	GLint status;
	glGetProgramiv( pHandle, GL_LINK_STATUS, &status );
	if( status == GL_FALSE ) 
	{
		fprintf( stderr, "Failed to link shader program!\n" );
		for (unsigned int i = 0; i < shaders.size(); i++)
		{
			fprintf(stderr, "%s\n", shaders[i].filename.c_str());
		}		
		GLint logLen;
		glGetProgramiv(pHandle, GL_INFO_LOG_LENGTH, &logLen);
		if( logLen > 0 )
		{
			char * log = (char *)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(pHandle, logLen, &written, log);
			fprintf(stderr, "Program log: \n%s", log);
			free(log);			
		}
		return -2;
	}
	else
	{
		programHandle = pHandle;
		return 0;
	}
}