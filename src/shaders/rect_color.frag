#version 410
in vec3 uv;

out vec4 fragColor;

uniform sampler2D tex;

uniform int colorFlag;

void main()
{

	fragColor = vec4(0,0,0,1);
	if (colorFlag == 0) fragColor = vec4(0,0,1,1);	
	else
	if (colorFlag == 1) fragColor = vec4(0,1,0,1);
	else
	if (colorFlag == 2) fragColor = vec4(0.8,0.52,0.24,1);
    //float value = texture( tex, uv.xy).r;

    //if(value > 0.0001)
    //    fragColor = vec4(1,1,1,1);
    //else
    //    fragColor = vec4(0.67, 0.76, 0.78, 1);
}
