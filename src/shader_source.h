#pragma once

const char *vertex_shader_source = "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "layout(location = 1) in vec3 aColor;\n"

    "out vec4 vertex_color;\n"

    "void main() {\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   vertex_color = vec4(aColor, 1.0);\n"
    "}\0";

const char *fragment_shader_source_1 = "#version 330 core\n"
    "uniform vec4 our_color;\n"
    "out vec4 frag_color;\n"

    "void main() {\n"
    "   frag_color = our_color;\n"
    "}\0";

const char *fragment_shader_source_2 = "#version 330 core\n"
    "in vec4 vertex_color;\n"
    "out vec4 frag_color;\n"

    "void main() {\n"
    "   frag_color = vertex_color;\n"
    "}\0";


