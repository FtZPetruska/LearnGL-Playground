#include "../BaseApplication.hpp"

#include "Shader.hpp"
#include "ShadingFiles.hpp"
#include "Utils.hpp"

#include <memory>

class Shading : public BaseApplication
{
  public:
    virtual ~Shading() = default;

  private:
    void
    setup(void) override
    {
        static constexpr std::array<GLfloat, 18> vertices{
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, /* Bottom left */
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  /* Bottom right */
            0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   /* Top */
        };

        static constexpr std::array<GLuint, 3> indices{0, 1, 2};

        glGenVertexArrays(static_cast<GLsizei>(vaos.size()), vaos.data());
        glGenBuffers(static_cast<GLsizei>(vbos.size()), vbos.data());
        glGenBuffers(static_cast<GLsizei>(ebos.size()), ebos.data());

        glBindVertexArray(vaos[0]);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
        glBufferData(GL_ARRAY_BUFFER, Utils::arrayDataSize(vertices), vertices.data(), GL_STATIC_DRAW);

        /* Copie the indices into the EBO */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Utils::arrayDataSize(indices), indices.data(), GL_STATIC_DRAW);

        /* Set vertices position attributes */
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0);

        /* Set vertices colour attributes*/
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        shader = std::make_unique<Shader>(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
    }

    void
    processInputs(void) override
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        /* Triangle movement */
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
            updateHorizontalOffset(-0.02f);
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            updateHorizontalOffset(0.02f);
        }
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
            updateVerticalOffset(-0.02f);
        }
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
            updateVerticalOffset(0.02f);
        }

        /* Triangle flip */
        static bool is_space_released = true;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && is_space_released) {
            updateFlip();
            is_space_released = false;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
            is_space_released = true;
        }
    }

    void
    updateHorizontalOffset(GLfloat increment)
    {
        static GLfloat current_offset = 0.0f;
        current_offset += increment;
        current_offset = Utils::clamp(current_offset, -1.0f, 1.0f);
        shader->setUniform("hOffset", current_offset);
    }

    void
    updateVerticalOffset(GLfloat increment)
    {
        static GLfloat current_offset = 0.0f;
        current_offset += increment;
        current_offset = Utils::clamp(current_offset, -1.0f, 1.0f);
        shader->setUniform("vOffset", current_offset);
    }

    void
    updateFlip(void)
    {
        static GLint flip = 1;
        flip *= -1;
        shader->setUniform("flip", flip);
    }

    void
    render(void) override
    {
        Utils::RGBColour colour = scroller.getNext();
        glClearColor(colour.r, colour.g, colour.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Set the shader program and attributes (through vao) */
        shader->useProgram();
        glBindVertexArray(vaos[0]);

        /* Bind the element buffer and draw it */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[0]);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

        /* Keep last */
        glfwSwapBuffers(window);
    }

    void
    teardown(void) override
    {
        glDeleteVertexArrays(static_cast<GLsizei>(vaos.size()), vaos.data());
        glDeleteBuffers(static_cast<GLsizei>(vbos.size()), vbos.data());
        glDeleteBuffers(static_cast<GLsizei>(ebos.size()), ebos.data());
    }

    std::unique_ptr<Shader> shader = nullptr;
    std::array<GLuint, 1> vaos, vbos, ebos;
    Utils::ScrollingColour scroller{};
};

int
main(int, char **)
{
    Shading app{};
    return app.run();
}