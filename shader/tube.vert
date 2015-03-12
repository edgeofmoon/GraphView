//#version 330

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
	//float r = radius;
	float r = 0.4;
	/*
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
	*/
	
	vec3 tposition = position+normal*r;
	fposition = vec3(mvMat*vec4(tposition,1));

	colourV = color;
	fnormal = normalize(vec3(transpose(inverse(mvMat))*vec4(normal,0)));
	ftexCoord = texCoord;

    gl_Position = projMat*mvMat*vec4(tposition,1);
}