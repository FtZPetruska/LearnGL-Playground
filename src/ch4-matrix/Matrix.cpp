#include "../BaseApplication.hpp"

#include "MatrixFiles.hpp"
#include "Shader.hpp"
#include "Utils.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

class Matrix : public BaseApplication
{
  public:
    virtual ~Matrix() = default;

  private:
    void
    setup(void) override
    {
        static constexpr std::array<GLfloat, 20> vertices{
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   /* Top right */
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  /* Bottom right */
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, /* Bottom left */
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  /* Top left */
        };

        static constexpr std::array<GLuint, 6> indices{
            0, 1, 3, /* First triangle */
            1, 2, 3, /* Second triangle */
        };

        Utils::Image yanfei{YANFEI_FILE};
        Utils::Image hutao{HUTAO_FILE};

        const Utils::ImageData &yanfei_data = yanfei.getImageData();
        const Utils::ImageData &hutao_data = hutao.getImageData();

        glGenTextures(static_cast<GLsizei>(textures.size()), textures.data());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        if (nullptr != yanfei_data.pixels) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, yanfei_data.width, yanfei_data.height, 0, GL_RGB, GL_UNSIGNED_BYTE, yanfei_data.pixels);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        if (nullptr != hutao_data.pixels) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, hutao_data.width, hutao_data.height, 0, GL_RGB, GL_UNSIGNED_BYTE, hutao_data.pixels);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        glGenVertexArrays(static_cast<GLsizei>(vaos.size()), vaos.data());
        glGenBuffers(static_cast<GLsizei>(vbos.size()), vbos.data());
        glGenBuffers(static_cast<GLsizei>(ebos.size()), ebos.data());

        glBindVertexArray(vaos[0]);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
        glBufferData(GL_ARRAY_BUFFER, Utils::arrayDataSize(vertices), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Utils::arrayDataSize(indices), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        shader = std::make_unique<Shader>(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
        shader->useProgram();
        shader->setUniform("texture0", 0);
        shader->setUniform("texture1", 1);
    }

    void
    processInputs(void) override
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        /* Translation vector */
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
            updateHorizontalOffset(-0.02f);
            flips.x = -1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            updateHorizontalOffset(0.02f);
            flips.x = 1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
            updateVerticalOffset(-0.02f);
        }
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
            updateVerticalOffset(0.02f);
        }

        /* Horizontal flip */
        static bool is_space_released = true;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && is_space_released) {
            flips.y *= -1.0f;
            is_space_released = false;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
            is_space_released = true;
        }

        /* Set texture mixing */
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            updateTextureMix(0.02f);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            updateTextureMix(-0.02f);
        }

        /* Transformation matrix */
        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
            updateMatrixScale(0.02f);
        }
        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
            updateMatrixScale(-0.02f);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            updateMatrixRotation(2.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            updateMatrixRotation(-2.0f);
        }
    }

    void
    updateHorizontalOffset(GLfloat increment)
    {
        static GLfloat current_offset = 0.0f;
        current_offset += increment;
        translation.x = Utils::clamp(current_offset, -1.0f, 1.0f);
    }

    void
    updateVerticalOffset(GLfloat increment)
    {
        static GLfloat current_offset = 0.0f;
        current_offset += increment;
        translation.y = Utils::clamp(current_offset, -1.0f, 1.0f);
    }

    void
    updateTextureMix(GLfloat increment)
    {
        static GLfloat mixer = 0.5f;
        mixer += increment;
        mixer = Utils::clamp(mixer, 0.0f, 1.0f);
        shader->setUniform("mixer", mixer);
    }

    void
    updateMatrixScale(GLfloat increment)
    {
        scale += increment;
    }

    void
    updateMatrixRotation(GLfloat increment)
    {
        angle += increment;
    }

    void
    render(void) override
    {
        Utils::RGBColour colour = scroller.getNext();
        glClearColor(colour.r, colour.g, colour.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Set the shader program and attributes (through vao) */
        shader->useProgram();
        updateTransformation();
        updateTextureFlip();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        glBindVertexArray(vaos[0]);

        /* Bind the element buffer and draw it */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        drawSecondBox();

        /* Keep last */
        glfwSwapBuffers(window);
    }

    void
    updateTransformation(void)
    {
        static constexpr glm::vec3 rotation_axis(0.0f, 0.0f, 1.0f);
        glm::mat4 transformation = glm::mat4(1.0f);
        /* Scale -> Rotate -> Translate is the recommended order of operation, but order of operation is reverse! */
        transformation = glm::translate(transformation, translation);
        transformation = glm::rotate(transformation, glm::radians(angle), rotation_axis);
        transformation = glm::scale(transformation, glm::vec3(scale, scale, 1.0f));
        shader->setUniform("transform", transformation);
    }

    void
    updateTextureFlip(void)
    {
        shader->setUniform("flips", flips);
    }

    void
    drawSecondBox(void)
    {
        GLfloat time = static_cast<GLfloat>(std::sin(glfwGetTime()));
        glm::mat4 transformation = glm::mat4(1.0f);
        transformation = glm::translate(transformation, glm::vec3(-0.5f, 0.5f, 0.0f));
        transformation = glm::scale(transformation, glm::vec3(time));
        shader->setUniform("transform", transformation);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
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
    std::array<GLuint, 2> textures;
    Utils::ScrollingColour scroller{};
    glm::vec3 translation{0.0f};
    glm::vec2 flips{1.0f};
    GLfloat scale = 1.0f;
    GLfloat angle = 0.0f;
};

int
main(int, char **)
{
    Matrix app{};
    return app.run();
}
