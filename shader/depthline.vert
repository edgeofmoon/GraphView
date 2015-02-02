#version 330

uniform mat4 pespm;
uniform mat4 mvm;
uniform sampler2D deptexUnit;

in vec3 position;
in vec3 tangent;

out vec3 fposition;
out vec3 ftangent;

void main (void)
{
	fposition = vec3(mvm*vec4(position,1));
	ftangent = normalize(vec3(transpose(inverse(mvm))*vec4(tangent,0)));
    gl_Position = pespm*mvm*vec4(position,1);
}