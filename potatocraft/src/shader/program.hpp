#ifndef PROGRAM_H
#define PROGRAM_H

#include <src/shader/shader.hpp>

class Program
{
private:
    GLuint m_program;

public:
    operator GLuint();
    void attach(Shader &shader);
    void detach(Shader &shader);
    void link(bool& status);
    void use();
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);

    Program();
    ~Program();
};

#endif PROGRAM_H