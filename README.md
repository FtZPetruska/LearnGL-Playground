# LearnOpenGL Playground

Personal playground for the [LearnOpenGL](https://learnopengl.com) tutorials.

I initially tried to do it all in C89 before the introduction of GLM. And since I didn't feel like writing a whole linear algebra library in C, I decided to start over using modern C++.

## Dependencies

This project aims to use C++20 feature, any recent version of GCC/Clang/MSVC _should_ work. CMake and a generator is required for compilation.

The project has the following dependencies:

- [fmtlib](https://fmt.dev/latest/index.html)
- [Glad](https://glad.dav1d.de/)
- [GLFW](https://www.glfw.org/)
- [GLM](https://glm.g-truc.net/0.9.9/index.html)
- [stb_image](https://github.com/nothings/stb)

Vendored versions are provided in the `external` directory. Make sure you run `git submodule init && git submodule update` if you wish to use them.

You can control whether to skip searching for the libraries in your system by setting the following variables to `ON` (defaults to `OFF`):

```cmake
USE_VENDORED_GLFW
USE_VENDORED_GLM
USE_VENDORED_FMT
```

## Attribution and licensing

The code samples provided by [Joey de Vries](http://joeydevries.com/) are published under [CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc/4.0/legalcode).

[fmtlib License](https://github.com/fmtlib/fmt/blob/master/LICENSE.rst)

[Glad License](https://github.com/Dav1dde/glad/blob/master/LICENSE)

[GLFW License](https://www.glfw.org/license)

[GLM License](https://github.com/g-truc/glm/blob/master/copying.txt)

[stb_image](https://github.com/nothings/stb/blob/master/LICENSE)

Images in the `assets` directory are just memes I was sent.

The modified code in this repo is distributed under the MIT license. It is mainly just to be covered for a permissive license, this code is just hacking around a tutorial.
