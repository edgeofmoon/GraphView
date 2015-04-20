#version 330

uniform mat4x4 mvMat;
uniform mat4x4 projMat;
uniform int screenSpace;

in vec3 position;
in vec3 normal;
in vec2 texCoord;
in float radius;
in vec4 color;


out vec4 colourV;
out vec3 fnormal;
out vec3 fposition;

out float isInBox;

void main (void)
{
	float r = max(0.02, radius);
	isInBox = 2;
	
	if(screenSpace == 1){
		vec4 p1=vec4(position, 1);
		vec4 p2=vec4(position+normal, 1);

		vec4 p3=projMat*mvMat*p1;
		vec4 p4=projMat*mvMat*p2;

		float size=length((p4-p3).xyz);

		r=r/size*4;
	}

	vec3 tposition = position+normal*r;
	gl_Position = projMat*mvMat*vec4(tposition,1);

	fposition = vec3(mvMat*vec4(tposition,1));

	colourV = color;
	fnormal = (transpose(inverse(mvMat))*vec4(normal,0)).xyz;
}