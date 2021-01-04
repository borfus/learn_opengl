#pragma once

const char *vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragment_shader_source_beige = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4((226.0f/255.0f), (205.0f/255.0f), (176.0f/255.0f), 1.0f);\n"
    "}\0";

const char *fragment_shader_source_purple = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4((150.0f/255.0f), (120.0f/255.0f), (180.0f/255.0f), 1.0f);\n"
    "}\0";

