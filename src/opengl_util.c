#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>

#include "opengl_util.h"
#include "file_util.h"

void shader_init(struct shader *shader, const char *vertex_path, const char *fragment_path) {
    FILE *vertex_shader_file = fopen(vertex_path, "r");
    if (vertex_shader_file == NULL) {
        fprintf(stderr, "Failed to open vertex shader file with path %s.\n", vertex_path);
        exit(1);
    }

    FILE *fragment_shader_file = fopen(fragment_path, "r");
    if (fragment_shader_file == NULL) {
        fprintf(stderr, "Failed to open fragment shader file with path %s.\n", fragment_path);
        exit(1);
    }

    shader->vertex_code = read_file_string(vertex_shader_file);
    shader->fragment_code = read_file_string(fragment_shader_file);

    shader_init_vertex_shader(shader);
    shader_init_fragment_shader(shader);
    shader_init_program(shader); 
}

void shader_init_vertex_shader(struct shader *shader) {
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &(shader->vertex_code), NULL);
    glCompileShader(vertex_shader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
    }

    shader->vertex_shader = vertex_shader;
}

void shader_init_fragment_shader(struct shader *shader) {
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &(shader->fragment_code), NULL);
    glCompileShader(fragment_shader);

    int success;
    char infoLog[512];
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
    }

    shader->fragment_shader = fragment_shader;
}

void shader_init_program(struct shader *shader) {
    shader->id = glCreateProgram();

    // Link compiled shaders to program object
    glAttachShader(shader->id, shader->vertex_shader);
    glAttachShader(shader->id, shader->fragment_shader);
    glLinkProgram(shader->id);

    glDeleteShader(shader->vertex_shader);
    glDeleteShader(shader->fragment_shader);

    int success;
    char infoLog[512];
    glGetProgramiv(shader->id, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shader->id, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::PROGRAM::SHADER::LINKING_FAILED\n%s", infoLog);
    }
}

void shader_use(struct shader *shader) {
    glUseProgram(shader->id);
}

void shader_set_bool(struct shader *shader, const char *name, bool value) {
    glUniform1i(glGetUniformLocation(shader->id, name), (int)value);
}

void shader_set_int(struct shader *shader, const char *name, int v) {
    glUniform1i(glGetUniformLocation(shader->id, name), v);
}

void shader_set_2int(struct shader *shader, const char *name, int v1, int v2) {
    glUniform2i(glGetUniformLocation(shader->id, name), v1, v2);
}

void shader_set_3int(struct shader *shader, const char *name, int v1, int v2, int v3) {
    glUniform3i(glGetUniformLocation(shader->id, name), v1, v2, v3);
}

void shader_set_4int(struct shader *shader, const char *name, int v1, int v2, int v3, int v4) {
    glUniform4i(glGetUniformLocation(shader->id, name), v1, v2, v3, v4);
}

void shader_set_float(struct shader *shader, const char *name, float v) {
    glUniform1f(glGetUniformLocation(shader->id, name), v);
}

void shader_set_2float(struct shader *shader, const char *name, float v1, float v2) {
    glUniform2f(glGetUniformLocation(shader->id, name), v1, v2);
}

void shader_set_3float(struct shader *shader, const char *name, float v1, float v2, float v3) {
    glUniform3f(glGetUniformLocation(shader->id, name), v1, v2, v3);
}

void shader_set_4float(struct shader *shader, const char *name, float v1, float v2, float v3, float v4) {
    glUniform4f(glGetUniformLocation(shader->id, name), v1, v2, v3, v4);
}

void shader_set_mat4(struct shader *shader, const char *name, mat4s mat) {
    glUniformMatrix4fv(glGetUniformLocation(shader->id, name), 1, GL_FALSE, (const GLfloat *)&mat.raw);
}

// Example of a callback function- This one is used for handling window resizing
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // Need to redo the OpenGL viewport if the window size changes
    glViewport(0, 0, width, height);
}
