#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>

class Shader
{
private:
    GLuint m_shader;
	GLuint m_type;

public:
    operator GLuint();
    operator bool();
    void create(GLenum type);
	void load_file(GLenum type, const std::string& file, bool& status);
	bool load_src(GLenum type, const std::string& src);
	void set_src(const std::string& src);
	void set_file(const std::string& src);
	void compile();

    Shader();
    Shader(GLenum type);
    ~Shader();
};

#endif SHADER_HEADER