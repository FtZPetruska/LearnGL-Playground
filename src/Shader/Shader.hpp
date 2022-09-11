#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <filesystem>

class Shader
{
  public:
    Shader(const std::filesystem::path &vertex_path, const std::filesystem::path &fragment_path);

    /* Put the shader program in use */
    void useProgram(void) const;

    /* Set a uniform in this shader program */
    void setUniform(const std::string &name, GLint value) const;
    void setUniform(const std::string &name, GLfloat value) const;
    void setUniform(const std::string &name, const glm::vec2 &value) const;
    void setUniform(const std::string &name, const glm::vec3 &value) const;
    void setUniform(const std::string &name, const glm::vec4 &value) const;
    void setUniform(const std::string &name, const glm::mat2 &value) const;
    void setUniform(const std::string &name, const glm::mat3 &value) const;
    void setUniform(const std::string &name, const glm::mat4 &value) const;

  private:
    GLuint shader_program;
};

#endif /* SHADER_H */
