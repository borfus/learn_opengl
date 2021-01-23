#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "opengl_util.h"
#include "shader_source.h"

#define WIDTH 1280
#define HEIGHT 960

float triangle1_vertices[] = {
    -0.5f,  0.5f, 0.0f,
    -0.8f, -0.5f, 0.0f,
    -0.2f, -0.5f, 0.0f
};

float triangle2_vertices[] = {
    // positions         // colors
    0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f, // top
    0.8f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, // bottom right
    0.2f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f  // bottom left
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

void error(char *message) {
    fprintf(stderr, "Error: %s\n", message);
    glfwTerminate();
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    GLFWwindow *window;
    unsigned int vertex_shader;
    unsigned int fragment_shader_1;
    unsigned int fragment_shader_2;
    unsigned int shader_program_1;
    unsigned int shader_program_2;

    glfwInit();

    // Set major and minor version of minimum required OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window object with GLFW and make it the current OpenGL context
    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello Triangle", NULL, NULL);
    if (!window) {
        error("Failed to create GLFW window.");
        return 1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD with the 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        error("Failed to initialize GLAD.");
        return 1;
    }

    // Set the initial viewport but then also register a callback function for resizing the window
    glViewport(0, 0, WIDTH, HEIGHT); // Tell OpenGL the size of the rendering window.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ----- Shaders -----
    vertex_shader = init_vertex_shader(vertex_shader_source);
    fragment_shader_1 = init_fragment_shader(fragment_shader_source_1);
    fragment_shader_2 = init_fragment_shader(fragment_shader_source_2);

    init_shader_program(vertex_shader, &shader_program_1, &fragment_shader_1);
    init_shader_program(vertex_shader, &shader_program_2, &fragment_shader_2);

    // No longer needed after shaderProgram creation
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader_1);
    glDeleteShader(fragment_shader_2);
    // ----- End Shaders -----

    // ----- Vertex data and attributes -----
    // Buffer data
    unsigned int VBO1;
    glGenBuffers(1, &VBO1);
    unsigned int VBO2;
    glGenBuffers(1, &VBO2);
    // Vertex Array Object for repeated use and simplifying render calls
    unsigned int VAO1;
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);
    // Element buffer object
    // Helpful for storing elements of triangle vertices from an array
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    // Bind vertex data to VBO so it can be used for rendering
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1_vertices), triangle1_vertices, GL_STATIC_DRAW);

    // Tell OpenGL how to interpret the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // Vertex Array Object for repeated use and simplifying render calls
    unsigned int VAO2;
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    // Bind vertex data to VBO so it can be used for rendering
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2_vertices), triangle2_vertices, GL_STATIC_DRAW);

    // Tell OpenGL how to interpret the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ----- End Vertex data and attributes -----

    // render loop, GLFW has a function to determine if it needs to close the window
    while (!glfwWindowShouldClose(window)) {
        // input
        process_input(window);

        // ------------- rendering commands -------------
        glClearColor((38.0f/255.0f), (27.0f/255.0f), (14.0f/255.0f), 1.0f); // sets the color buffer bit (state-setting function)
        glClear(GL_COLOR_BUFFER_BIT); // clears the screen with the color buffer bit and whatever we set it to (state-using function)


        float time_value = glfwGetTime();
        float green_value = (sin(time_value) / 2.0) + 0.5f;
        int color_location = glGetUniformLocation(shader_program_1, "our_color");
        glUseProgram(shader_program_1); 
        glUniform4f(color_location, 0.0, green_value, 0.0, 1.0);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glDrawElements instead of glDrawArrays to indicate that we want to draw using
        // indices provided by the element buffer
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glUseProgram(shader_program_2); 
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // ------------- end rendering -------------

        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // GLFW has this function to clean and delete all of the resources used
    glfwTerminate();
    return 0;
}

