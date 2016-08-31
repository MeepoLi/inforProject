#version 410
in vec3 uv;

out vec4 fragColor;

uniform sampler2D tex;

void main()
{
//	fragColor = vec4(1,0,0,1);
    float value = texture( tex, uv.xy).r;

    if(value > 0.0001)
        fragColor = vec4(1,1,1,1);
    else
        fragColor = vec4(0.67, 0.76, 0.78, 1);
}
