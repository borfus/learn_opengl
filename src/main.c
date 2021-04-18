#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "opengl_util.h"

#define WIDTH 1280
#define HEIGHT 960

float vertices[] = {
    -0.5, -0.5, -0.5, 0.0, 0.0,
    0.5, -0.5, -0.5, 1.0, 0.0,
    0.5, 0.5, -0.5, 1.0, 1.0,
    0.5, 0.5, -0.5, 1.0, 1.0,
    -0.5, 0.5, -0.5, 0.0, 1.0,
    -0.5, -0.5, -0.5, 0.0, 0.0,

    -0.5, -0.5, 0.5, 0.0, 0.0,
    0.5, -0.5, 0.5, 1.0, 0.0,
    0.5, 0.5, 0.5, 1.0, 1.0,
    0.5, 0.5, 0.5, 1.0, 1.0,
    -0.5, 0.5, 0.5, 0.0, 1.0,
    -0.5, -0.5, 0.5, 0.0, 0.0,

    -0.5, 0.5, 0.5, 1.0, 0.0,
    -0.5, 0.5, -0.5, 1.0, 1.0,
    -0.5, -0.5, -0.5, 0.0, 1.0,
    -0.5, -0.5, -0.5, 0.0, 1.0,
    -0.5, -0.5, 0.5, 0.0, 0.0,
    -0.5, 0.5, 0.5, 1.0, 0.0,

    0.5, 0.5, 0.5, 1.0, 0.0,
    0.5, 0.5, -0.5, 1.0, 1.0,
    0.5, -0.5, -0.5, 0.0, 1.0,
    0.5, -0.5, -0.5, 0.0, 1.0,
    0.5, -0.5, 0.5, 0.0, 0.0,
    0.5, 0.5, 0.5, 1.0, 0.0,

    -0.5, -0.5, -0.5, 0.0, 1.0,
    0.5, -0.5, -0.5, 1.0, 1.0,
    0.5, -0.5, 0.5, 1.0, 0.0,
    0.5, -0.5, 0.5, 1.0, 0.0,
    -0.5, -0.5, 0.5, 0.0, 0.0,
    -0.5, -0.5, -0.5, 0.0, 1.0,

    -0.5, 0.5, -0.5, 0.0, 1.0,
    0.5, 0.5, -0.5, 1.0, 1.0,
    0.5, 0.5, 0.5, 1.0, 0.0,
    0.5, 0.5, 0.5, 1.0, 0.0,
    -0.5, 0.5, 0.5, 0.0, 0.0,
    -0.5, 0.5, -0.5, 0.0, 1.0
};

vec3s cube_positions[] = {
    {{0.0, 0.0, 0.0}},
    {{2.0, 5.0, -15.0}},
    {{-1.5, -2.2, -2.5}},
    {{-3.8, -2.0, -12.3}},
    {{2.4, -0.4, -3.5}},
    {{-1.7, 3.0, -7.5}},
    {{1.3, -2.0, -2.5}},
    {{1.5, 2.0, -2.5}},
    {{1.5, 0.2, -1.5}},
    {{-1.3, 1.0, -1.5}}
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

// Camera vectors
vec3s camera_pos = {{0.0, 0.0, 3.0}};
vec3s camera_front = {{0.0, 0.0, -1.0}};
vec3s camera_up = {{0.0, 1.0, 0.0}};
float pitch = 0.0;
float yaw = -90.0; // Point towards the -z axis by default

void error(char *message) {
    fprintf(stderr, "Error: %s\n", message);
    glfwTerminate();
}

float delta_time = 0.0;
float last_frame = 0.0;
float percent = 0.2;
void process_input(GLFWwindow *window) {
    float camera_speed = 2.5 * delta_time;
     
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        percent += 0.001;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        percent -= 0.001;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera_pos = glms_vec3_muladds(camera_front, camera_speed, camera_pos);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera_pos = glms_vec3_sub(camera_pos, glms_vec3_scale(camera_front, camera_speed));
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera_pos = glms_vec3_sub(camera_pos, glms_vec3_scale(glms_vec3_normalize(glms_vec3_cross(camera_front, camera_up)), camera_speed));
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera_pos = glms_vec3_add(camera_pos, glms_vec3_scale(glms_vec3_normalize(glms_vec3_cross(camera_front, camera_up)), camera_speed));
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera_pos = glms_vec3_muladds(camera_up, camera_speed, camera_pos);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera_pos = glms_vec3_sub(camera_pos, glms_vec3_scale(camera_up, camera_speed));
    }
}

bool first_mouse = true;
float lastx = WIDTH/2, lasty = HEIGHT/2; // in the middle of the screen
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (first_mouse) {
        lastx = xpos;
        lasty = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - lastx;
    float yoffset = lasty - ypos; // reversed because y ranges from bottom to top
    lastx = xpos;
    lasty = ypos;

    const float sensitivity = 0.1;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // ypos constraints
    if (pitch > 89.0) {
        pitch = 89.0;
    }
    if (pitch < -89.0) {
        pitch = -89.0;
    }

    // Update camera direction
    vec3s direction;
    direction.x = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    direction.y = sin(glm_rad(pitch));
    direction.z = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
    camera_front = glms_normalize(direction);
}

float fov = 45.0;
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    // yoffset tells us the amount we scrolled vertically
    fov -= (float)yoffset;
    if (fov < 1.0) {
        fov = 1.0;
    }
    if (fov > 45.0) {
        fov = 45.0;
    }
}

int main() {
    GLFWwindow *window;

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

    // Capture mouse and hide it
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // ----- Shaders -----

    struct shader shader;
    shader_init(&shader, "resources/shaders/basic.vs", "resources/shaders/basic.fs");

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

    /* glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); */
    /* glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); */

    // Tell OpenGL how to interpret the vertex position data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    // Vertex texture attributes
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

    mat4s view = glms_mat4_identity();
    mat4s projection = glms_perspective(glm_rad(fov), (float)WIDTH / (float)HEIGHT, 0.1, 100.0);

    // render loop, GLFW has a function to determine if it needs to close the window
    shader_use(&shader);
    shader_set_int(&shader, "texture1", 0);
    shader_set_int(&shader, "texture2", 1);

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        // input
        process_input(window);

        // ------------- rendering commands -------------

        glClearColor((38.0f/255.0f), (27.0f/255.0f), (14.0f/255.0f), 1.0f); // sets the color buffer bit (state-setting function)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears the screen with the color buffer bit and whatever we set it to (state-using function)

        shader_set_float(&shader, "percent", percent);

        // Update camera view
        view = glms_lookat(camera_pos, glms_vec3_add(camera_pos, camera_front), camera_up);
        projection = glms_perspective(glm_rad(fov), (float)WIDTH / (float)HEIGHT, 0.1, 100.0);

        shader_set_mat4(&shader, "view", view);
        shader_set_mat4(&shader, "projection", projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);
        for (int i = 0; i < 10; i++) {
            mat4s model = glms_mat4_identity();
            model = glms_translate(model, cube_positions[i]);
            float angle = 20.0 * i;
            model = glms_rotate(model, glm_rad(angle), (vec3s){{1.0, 0.3, 0.5}});

            if (i % 3 == 0) {
                model = glms_rotate(model, (float)glfwGetTime() * glm_rad(10.0), (vec3s){{0.5, 1.0, 0.0}});
            }

            shader_set_mat4(&shader, "model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        // ------------- end rendering -------------

        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);

        // Calc delta_time
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
    }

    // GLFW has this function to clean and delete all of the resources used
    glfwTerminate();
    return 0;
}

