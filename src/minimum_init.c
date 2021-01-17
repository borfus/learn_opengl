#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// This is the minimum amount of code to initialize GLFW and GLAD that I can tell
// Compiled with: gcc main.c glad.c -lGL -lglfw -ldl
int main() {
    // ----- Begin Initializing -----

    glfwInit();

    // Set major and minor version of minimum required OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window object with GLFW and make it the current OpenGL context
    GLFWwindow *window = glfwCreateWindow(800, 600, "Window name", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window.\n");
        return 1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD with the 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD.\n");
        return 1;
    }

    // ----- Done initializing -----

    int max;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max);
    printf("Max vertex attributes supported: %i\n", max);

    return 0;
}

