#version 330

uniform sampler2D texUnit;
uniform mat4x4 mvMat;
uniform mat4x4 projMat;
uniform vec3 boxLow1;
uniform vec3 boxHigh1;
uniform vec3 boxLow2;
uniform vec3 boxHigh2;

in vec3 position;
in vec3 normal;
in vec2 texCoord;
in float radius;
in vec4 color;


out vec4 colourV;
out vec3 fnormal;
out vec2 ftexCoord;
out vec3 fposition;

out float isInBox;

void main (void)
{
	float r = max(0.02, radius);
	
	if(position.x > boxLow1.x && position.x < boxHigh1.x
		&& position.y > boxLow1.y && position.y < boxHigh1.y
		&& position.z > boxLow1.z && position.z < boxHigh1.z ){
		isInBox = 2;
	}
	else{
		if(position.x > boxLow2.x && position.x < boxHigh2.x
			&& position.y > boxLow2.y && position.y < boxHigh2.y
			&& position.z > boxLow2.z && position.z < boxHigh2.z ){
			isInBox = 2;
		}
		else{
			isInBox = 0;
			r = 0.0;
		}
	}
	/*
	vec4 p1=vec4(position, 1);
	vec4 p2=vec4(position+normal, 1);

	vec4 p3=projMat*mvMat*p1;
	vec4 p4=projMat*mvMat*p2;

	float size=length(vec3(p4-p3));

	r=r/size*30;
	*/
	vec3 tposition = position+normal*r;
	gl_Position = projMat*mvMat*vec4(tposition,1);

	fposition = vec3(mvMat*vec4(tposition,1));

	colourV = color;
	vec4 tnormal = transpose(inverse(mvMat))*vec4(normal,0);
	fnormal = normalize(vec3(tnormal.x, tnormal.y, tnormal.z));
	ftexCoord = texCoord;
}