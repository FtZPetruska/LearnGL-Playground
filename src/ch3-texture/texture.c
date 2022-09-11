#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "../shader/shader.h"
#include "../utils.h"
#include "texture_files.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static GLuint vao, vbo, ebo, textures[2];
static Shader shader;

static Colour updateColour(void);
static void updateOffset(float increment);
static void updateMix(float increment);

typedef struct
{
    int width, height, channels;
    unsigned char *data;
} Image;

static Image loadImage(const char *path);

void
setup(void)
{
    /** (x,y,z), (r, g, b), (t.x, t.y)
     * A flat array of 3D points with their colour and texture coordinate.
     */
    static const float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   /* Top right */
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  /* Bottom right */
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, /* Bottom left */
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  /* Top left */
    };

    /* Indices corresponding to the points defined in vertices. */
    static const GLuint indices[] = {
        0, 1, 3, /* First triangle */
        1, 2, 3, /* Second triangle */
    };

    Image yanfei = loadImage(SMUG_TEXT);
    Image hutao = loadImage(SHOCK_TEXT);

    /* Set the upscale and downscale settings for textures */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Create and binds a texture */
    glGenTextures(2, textures);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    if (yanfei.data) {
        /* Upload the image to the texture */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, yanfei.width, yanfei.height, 0, GL_RGB, GL_UNSIGNED_BYTE, yanfei.data);

        /* Generate the mipmap */
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    if (hutao.data) {
        /* Upload the image to the texture */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, hutao.width, hutao.height, 0, GL_RGB, GL_UNSIGNED_BYTE, hutao.data);

        /* Generate the mipmap */
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    /* Clean up the images */
    stbi_image_free(yanfei.data);
    stbi_image_free(hutao.data);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    shader = shader_make(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
    shader_use(shader);

    shader_setInt(shader, "texture0", 0);
    shader_setInt(shader, "texture1", 1);
}

static Image
loadImage(const char *path)
{
    Image out = {0, 0, 0, NULL};
    if (NULL == path) {
        fprintf(stderr, "loadImage: %s\n", "Path is NULL");
        return out;
    }
    stbi_set_flip_vertically_on_load(1);
    out.data = stbi_load(path, &out.width, &out.height, &out.channels, 0);
    return out;
}

void
processInputs(GLFWwindow *window)
{
    static GLboolean was_released = GL_TRUE;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        updateOffset(0.02f);
        shader_setInt(shader, "direction", 1);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        updateOffset(-0.02f);
        shader_setInt(shader, "direction", -1);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && was_released) {
        static int flip = 1;
        flip *= -1;
        shader_setInt(shader, "flip", flip);
        was_released = GL_FALSE;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        was_released = GL_TRUE;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        updateMix(-0.02f);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        updateMix(0.02f);
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

static void
updateMix(float increment)
{
    static float mixer = 0.5f;
    mixer += increment;
    mixer = clamp(mixer, 0.0f, 1.0f);
    shader_setFloat(shader, "mixer", mixer);
}

void
render(GLFWwindow *window)
{
    /* Clear the screen with a specific colour */
    updateColour();
    glClear(GL_COLOR_BUFFER_BIT);

    /* Set the shader program and attributes (through vao) */
    shader_use(shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glBindVertexArray(vao);

    /* Bind the element buffer and draw it */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

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
