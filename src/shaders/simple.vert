#version 410
layout (location = 0) in vec3 vpos;

uniform dmat4 mvp;
out vec3 uv;

void main()
{
    gl_Position = vec4(mvp * dvec4(vpos,1));
    uv = vpos;
}
