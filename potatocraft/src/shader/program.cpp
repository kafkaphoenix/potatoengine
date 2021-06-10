#include <vector>
#include <src/shader/program.hpp>

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
			link_status = false;
		}
		return;
	}
	link_status = true;
}

void Program::use() {
	glUseProgram(m_program);
}

void Program::setInt(const std::string &name, int value) {
	glUniform1i(glGetUniformLocation(m_program, name.c_str()), value); 
};

void Program::setFloat(const std::string &name, float value) {
	glUniform1f(glGetUniformLocation(m_program, name.c_str()), value); 
};

Program::Program() {
	m_program = glCreateProgram();
}

Program::~Program() {
	glDeleteProgram(m_program);
}