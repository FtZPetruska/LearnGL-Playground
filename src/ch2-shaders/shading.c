#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "../shader/shader.h"
#include "../utils.h"
#include "shading_files.h"

static GLuint vao, vbo, ebo;
static Shader shader;

static Colour updateColour(void);
static void updateOffset(float increment);

void
setup(void)
{
    /** (x,y,z), (r, g, b)
     * A flat array of 3D points with their colour.
     */
    static const float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, /* Bottom left */
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  /* Bottom right */
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   /* Top */
    };

    /* Indices corresponding to the points defined in vertices. */
    static const GLuint indices[] = {
        0,
        1,
        2,
    };

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    shader = shader_make(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
}

void
processInputs(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        updateOffset(0.02f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        updateOffset(-0.02f);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        shader_setInt(shader, "flip", -1);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        shader_setInt(shader, "flip", 1);
    }
}

static void
updateOffset(float increment)
{
    static float current_offset = 0.0f;
    current_offset += increment;
    current_offset = clamp(current_offset, -1.0f, 1.0f);
    shader_setFloat(shader, "offset", current_offset);
}

void
render(GLFWwindow *window)
{
    /* Clear the screen with a specific colour */
    updateColour();
    glClear(GL_COLOR_BUFFER_BIT);

    /* Set the shader program and attributes (through vao) */
    shader_use(shader);
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
    shader_delete(shader);
}

static Colour
updateColour(void)
{
    static Colour colour = {1.0f, 0.0f, 0.0f, 1.0f};
    updateColours(&colour);
    glClearColor(colour.r, colour.g, colour.b, colour.a);
    return colour;
}
