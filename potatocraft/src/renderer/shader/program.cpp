#include <vector>

#include <src/renderer/shader/program.hpp>

Program::operator GLuint() {
	return m_program;
}

void Program::attach(Shader &shader) {
	glAttachShader(m_program, shader);
}

void Program::detach(Shader &shader) {
	glDetachShader(m_program, shader);
}

void Program::link(bool& link_status) {
	glLinkProgram(m_program);
	GLint status = GL_FALSE;
	glGetProgramiv(m_program, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
        GLint maxLength = 0;
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength);
		fprintf(stdout, "Shader linking failed.\n");
		if (maxLength > 0) {
			std::vector<GLchar> shaderProgramInfoLog(maxLength);
			glGetProgramInfoLog(m_program, maxLength, &maxLength, &shaderProgramInfoLog[0]);
			fprintf(stdout, "\tError info: %s\n", shaderProgramInfoLog.data());
		}
		link_status = false;
	} else {
		link_status = true;
	}
}

void Program::use() {
	glUseProgram(m_program);
}

void Program::setInt(const std::string &name, int value) {
	glUniform1i(glGetUniformLocation(m_program, name.c_str()), value); 
}

void Program::setFloat(const std::string &name, float value) {
	glUniform1f(glGetUniformLocation(m_program, name.c_str()), value); 
}

void Program::setVec2(const std::string &name, const glm::vec2 &vec) {
    glUniform2f(glGetUniformLocation(m_program, name.c_str()), vec.x, vec.y);
}

void Program::setVec3(const std::string &name, const glm::vec3 &vec) {
    glUniform3f(glGetUniformLocation(m_program, name.c_str()), vec.x, vec.y, vec.z);
}

void Program::setVec4(const std::string &name, const glm::vec4 &vec) {
    glUniform4f(glGetUniformLocation(m_program, name.c_str()), vec.x, vec.y, vec.z, vec.w);
}

void Program::setMat4(const std::string &name, const glm::mat4 &mat) {
    glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

const std::string& Program::getName() const noexcept {
	return m_name;
}

Program::Program(const std::string &name) {
	m_program = glCreateProgram();
	m_name = name;
}

Program::~Program() {
	glDeleteProgram(m_program);
}