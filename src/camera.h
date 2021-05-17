#ifndef INCLUDE_CAMERA_H
#define INCLUDE_CAMERA_H

#include <glad/glad.h>
#include <cglm/types-struct.h>

#define CAMERA_DEFAULT_YAW -90.0f
#define CAMERA_DEFAULT_PITCH 0.0f
#define CAMERA_DEFAULT_SPEED 2.5f
#define CAMERA_DEFAULT_SENSITIVITY 0.1f
#define CAMERA_DEFAULT_FOV 45.0f 

enum camera_movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

struct camera {
    vec3s position;
    vec3s front;
    vec3s up;
    vec3s right;
    vec3s world_up;
    float yaw;
    float pitch;
    float speed;
    float sensitivity;
    float fov;
};

void camera_init_vec3s(struct camera *camera, vec3s posistion, vec3s world_up, float yaw, float pitch);
void camera_init_scalar(struct camera *camera, float posx, float posy, float posz, float upx, float upy, float upz, float yaw, float pitch);
mat4s camera_get_view_mat4s(struct camera *camera);
void camera_process_keyboard(struct camera *camera, enum camera_movement direction, float delta_time);
void camera_process_mouse_movement(struct camera *camera, float xoffset, float yoffset);
void camera_process_mouse_scroll(struct camera *camera, float yoffset);

#endif
