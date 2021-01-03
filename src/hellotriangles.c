#include <stdio.h>
#include <stdbool.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow *window;
unsigned int vertexShader;
unsigned int fragmentShader1;
unsigned int fragmentShader2;
unsigned int shaderProgram1;
unsigned int shaderProgram2;

float triangle1_vertices[] = {
    -0.5f,  0.5f, 0.0f,
    -0.8f, -0.5f, 0.0f,
    -0.2f, -0.5f, 0.0f,
};

float triangle2_vertices[] = {
     0.5f,  0.5f, 0.0f,
     0.8f, -0.5f, 0.0f,
     0.2f, -0.5f, 0.0f,
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

void error(char *message) {
    fprintf(stderr, "Error: %s\n", message);
    glfwTerminate();
}

// Example of a callback function- This one is used for handling window resizing
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // Need to redo the OpenGL viewport if the window size changes
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void init_vertex_shader() {
    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main() {\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
    }
}

void init_fragment_shader1() {
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "   FragColor = vec4((226.0f/255.0f), (205.0f/255.0f), (176.0f/255.0f), 1.0f);\n"
        "}\0";

    fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader1, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader1);

    int success;
    char infoLog[512];
    glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
    }
}

void init_fragment_shader2() {
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "   FragColor = vec4((150.0f/255.0f), (120.0f/255.0f), (180.0f/255.0f), 1.0f);\n"
        "}\0";

    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader2);

    int success;
    char infoLog[512];
    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
    }
}

void init_shader_program(unsigned int *shaderProgram, unsigned int *fragmentShader) {
    *shaderProgram = glCreateProgram();

    // Link compiled shaders to program object
    glAttachShader(*shaderProgram, vertexShader);
    glAttachShader(*shaderProgram, *fragmentShader);
    glLinkProgram(*shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(*shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::PROGRAM::SHADER::LINKING_FAILED\n%s", infoLog);
    }
}

void delete_shader_objects() {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader1);
    glDeleteShader(fragmentShader2);
}

int main() {
    glfwInit();

    // Set major and minor version of minimum required OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window object with GLFW and make it the current OpenGL context
    window = glfwCreateWindow(800, 600, "Hello Triangle", NULL, NULL);
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
    glViewport(0, 0, 800, 600); // Tell OpenGL the size of the rendering window.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ----- Shaders -----
    init_vertex_shader();
    init_fragment_shader1();
    init_fragment_shader2();
    init_shader_program(&shaderProgram1, &fragmentShader1);
    init_shader_program(&shaderProgram2, &fragmentShader2);
    delete_shader_objects(); // No longer needed after shaderProgram creation
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // ----- End Vertex data and attributes -----

    // render loop, GLFW has a function to determine if it needs to close the window
    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        // ------------- rendering commands -------------
        glClearColor((38.0f/255.0f), (27.0f/255.0f), (14.0f/255.0f), 1.0f); // sets the color buffer bit (state-setting function)
        glClear(GL_COLOR_BUFFER_BIT); // clears the screen with the color buffer bit and whatever we set it to (state-using function)

        glUseProgram(shaderProgram1); 
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glDrawElements instead of glDrawArrays to indicate that we want to draw using
        // indices provided by the element buffer
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glUseProgram(shaderProgram2); 
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

