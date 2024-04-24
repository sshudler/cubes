# Cubes

This small OpenGL demo application is based on the material and code here:
<https://learnopengl.com/Introduction>
<https://github.com/JoeyDeVries/LearnOpenGL>

It shows 10 randomly spinning cubes such that each cube has a different spinning velocity. The code implements a very simple scene graph in which the cube model is shared between 10 sibling nodes. Each side of the cube has the same texture.

The code compiles and runs in Linux (Ubuntu 22.04, to be precise). It uses CMake for the compilation process and requires the glfw library.
