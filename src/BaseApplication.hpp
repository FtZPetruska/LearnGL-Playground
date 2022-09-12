#ifndef BASEAPPLICATION_HPP
#define BASEAPPLICATION_HPP

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <fmt/core.h>

class BaseApplication
{
  public:
    int
    run(void)
    {
        if (0 > init()) {
            cleanup();
            return -1;
        }

        setup();
        while (!glfwWindowShouldClose(window)) {
            processInputs();
            render();
            glfwPollEvents();
        }
        teardown();

        cleanup();
        return 0;
    }

  private:
    int
    init(void)
    {
        if (GLFW_FALSE == glfwInit()) {
            fmt::print(stderr, "init: {}\n", "Failed to initialise GLFW.");
            return -1;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        static constexpr int WIDTH{800};
        static constexpr int HEIGHT{600};

        window = glfwCreateWindow(WIDTH, HEIGHT, "Baguet's GL playground", nullptr, nullptr);
        if (nullptr == window) {
            fmt::print(stderr, "init: {}\n", "Failed to create GLFW window.");
            return -1;
        }
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            fmt::print(stderr, "init: {}\n", "Failed to initialise GLAD.");
            return -1;
        }

        glViewport(0, 0, WIDTH, HEIGHT);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

        return 0;
    }

    virtual void setup(void) = 0;
    virtual void processInputs(void) = 0;
    virtual void render(void) = 0;
    virtual void teardown(void) = 0;

    void
    cleanup(void)
    {
        glfwTerminate();
        window = nullptr;
    }

    static void
    framebufferSizeCallback(GLFWwindow *, GLsizei width, GLsizei height)
    {
        glViewport(0, 0, width, height);
    }

  protected:
    /* The window should be accessible to the derived classes */
    GLFWwindow *window = nullptr;

    ~BaseApplication() = default;
};

#endif
