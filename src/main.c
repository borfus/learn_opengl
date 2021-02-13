#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "opengl_util.h"

#define WIDTH 1280
#define HEIGHT 960

float vertices[] = {
    // positions     // colors      // texture coords
     0.5,  0.5, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, // top right
     0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, // bottom right
    -0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, // bottom left
    -0.5,  0.5, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0  // top left
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
    unsigned int fragment_shader;
    unsigned int shader_program;

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

    struct shader shader;
    shader_init(&shader, "resources/shaders/vshader", "resources/shaders/fshader");

    // ----- End Shaders -----

    // ----- Vertex data and attributes -----
    // Buffer data
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // Vertex Array Object for repeated use and simplifying render calls
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // Element buffer object
    // Helpful for storing elements of triangle vertices from an array
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    // Bind vertex data to VBO so it can be used for rendering
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Tell OpenGL how to interpret the vertex position data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    // Vertex color attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Vertex texture attributes
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // ----- End Vertex data and attributes -----

    // ----- Textures -----

    // Load the texture width, height, number of color channels, and byte data with stb_image
    int tex_width, tex_height, tex_color_channels;
    unsigned char *tex_data = stbi_load("resources/textures/container.jpg", &tex_width, &tex_height, &tex_color_channels, 0);

    // Create the texture object and store the ID
    unsigned int texture1;
    glGenTextures(1, &texture1);

    // Bind the texture object so that future commands related to textures opperate on our newly created texture object
    glBindTexture(GL_TEXTURE_2D, texture1);

    // Set texture wrapping and filtering properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (tex_data) {
        // Generate the texture and mipmap for the bound GL_TEXTURE_2D target
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        error("Failed to load texture.");
        return 1;
    }

    stbi_image_free(tex_data);

    stbi_set_flip_vertically_on_load(true);
    tex_data = stbi_load("resources/textures/awesomeface.png", &tex_width, &tex_height, &tex_color_channels, 0);

    unsigned int texture2;
    glGenTextures(1, &texture2);

    // Bind the texture object so that future commands related to textures opperate on our newly created texture object
    glBindTexture(GL_TEXTURE_2D, texture2);

    // Set texture wrapping and filtering properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (tex_data) {
        // Generate the texture and mipmap for the bound GL_TEXTURE_2D target
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        error("Failed to load texture.");
        return 1;
    }

    // Free the texture data loaded in with stb_image since we don't need it anymore
    stbi_image_free(tex_data);

    // ----- End Textures -----

    // render loop, GLFW has a function to determine if it needs to close the window
    shader_use(&shader);
    shader_set_int(&shader, "texture1", 0);
    shader_set_int(&shader, "texture2", 1);
    while (!glfwWindowShouldClose(window)) {
        // input
        process_input(window);

        // ------------- rendering commands -------------

        glClearColor((38.0f/255.0f), (27.0f/255.0f), (14.0f/255.0f), 1.0f); // sets the color buffer bit (state-setting function)
        glClear(GL_COLOR_BUFFER_BIT); // clears the screen with the color buffer bit and whatever we set it to (state-using function)

        float time_value = glfwGetTime();
        float green_value = (sin(time_value) / 2.0) + 0.5f;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // ------------- end rendering -------------

        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // GLFW has this function to clean and delete all of the resources used
    glfwTerminate();
    return 0;
}

