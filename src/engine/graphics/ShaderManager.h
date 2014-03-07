#ifndef __SHADER_MANAGER_H__
#define __SHADER_MANAGER_H__

#include <map>
#include <memory>

#include <GL/glew.h>

#include "../core/Debug.h"

class ShaderManager {
	private:
		std::map<std::string, GLuint> shaderPrograms;
		static std::shared_ptr<ShaderManager> instance;
		ShaderManager();
	public:
		static std::shared_ptr<ShaderManager> getInstance();
		GLuint createShader(const char * shaderCode, GLenum shaderType);
		GLuint createProgram(const char * vertexShaderCode, const char * fragmentShaderCode);
		GLuint getCurrentProgram();	// TODO: do differently
};

#endif