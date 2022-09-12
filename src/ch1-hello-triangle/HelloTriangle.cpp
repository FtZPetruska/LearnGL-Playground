#include "../BaseApplication.hpp"

#include "HelloTriangleFiles.hpp"
#include "Shader.hpp"
#include "Utils.hpp"

#include <memory>

class HelloTriange : public BaseApplication
{
  public:
    virtual ~HelloTriange() = default;

  private:
    void
    setup(void) override
    {
        static constexpr std::array<GLfloat, 15> horizontal_vertices{
            0.0f, 0.0f, 0.0f,   /* Center */
            -0.6f, 0.5f, 0.0f,  /* Left-Up */
            -0.6f, -0.5f, 0.0f, /* Left-Down */
            0.6f, 0.5f, 0.0f,   /* Right-Up */
            0.6f, -0.5f, 0.0f,  /* Right-Down */
        };

        static constexpr std::array<GLfloat, 15> vertical_vertices{
            0.0f, 0.0f, 0.0f,   /* Center */
            -0.5f, 0.6f, 0.0f,  /* Top-left */
            0.5f, 0.6f, 0.0f,   /* Top-right */
            -0.5f, -0.6f, 0.0f, /* Bottom-Left */
            0.5f, -0.6f, 0.0f,  /* Bottom-Right */
        };

        static constexpr std::array<GLuint, 6> horizontal_indices{
            0, 1, 2, /* Left triangle */
            0, 3, 4, /* Top triange */
        };

        static constexpr std::array<GLuint, 6> vertical_indices{
            0, 1, 2, /* Top triangle */
            0, 3, 4, /* Bottom triangle */
        };

        /* Generate buffers */
        glGenVertexArrays(static_cast<GLsizei>(vaos.size()), vaos.data());
        glGenBuffers(static_cast<GLsizei>(vbos.size()), vbos.data());
        glGenBuffers(static_cast<GLsizei>(ebos.size()), ebos.data());

        /* Bind the VAO first so the latter commands are tied to it */
        glBindVertexArray(vaos[0]);

        /* Copy the vertices into the VBO */
        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
        glBufferData(GL_ARRAY_BUFFER, Utils::arrayDataSize(horizontal_vertices), horizontal_vertices.data(), GL_STATIC_DRAW);

        /* Copie the indices into the EBO */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Utils::arrayDataSize(horizontal_indices), horizontal_indices.data(), GL_STATIC_DRAW);

        /* Set vertices attributes */
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0);

        /* Bind the VAO first so the latter commands are tied to it */
        glBindVertexArray(vaos[1]);

        /* Copy the vertices into the VBO */
        glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
        glBufferData(GL_ARRAY_BUFFER, Utils::arrayDataSize(vertical_vertices), vertical_vertices.data(), GL_STATIC_DRAW);

        /* Copie the indices into the EBO */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Utils::arrayDataSize(vertical_indices), vertical_indices.data(), GL_STATIC_DRAW);

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
        Utils::RGBColour colour = scroller.getNext();
        glClearColor(colour.r, colour.g, colour.b, 1.0f);
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
        glDeleteVertexArrays(static_cast<GLsizei>(vaos.size()), vaos.data());
        glDeleteBuffers(static_cast<GLsizei>(vbos.size()), vbos.data());
        glDeleteBuffers(static_cast<GLsizei>(ebos.size()), ebos.data());
        horizontal_shader.reset();
        vertical_shader.reset();
    }

    std::unique_ptr<Shader> horizontal_shader = nullptr;
    std::unique_ptr<Shader> vertical_shader = nullptr;
    std::array<GLuint, 2> vaos, vbos, ebos;
    Utils::ScrollingColour scroller{};
};

int
main(int, char **)
{
    HelloTriange app{};
    return app.run();
}
