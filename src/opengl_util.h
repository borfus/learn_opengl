#pragma once

unsigned int init_vertex_shader(const char *shader_source);
unsigned int init_fragment_shader(const char *shader_source);
void init_shader_program(unsigned int vertex_shader, unsigned int *shader_program, unsigned int *fragment_shader);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

