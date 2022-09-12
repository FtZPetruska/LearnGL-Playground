#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <filesystem>

class Shader
{
  public:
    Shader(const std::filesystem::path &vertex_path, const std::filesystem::path &fragment_path) noexcept;
    ~Shader() noexcept;

    Shader(Shader&& shader) noexcept;
    Shader& operator=(Shader&& shader) noexcept;

    /* Copying a shader program does not make sense. */
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    /* Put the shader program in use */
    void useProgram(void) const noexcept;

    /* Set a uniform in this shader program */
    void setUniform(const std::string &name, GLint value) const noexcept;
    void setUniform(const std::string &name, GLfloat value) const noexcept;
    void setUniform(const std::string &name, const glm::vec2 &value) const noexcept;
    void setUniform(const std::string &name, const glm::vec3 &value) const noexcept;
    void setUniform(const std::string &name, const glm::vec4 &value) const noexcept;
    void setUniform(const std::string &name, const glm::mat2 &value) const noexcept;
    void setUniform(const std::string &name, const glm::mat3 &value) const noexcept;
    void setUniform(const std::string &name, const glm::mat4 &value) const noexcept;

  private:
    GLuint shader_program;
};

#endif /* SHADER_H */
