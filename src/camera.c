#include "camera.h"
#include <cglm/cglm.h>
#include <cglm/struct.h>

const float CAMERA_DEFAULT_YAW = -90.0f; // Point towards the -z axis by default
const float CAMERA_DEFAULT_PITCH = 0.0f;
const float CAMERA_DEFAULT_SPEED = 2.5f;
const float CAMERA_DEFAULT_SENSITIVITY = 0.1f;
const float CAMERA_DEFAULT_FOV = 45.0f;

void update_camera_vectors(struct camera *camera) {
    vec3s front;
    front.x = (float) cos(glm_rad(camera->yaw)) * (float) cos(glm_rad(camera->pitch));
    front.y = (float) sin(glm_rad(camera->pitch));
    front.z = (float) sin(glm_rad(camera->yaw)) * (float) cos(glm_rad(camera->pitch));
    camera->front = glms_vec3_normalize(front);
    camera->right = glms_vec3_normalize(glms_cross(camera->front, camera->world_up));
    camera->up = glms_vec3_normalize(glms_cross(camera->right, camera->front));
}

void camera_init_vec3s(struct camera *camera, vec3s posistion, vec3s world_up, float yaw, float pitch) {
    camera->front = (vec3s) {{0.0f, 0.0f, -1.0f}};
    camera->speed = CAMERA_DEFAULT_SPEED;
    camera->sensitivity = CAMERA_DEFAULT_SENSITIVITY;
    camera->fov = CAMERA_DEFAULT_FOV;

    camera->position = posistion;
    camera->world_up = world_up;
    camera->yaw = yaw;
    camera->pitch = pitch;

    update_camera_vectors(camera);
}

void camera_init_scalar(struct camera *camera, float posx, float posy, float posz, float upx, float upy, float upz, float yaw, float pitch) {
    camera->front = (vec3s) {{0.0f, 0.0f, -1.0f}};
    camera->speed = CAMERA_DEFAULT_SPEED;
    camera->sensitivity = CAMERA_DEFAULT_SENSITIVITY;
    camera->fov = CAMERA_DEFAULT_FOV;

    camera->position = (vec3s) {{posx, posy, posz}};
    camera->world_up = (vec3s) {{upx, upy, upz}};
    camera->yaw = yaw;
    camera->pitch = pitch;

    update_camera_vectors(camera);
}

mat4s camera_get_view_mat4s(struct camera *camera) {
    return glms_lookat(camera->position, glms_vec3_add(camera->position, camera->front), camera->up);
}

void camera_process_keyboard(struct camera *camera, enum camera_movement direction, float delta_time) {
    float velocity = camera->speed * delta_time;
    vec3s camera_grounded = (vec3s) {{1.0f, 0.0f, 1.0f}};
    if (direction == FORWARD) {
        // Fly-style camera
        //camera->position = glms_vec3_muladds(camera->front, velocity, camera->position);
        // FPS-style camera
        camera->position = glms_vec3_add(camera->position, glms_vec3_scale(glms_vec3_normalize(glms_vec3_mul(camera->front, camera_grounded)), velocity));
    }
    if (direction == BACKWARD) {
        // Fly-style camera
        //camera->position= glms_vec3_sub(camera->position, glms_vec3_scale(camera->front, velocity));
        // FPS-style camera
        camera->position = glms_vec3_sub(camera->position, glms_vec3_scale(glms_vec3_normalize(glms_vec3_mul(camera->front, camera_grounded)), velocity));
    }
    if (direction == LEFT) {
        camera->position = glms_vec3_sub(camera->position, glms_vec3_scale(camera->right, velocity));
    }
    if (direction == RIGHT) {
        camera->position = glms_vec3_add(camera->position, glms_vec3_scale(camera->right, velocity));
    }
    if (direction == UP) {
        camera->position = glms_vec3_muladds(camera->world_up, velocity, camera->position);
    }
    if (direction == DOWN) {
        camera->position = glms_vec3_sub(camera->position, glms_vec3_scale(camera->world_up, velocity));
    }
}

void camera_process_mouse_movement(struct camera *camera, float xoffset, float yoffset) {
    xoffset *= camera->sensitivity;
    yoffset *= camera->sensitivity;
    camera->yaw += xoffset;
    camera->pitch += yoffset;

    if (camera->pitch > 89.0f) {
        camera->pitch = 89.0f;
    }
    if (camera->pitch < -89.0f) {
        camera->pitch = -89.0f;
    }

    update_camera_vectors(camera);
}

void camera_process_mouse_scroll(struct camera *camera, float yoffset) {
    float fov = camera->fov;

    fov -= yoffset;
    if (fov < 1.0f) {
        fov = 1.0f;
    }
    if (fov > CAMERA_DEFAULT_FOV) {
        fov = CAMERA_DEFAULT_FOV;
    }

    camera->fov = fov;
}
