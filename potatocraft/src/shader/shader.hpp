#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>

class Shader
{
public:
    Shader();
    Shader(GLenum type);
    ~Shader();

    operator GLuint();
    operator bool();
    void create(GLenum type);
	void load_file(GLenum type, const std::string& file, bool& status);
	bool load_src(GLenum type, const std::string& src);
	void set_src(const std::string& src);
	void set_file(const std::string& src);
	void compile();

private:
    GLuint m_shader;
	GLuint m_type;
};

#endif SHADER_HEADER