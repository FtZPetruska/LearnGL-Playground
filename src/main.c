#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#define WIDTH  800
#define HEIGHT 600

extern void setup(void);
extern void render(GLFWwindow *window);
extern void processInputs(GLFWwindow *window);
extern void teardown(void);

static void logAndExit(const char *error_message);
static void framebufferSizeCallback(GLFWwindow *window, GLsizei width, GLsizei height);

/* Initialise GLFW returns 0 on success, -1 on error */
static int
init(void)
{
    if (GLFW_FALSE == glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Apple being buttfaces again */
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    return 0;
}

int
main()
{
    GLFWwindow *window;
    if (0 > init()) {
        logAndExit("Failed to initialise GLFW.");
    }

    window = glfwCreateWindow(WIDTH, HEIGHT, "Baguet's GL playground", NULL, NULL);
    if (NULL == window) {
        logAndExit("Failed to create GLFW window.");
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        logAndExit("Failed to initialise GLAD.");
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    setup();

    while (!glfwWindowShouldClose(window)) {
        processInputs(window);
        render(window);
        glfwPollEvents();
    }

    teardown();

    glfwTerminate();
    return 0;
}

static void
logAndExit(const char *error_message)
{
    if (NULL != error_message) {
        fprintf(stderr, "%s\n", error_message);
    }
    glfwTerminate();
    exit(-1);
}

static void
framebufferSizeCallback(GLFWwindow *window, GLsizei width, GLsizei height)
{
    (void) window;
    glViewport(0, 0, width, height);
}
