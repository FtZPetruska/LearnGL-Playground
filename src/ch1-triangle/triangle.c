#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <stdio.h>

#include "../utils.h"

#define ERROR_BUFSIZE 256

static GLuint vao, vbo, ebo, shader_program;

static void updateClearColour(void);
static void coompileShader(GLuint shader, const GLchar *shader_source);
static void coompileVertexShader(GLuint vertex_shader);
static void coompileFragmentShader(GLuint fragment_shader);

void
setup(void)
{
    GLuint vertex_shader, fragment_shader;
    GLint success;

    /** (x,y,z)
     * A flat array of 3D points.
     */
    static const float vertices[] = {
        /* Triangle */
        -0.8f, 0.8f, 0.0f,
        -0.6f, 0.8f, 0.0f,
        -0.7f, 0.6f, 0.0f,

        /* Rectangle */
        0.5f, 0.5f, 0.0f,   /* top right */
        0.5f, -0.5f, 0.0f,  /* bottom right */
        -0.5f, -0.5f, 0.0f, /* bottom left */
        -0.5f, 0.5f, 0.0f   /* top left */
    };

    /* Indices corresponding to the points defined in vertices. */
    static const GLuint indices[] = {
        /* Triangle */
        0, 1, 2,

        /* Rectangle (two triangles) */
        3, 4, 6,
        4, 5, 6};

    /* Create and binds a vertex array to store attribute */
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    /* Cteate an element buffer to store indices of vertices */
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    /* Copies the indices into the ELEMENT_ARRAY_BUFFER target */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

    /* Create a vertex buffer, and bind it to the ARRAY_BUFFER target */
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    /* Copies the vertices into the ARRAY_BUFFER target */
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

    /** We tell GL how to interpret the vertex data:
     * - The first param is the (location = x) of the vertex shader
     * - The second param is the size of the vertex (how many values)
     * - The third param is the type of the values
     * - The fourth param allows us to normalise int values to floats
     * - The fifth param is the stride, how many bytes is a vertex
     * - The last param is the offset of the data in the array
     */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    /* Create a vertex shader and compile a source */
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    coompileVertexShader(vertex_shader);

    /* Create a fragment shader and compile a source */
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    coompileFragmentShader(fragment_shader);

    /* Create a shader program and attach the shaders to it */
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        char error_buffer[ERROR_BUFSIZE];
        glGetProgramInfoLog(shader_program, ERROR_BUFSIZE, NULL, error_buffer);
        fprintf(stderr, "Failed shader compilation: %s\n", error_buffer);
    }

    /* Once the shader program is in use, the shaders can be freed */
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void
processInputs(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void
render(GLFWwindow *window)
{
    /* Clear the screen with a specific colour */
    updateClearColour();
    glClear(GL_COLOR_BUFFER_BIT);

    /* Set the shader program and attributes (through vao) */
    glUseProgram(shader_program);
    glBindVertexArray(vao);

    /* Bind the element buffer and draw it */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, NULL);

    /* Keep last */
    glfwSwapBuffers(window);
}

void
teardown(void)
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shader_program);
}

static void
updateClearColour(void)
{
    static Colour colour = {1.0f, 0.0f, 0.0f, 1.0f};
    updateColours(&colour);
    glClearColor(colour.r, colour.g, colour.b, colour.a);
}

static void
coompileShader(GLuint shader, const GLchar *shader_source)
{
    GLint success;

    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char error_buffer[ERROR_BUFSIZE];
        glGetShaderInfoLog(shader, ERROR_BUFSIZE, NULL, error_buffer);
        fprintf(stderr, "Failed shader compilation: %s\n", error_buffer);
    }
}

static void
coompileVertexShader(GLuint vertex_shader)
{
    static const GLchar *vertex_shader_source =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    coompileShader(vertex_shader, vertex_shader_source);
}

static void
coompileFragmentShader(GLuint fragment_shader)
{
    static const GLchar *fragment_shader_source =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";
    coompileShader(fragment_shader, fragment_shader_source);
}
