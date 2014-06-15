/**
 * {XCube3D Game Engine}
 * Copyright(C) { 2013-2014 }  {AlmasB} {a.baimagambetov1@uni.brighton.ac.uk}
 *
 * This program is free software; you can redistribute it and / or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110 - 1301 USA.
 */

#ifndef __SHADER_MANAGER_H__
#define __SHADER_MANAGER_H__

#include <map>
#include <memory>
#include <string.h> // strlen()

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
