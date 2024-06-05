#version 460 core
attribute vec3 aPos;
attribute vec2 aTexCoord;

varying vec2 pos;

void main()
{
    pos = aTexCoord;

    vec4 position = vec4(aPos, 1.0);
    position.xy = position.xy * 2.0 - 1.0;

    gl_Position = position; 
}
