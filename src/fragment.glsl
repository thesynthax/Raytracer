#version 460 core

in vec2 uv;

out vec4 FragColor;

uniform float aspectRatio;
uniform float time;

void main() {
    vec2 n = vec2(uv.x * aspectRatio, uv.y);
    float d = length(n);

    d = sin(d*10.0f + time);
    d = 0.1f/d;

    FragColor = vec4(d, d, d, 1.0f);
}
