#version 410
layout (location = 0) in vec3 vpos;
//layout (location = 1) in vec3 vcolor;

uniform dmat4 mvp;
//uniform dvec2 size;

//out vec3 color;
out vec3 uv;

void main()
{
//    gl_Position = vec4(mvp * dvec4(1,1,1,1) * dvec4(2*vpos-vec3(1,1,0),1));
    gl_Position = vec4(mvp * dvec4(vpos,1));//vec4(mvp * dvec4(2*vpos-vec3(1,1,0),1));

//    color = vcolor;
    uv = vpos;
}
