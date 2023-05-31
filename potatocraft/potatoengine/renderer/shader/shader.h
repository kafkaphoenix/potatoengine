#pragma once

#include <glad/glad.h>

namespace potatoengine {

class Shader {
   public:
    Shader();
    ~Shader();

    operator GLuint() const;
    operator bool() const;
    void create(GLenum type);
    void load_file(GLenum type, const std::string &file, bool &status);
    bool load_src(GLenum type, const std::string &src);
    void set_src(const std::string &src);
    void set_file(const std::string &src);
    void compile();

   private:
    int32_t m_id;
    GLenum m_type;
};

}