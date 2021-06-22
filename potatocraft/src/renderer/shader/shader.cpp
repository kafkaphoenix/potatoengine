#include "src/pcpch.hpp"
#include "src/renderer/shader/shader.hpp"

namespace potatocraft {

	bool readfile(const char* filename, std::string &contents) {
		std::ifstream f(filename);
		if(!f.good()) {
			fprintf(stdout, "Failed to load file.\n");
			return false;
		}
		char c;
		while((c = f.get()), f.good()) {
			contents+=c;
		}
		f.close();
		return true;
	}

	void Shader::create(GLenum type) {
		m_type = type;
		m_shader = glCreateShader(type);
	}

	bool Shader::load_src(GLenum type, const std::string& src) {
		this->create(type);
		this->set_src(src);
		this->compile();
		GLint status = GL_FALSE;
		glGetShaderiv(m_shader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			GLint maxLength = 0;
			glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &maxLength);
			fprintf(stdout, "Shader compilation failed.\n");
			if (maxLength > 0) {
				std::vector<GLchar> shaderInfoLog(maxLength);
				glGetShaderInfoLog(m_shader, maxLength, &maxLength, &shaderInfoLog[0]);
				fprintf(stdout, "\tError info: %s\n", shaderInfoLog.data());
			}
			return false;
		}
		return true;
	}

	void Shader::load_file(GLenum type, const std::string& file, bool& status) {
		std::string src;
		if(readfile(file.c_str(), src)) {
			status = this->load_src(type, src);
		}
	}

	void Shader::set_src(const std::string& src) {
		const GLchar *src_cstr = (const GLchar *)src.c_str();
		glShaderSource(m_shader, 1, &src_cstr, 0);
	}

	void Shader::set_file(const std::string& file) {
		std::string src;
		readfile(file.c_str(), src);
		this->set_src(src);
	}

	void Shader::compile() {
		glCompileShader(m_shader);
	}

	Shader::operator GLuint() {
		return m_shader;
	}

	Shader::operator bool() {
		return m_shader != static_cast<GLuint>(-1);
	}

	Shader::Shader() {
		m_shader = -1;
	}

	Shader::~Shader() {
		if(*this)
			glDeleteShader(m_shader);
	}

}