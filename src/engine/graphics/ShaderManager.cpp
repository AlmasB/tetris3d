#include "ShaderManager.h"

std::shared_ptr<ShaderManager> ShaderManager:: instance = std::shared_ptr<ShaderManager>(new ShaderManager());

std::shared_ptr<ShaderManager> ShaderManager::getInstance() {
	return instance;
}

GLuint ShaderManager::getCurrentProgram() {
	return shaderPrograms.begin()->second;
}

ShaderManager::ShaderManager() {}

GLuint ShaderManager::createShader(const char * shaderCode, GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType);
	if (shader == 0) {
		// exit ?
	}

	const GLchar* strings[] = { shaderCode };	// shader code strings
	GLint lengths[] = { (GLint)strlen(shaderCode) };	// shader code string length

	glShaderSource(shader, 1, strings, lengths);
	glCompileShader(shader);

	GLint ok;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
	if (!ok) {
		GLchar infoLog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cout << "Failed to compile shader: " << infoLog << std::endl;
		getchar();
		// exit ?
	}

	return shader;
}

GLuint ShaderManager::createProgram(const char * vertexShaderCode, const char * fragmentShaderCode) {
	std::string key = std::string(vertexShaderCode) + std::string(fragmentShaderCode);
	if (shaderPrograms.count(key) > 0)
		return shaderPrograms[key];

	GLuint program = glCreateProgram();
	if (0 == program) {
		// exit?
	}

	glAttachShader(program, createShader(vertexShaderCode, GL_VERTEX_SHADER));
	glAttachShader(program, createShader(fragmentShaderCode, GL_FRAGMENT_SHADER));
	glLinkProgram(program);

	GLint ok;
	glGetProgramiv(program, GL_LINK_STATUS, &ok);
	GLchar infoLog[1024];
	if (!ok) {
		glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
		std::cout << "Failed to link program: " << infoLog << std::endl;
		getchar();
		// exit ?
	}

	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &ok);
	if (!ok) {
		glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
		std::cout << "Failed to validate program: " << infoLog << std::endl;
		getchar();
		// exit ?
	}

	shaderPrograms[key] = program;
	return program;
}
