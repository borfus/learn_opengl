#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void error(char *message) {
    fprintf(stderr, "%s", message);
    glfwTerminate();
}

// Example of a callback function- This one is used for handling window resizing
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // Need to redo the OpenGL viewport if the window size changes
    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();

    // Set major and minor version of minimum required OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window object with GLFW and make it the current OpenGL context
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (!window) {
        error("Failed to create GLFW window.\n");
        return 1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD with the 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        error("Failed to initialize GLAD.\n");
        return 1;
    }

    // Set the initial viewport but then also register a callback function for resizing the window
    glViewport(0, 0, 800, 600); // Tell OpenGL the size of the rendering window.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // render loop, GLFW has a function to determine if it needs to close the window
    while (!glfwWindowShouldClose(window)) {
        // Swaps a color buffer (a large 2D buffer that contains color values for a GLFW window)
        glfwSwapBuffers(window);
        // Checks for....polling events!! (keyboard input, mouse movement, etc)
        // Calls corresponding functions when events take place (normally registered through callback functions)
        glfwPollEvents();
    }

    // GLFW has this function to clean and delete all of the resources used
    glfwTerminate();
    return 0;
}

