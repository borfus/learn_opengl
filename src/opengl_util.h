#pragma once
#include <stdbool.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>

struct shader {
    unsigned int id;
    unsigned int vertex_shader;
    unsigned int fragment_shader;
    const char *vertex_code;
    const char *fragment_code;
};

void shader_init(struct shader *shader, const char *vertex_path, const char *fragment_path);
void shader_init_vertex_shader(struct shader *shader);
void shader_init_fragment_shader(struct shader *shader);
void shader_init_program(struct shader *shader);
void shader_use(struct shader *shader);
void shader_set_bool(struct shader *shader, const char *name, bool value);
void shader_set_int(struct shader *shader, const char *name, int v);
void shader_set_2int(struct shader *shader, const char *name, int v1, int v2);
void shader_set_3int(struct shader *shader, const char *name, int v1, int v2, int v3);
void shader_set_4int(struct shader *shader, const char *name, int v1, int v2, int v3, int v4);
void shader_set_float(struct shader *shader, const char *name, float v);
void shader_set_2float(struct shader *shader, const char *name, float v1, float v2);
void shader_set_3float(struct shader *shader, const char *name, float v1, float v2, float v3);
void shader_set_4float(struct shader *shader, const char *name, float v1, float v2, float v3, float v4);
void shader_set_mat4(struct shader *shader, const char *name, mat4s mat);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

