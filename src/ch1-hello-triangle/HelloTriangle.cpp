#include "../BaseApplication.hpp"

#include "HelloTriangleFiles.hpp"
#include "Shader.hpp"

#include <memory>

class HelloTriange : public BaseApplication
{
  public:
    virtual ~HelloTriange() = default;

  private:
    void
    setup(void) override
    {
        static constexpr GLfloat horizontal_vertices[] = {
            0.0f, 0.0f, 0.0f,   /* Center */
            -0.6f, 0.5f, 0.0f,  /* Left-Up */
            -0.6f, -0.5f, 0.0f, /* Left-Down */
            0.6f, 0.5f, 0.0f,   /* Right-Up */
            0.6f, -0.5f, 0.0f,  /* Right-Down */
        };

        static constexpr GLfloat vertical_vertices[] = {
            0.0f, 0.0f, 0.0f,   /* Center */
            -0.5f, 0.6f, 0.0f,  /* Top-left */
            0.5f, 0.6f, 0.0f,   /* Top-right */
            -0.5f, -0.6f, 0.0f, /* Bottom-Left */
            0.5f, -0.6f, 0.0f,  /* Bottom-Right */
        };

        static constexpr GLuint horizontal_indices[] = {
            0, 1, 2, /* Left triangle */
            0, 3, 4, /* Top triange */
        };

        static constexpr GLuint vertical_indices[] = {
            0, 1, 2, /* Top triangle */
            0, 3, 4, /* Bottom triangle */
        };

        /* Generate buffers */
        glGenVertexArrays(2, vaos);
        glGenBuffers(2, vbos);
        glGenBuffers(2, ebos);

        /* Bind the VAO first so the latter commands are tied to it */
        glBindVertexArray(vaos[0]);

        /* Copy the vertices into the VBO */
        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof horizontal_vertices, horizontal_vertices, GL_STATIC_DRAW);

        /* Copie the indices into the EBO */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof horizontal_indices, horizontal_indices, GL_STATIC_DRAW);

        /* Set vertices attributes */
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0);

        /* Bind the VAO first so the latter commands are tied to it */
        glBindVertexArray(vaos[1]);

        /* Copy the vertices into the VBO */
        glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof vertical_vertices, vertical_vertices, GL_STATIC_DRAW);

        /* Copie the indices into the EBO */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof vertical_indices, vertical_indices, GL_STATIC_DRAW);

        /* Set vertices attributes */
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0);

        horizontal_shader = std::make_unique<Shader>(VERTEX_SHADER_FILE, HORIZONTAL_FRAGMENT_SHADER_FILE);
        vertical_shader = std::make_unique<Shader>(VERTEX_SHADER_FILE, VERTICAL_FRAGMENT_SHADER_FILE);
    }

    void
    processInputs(void) override
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    void
    render(void) override
    {
        glClearColor(0.9f, 0.0f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        horizontal_shader->useProgram();
        glBindVertexArray(vaos[0]);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        vertical_shader->useProgram();
        glBindVertexArray(vaos[1]);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void
    teardown(void) override
    {
        glDeleteVertexArrays(2, vaos);
        glDeleteBuffers(2, vbos);
        glDeleteBuffers(2, ebos);
        horizontal_shader.reset();
        vertical_shader.reset();
    }

    std::unique_ptr<Shader> horizontal_shader = nullptr;
    std::unique_ptr<Shader> vertical_shader = nullptr;
    GLuint vaos[2];
    GLuint vbos[2];
    GLuint ebos[2];
};

int
main(int, char **)
{
    HelloTriange app{};
    return app.run();
}
