#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

unsigned int init_vertex_shader(const char *shader_source) {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shader_source, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
    }

    return vertexShader;
}

unsigned int init_fragment_shader(const char *shader_source) {
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shader_source, NULL);
    glCompileShader(fragmentShader);

    int success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
    }

    return fragmentShader;
}

void init_shader_program(unsigned int vertex_shader, unsigned int *shader_program, unsigned int *fragment_shader) {
    *shader_program = glCreateProgram();

    // Link compiled shaders to program object
    glAttachShader(*shader_program, vertex_shader);
    glAttachShader(*shader_program, *fragment_shader);
    glLinkProgram(*shader_program);

    int success;
    char infoLog[512];
    glGetProgramiv(*shader_program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(*shader_program, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::PROGRAM::SHADER::LINKING_FAILED\n%s", infoLog);
    }
}

// Example of a callback function- This one is used for handling window resizing
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // Need to redo the OpenGL viewport if the window size changes
    glViewport(0, 0, width, height);
}
