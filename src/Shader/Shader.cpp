#include "Shader.hpp"

#include <fmt/core.h>
#include <fstream>
#include <vector>

enum class ShaderLogType
{
    SHADER,
    PROGRAM
};

static std::string readFile(const std::filesystem::path &file_path);
static GLuint compileShader(const GLchar *shader_content, GLenum type);
static void checkAndLogShaderError(GLuint shader, ShaderLogType type);
static GLuint linkShadersIntoProgram(const std::vector<GLuint> &shaders);
static void freeShaders(const std::vector<GLuint> &shaders);

static constexpr auto
getIvFuncFromType(ShaderLogType type)
{
    switch (type) {
    case (ShaderLogType::SHADER):
        return glGetShaderiv;
    case (ShaderLogType::PROGRAM):
        return glGetProgramiv;
    }
}

static constexpr GLenum
getIvStatusFromType(ShaderLogType type)
{
    switch (type) {
    case (ShaderLogType::SHADER):
        return GL_COMPILE_STATUS;
    case (ShaderLogType::PROGRAM):
        return GL_LINK_STATUS;
    }
}

static constexpr auto
getInfoLogFuncFromType(ShaderLogType type)
{
    switch (type) {
    case (ShaderLogType::SHADER):
        return glGetShaderInfoLog;
    case (ShaderLogType::PROGRAM):
        return glGetProgramInfoLog;
    }
}

Shader::Shader(const std::filesystem::path &vertex_path, const std::filesystem::path &fragment_path)
{
    const std::string vertex_content = readFile(vertex_path);
    const std::string fragment_content = readFile(fragment_path);

    GLuint vertex_shader = compileShader(vertex_content.c_str(), GL_VERTEX_SHADER);
    GLuint fragment_shader = compileShader(fragment_content.c_str(), GL_FRAGMENT_SHADER);

    shader_program = linkShadersIntoProgram({vertex_shader, fragment_shader});

    freeShaders({vertex_shader, fragment_shader});
}

static std::string
readFile(const std::filesystem::path &file_path)
{
    static constexpr char DELIMITER {EOF};
    std::ifstream file_stream{file_path, std::ios::in};
    std::string file_content{};

    if (file_stream.is_open()) {
        std::getline(file_stream, file_content, DELIMITER);
        file_stream.close();
    } else {
        fmt::print(stderr, "readFile: Failed to open file {}.\n", file_path.string());
    }
    return file_content;
}

static GLuint
compileShader(const GLchar *shader_content, GLenum shader_type)
{
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_content, nullptr);
    glCompileShader(shader);
    checkAndLogShaderError(shader, ShaderLogType::SHADER);
    return shader;
}

static void
checkAndLogShaderError(GLuint shader, ShaderLogType type)
{
    static constexpr std::size_t INFO_LOG_SIZE{1024};
    GLchar info_log[INFO_LOG_SIZE];
    GLint success;

    auto iv_func = getIvFuncFromType(type);
    GLenum iv_status = getIvStatusFromType(type);
    auto info_log_func = getInfoLogFuncFromType(type);

    iv_func(shader, iv_status, &success);
    if (!success) {
        info_log_func(shader, INFO_LOG_SIZE, nullptr, info_log);
        fmt::print(stderr, "{}\n", info_log);
    }
}

static GLuint
linkShadersIntoProgram(const std::vector<GLuint> &shaders)
{
    GLuint program = glCreateProgram();
    for (GLuint shader : shaders) {
        glAttachShader(program, shader);
    }
    glLinkProgram(program);
    checkAndLogShaderError(program, ShaderLogType::PROGRAM);
    return program;
}

static void
freeShaders(const std::vector<GLuint> &shaders)
{
    for (GLuint shader : shaders) {
        glDeleteShader(shader);
    }
}

void
Shader::useProgram(void) const
{
    glUseProgram(shader_program);
}

void
Shader::setUniform(const std::string &name, GLint value) const
{
    GLint location = glGetUniformLocation(shader_program, name.c_str());
    glUniform1i(location, value);
}

void
Shader::setUniform(const std::string &name, GLfloat value) const
{
    GLint location = glGetUniformLocation(shader_program, name.c_str());
    glUniform1f(location, value);
}

void
Shader::setUniform(const std::string &name, const glm::vec2 &value) const
{
    GLint location = glGetUniformLocation(shader_program, name.c_str());
    glUniform2f(location, value.x, value.y);
}

void
Shader::setUniform(const std::string &name, const glm::vec3 &value) const
{
    GLint location = glGetUniformLocation(shader_program, name.c_str());
    glUniform3f(location, value.x, value.y, value.z);
}

void
Shader::setUniform(const std::string &name, const glm::vec4 &value) const
{
    GLint location = glGetUniformLocation(shader_program, name.c_str());
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void
Shader::setUniform(const std::string &name, const glm::mat2 &value) const
{
    GLint location = glGetUniformLocation(shader_program, name.c_str());
    glUniformMatrix2fv(location, 1, GL_FALSE, &value[0][0]);
}

void
Shader::setUniform(const std::string &name, const glm::mat3 &value) const
{
    GLint location = glGetUniformLocation(shader_program, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
}

void
Shader::setUniform(const std::string &name, const glm::mat4 &value) const
{
    GLint location = glGetUniformLocation(shader_program, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}
