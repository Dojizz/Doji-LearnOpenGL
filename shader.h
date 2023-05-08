#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

// ����ļ���û��������������Ƿ�ɹ��ļ��, ��Ҫ�����ټ�

class ShaderProgram {
private:
	unsigned int programId;
	std::string vertFileName;
	std::string fragFileName;
	unsigned int vertexShader;
	unsigned int fragmentShader;
	enum ShaderType {VERTEX, FRAGMENT};

	unsigned int CompileShader(std::string source, ShaderType type);
	bool CompileSuccess(unsigned int shader, ShaderType type);
	bool LinkSuccess(unsigned int program);

public:

	// ��ȡ��ɫ���ļ�������
	ShaderProgram(std::string vert = "defaultVertexShader.vert", std::string frag = "defaultFragmentShader.frag");

	// ���ǰ����
	void Use();

	// ��ȡID
	unsigned int GetVertShader();
	unsigned int GetFragShader();
	unsigned int GetProgram();

	// ����uniform
	void SetBool(const std::string& name, bool value);
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
	void SetMat4fv(const std::string& name, glm::mat4 trans);
	void SetVec3(const std::string& name, glm::vec3 v);
};

unsigned int ShaderProgram::CompileShader(std::string source, ShaderType type) {
	unsigned int shader = 0;
	std::ifstream shaderFile;
	std::string fileContents;

	// ��֤ifstream��������׳��쳣
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		shaderFile.open(source);
		std::stringstream shaderStream;
		// ��ȡ�ļ����ݵ�������
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		// ������->string
		fileContents = shaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* shaderSource = fileContents.c_str();

	// ��ʼ����
	if (type == VERTEX)
		shader = glCreateShader(GL_VERTEX_SHADER);
	else
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);

	// ��������
	if (CompileSuccess(shader, type))
		return shader;
	else
		return 0;
	
}

bool ShaderProgram::CompileSuccess(unsigned int shader, ShaderType type) {
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		if (type == VERTEX)
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		else
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
	return true;
}

bool ShaderProgram::LinkSuccess(unsigned int program) {
	int  success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return false;
	}
	return true;
}

ShaderProgram::ShaderProgram(std::string vert, std::string frag) {
	vertFileName = vert;
	fragFileName = frag;

	// compile 
	vertexShader = CompileShader(vert, VERTEX);
	fragmentShader = CompileShader(frag, FRAGMENT);

	// create program
	programId = glCreateProgram();
	glAttachShader(programId, vertexShader);
	glAttachShader(programId, fragmentShader);
	glLinkProgram(programId);
	LinkSuccess(programId);
}

void ShaderProgram::Use() {
	glUseProgram(programId);
}

unsigned int ShaderProgram::GetVertShader() {
	return vertexShader;
}

unsigned int ShaderProgram::GetFragShader() {
	return fragmentShader;
}

unsigned int ShaderProgram::GetProgram() {
	return programId;
}

void ShaderProgram::SetBool(const std::string& name, bool value) {
	glUniform1i(glGetUniformLocation(programId, name.c_str()), (int)value);
}
void ShaderProgram::SetInt(const std::string& name, int value) {
	glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}
void ShaderProgram::SetFloat(const std::string& name, float value) {
	glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

void ShaderProgram::SetVec3(const std::string& name, glm::vec3 v) {
	glUniform3f(glGetUniformLocation(programId, name.c_str()), v.x, v.y, v.z);
}

void ShaderProgram::SetMat4fv(const std::string& name, glm::mat4 trans) {
	unsigned int transformLoc = glGetUniformLocation(programId, name.c_str());
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
}











#endif // !SHADER_H

