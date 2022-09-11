#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

#define INFO_LOG_SIZE 512

struct shader_t
{
    GLuint program;
};

typedef enum
{
    VERTEX,
    FRAGMENT
} shader_type;

static GLchar *readFile(const char *path);
static long getFileSize(FILE *file_handle);
static GLuint compileShader(const GLchar *shader_source, shader_type type);
static GLuint makeShaderProgram(GLuint vertex_shader, GLuint fragment_shader);
static void cleanup(GLchar *vertex_code, GLchar *fragment_code, GLuint vexter_shader, GLuint fragment_shader);

Shader
shader_make(const char *vertex_path, const char *fragment_path)
{
    GLchar *vertex_code, *fragment_code;
    GLuint vertex_shader, fragment_shader;
    Shader shader = calloc(1, sizeof(struct shader_t));
    if (NULL == shader) {
        fprintf(stderr, "shader_make: %s\n", "Not enough memory to allocate the shader.");
        return NULL;
    }

    vertex_code = readFile(vertex_path);
    if (NULL == vertex_code) {
        free(shader);
        return NULL;
    }

    fragment_code = readFile(fragment_path);
    if (NULL == fragment_code) {
        free(vertex_code);
        free(shader);
        return NULL;
    }

    vertex_shader = compileShader(vertex_code, VERTEX);
    fragment_shader = compileShader(fragment_code, FRAGMENT);

    shader->program = makeShaderProgram(vertex_shader, fragment_shader);

    cleanup(vertex_code, fragment_code, vertex_shader, fragment_shader);
    return shader;
}

static GLchar *
readFile(const char *path)
{
    long size;
    size_t read;
    GLchar *content = NULL;
    FILE *file_handle = fopen(path, "r");

    if (NULL == file_handle) {
        fprintf(stderr, "readFile: Unable to open %s\n", path);
        return NULL;
    }

    size = getFileSize(file_handle);
    if (-1L == size) {
        fprintf(stderr, "readFile: Unable to get the size of %s\n", path);
        return NULL;
    }

    /* Account for a NULL terminator */
    content = calloc((unsigned long) size + 1, sizeof(GLchar));
    if (NULL == content) {
        fprintf(stderr, "readFile: Unable to allocate buffer for %s\n", path);
        return NULL;
    }

    read = fread(content, sizeof(GLchar), (unsigned long) size, file_handle);
    if (read != (unsigned long) size) {
        fprintf(stderr, "readFile: Failed to read %s in its entirety\n", path);
        free(content);
        return NULL;
    }

    return content;
}

/* Get the size of the file, returns -1 in case of error. */
static long
getFileSize(FILE *file_handle)
{
    long size = 0;
    if (0 != fseek(file_handle, 0, SEEK_END)) {
        fprintf(stderr, "getFileSize: %s\n", "Unable to go to the end of the file");
        return -1L;
    }

    size = ftell(file_handle);
    if (-1L == size) {
        fprintf(stderr, "getFileSize: %s\n", "Unable to read the file position");
        return -1L;
    }

    if (0 != fseek(file_handle, 0, SEEK_SET)) {
        fprintf(stderr, "getFileSize: %s\n", "Unable to go to the beginning of the file");
        return -1L;
    }

    return size;
}

static GLuint
compileShader(const GLchar *shader_source, shader_type type)
{
    GLuint shader;
    GLint success;
    GLchar info_log[INFO_LOG_SIZE];

    shader = glCreateShader(VERTEX == type ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, INFO_LOG_SIZE, NULL, info_log);
        fprintf(stderr, "compilerShader: %s\n", info_log);
    };

    return shader;
}

static GLuint
makeShaderProgram(GLuint vertex_shader, GLuint fragment_shader)
{
    GLchar info_log[INFO_LOG_SIZE];
    GLint success;

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, info_log);
        fprintf(stderr, "makeShaderProgram: %s\n", info_log);
    }

    return program;
}

/* Cleans up the buffers and shaders */
static void
cleanup(GLchar *vertex_code, GLchar *fragment_code, GLuint vexter_shader, GLuint fragment_shader)
{
    if (vertex_code) {
        free(vertex_code);
    }
    if (fragment_code) {
        free(fragment_code);
    }
    glDeleteShader(vexter_shader);
    glDeleteShader(fragment_shader);
}

void
shader_delete(Shader shader)
{
    glDeleteProgram(shader->program);
}

void
shader_use(const Shader shader)
{
    glUseProgram(shader->program);
}

void
shader_setBool(const Shader shader, const GLchar *name, GLboolean value)
{
    shader_setInt(shader, name, (int) value);
}

void
shader_setInt(const Shader shader, const GLchar *name, GLint value)
{
    GLint location = glGetUniformLocation(shader->program, name);
    glUniform1i(location, value);
}

void
shader_setFloat(const Shader shader, const GLchar *name, GLfloat value)
{
    GLint location = glGetUniformLocation(shader->program, name);
    glUniform1f(location, value);
}
