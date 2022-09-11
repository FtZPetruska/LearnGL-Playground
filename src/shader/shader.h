
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

typedef struct shader_t *Shader;

/* Create a new shader from the two files */
Shader shader_make(const char *vertex_path, const char *fragment_path);

/* Delete a shader clean up the shader */
void shader_delete(Shader shader);

/* Use the shader */
void shader_use(const Shader shader);

/* Set a boolean uniform */
void shader_setBool(const Shader shader, const GLchar *name, GLboolean value);

/* Set an int uniform */
void shader_setInt(const Shader shader, const GLchar *name, GLint value);

/* Set a float uniform */
void shader_setFloat(const Shader shader, const GLchar *name, GLfloat value);

#endif
