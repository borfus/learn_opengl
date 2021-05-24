#version 330 core
in vec2 tex_coord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float percent;

uniform vec3 object_color;
uniform vec3 light_color;

out vec4 frag_color;

void main() {
    //frag_color = mix(texture(texture1, tex_coord), texture(texture2, tex_coord), percent);
    frag_color = vec4(light_color * object_color, 1.0);
}
