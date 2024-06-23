#version 460 core

in vec3 pos;

out vec4 FragColor;

void main() {
    FragColor = vec4(pos / 2.0f + 0.5f, 1.0);
}
