#version 330

uniform sampler2D texUnit;
uniform mat4x4 mvMat;
uniform mat4x4 projMat;
uniform vec3 boxLow1;
uniform vec3 boxHigh1;
uniform vec3 boxLow2;
uniform vec3 boxHigh2;
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
	float r = radius;
	
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
	if(screenSpace == 1){
		float diff;
		float screenSize = radius * 0.01;
		float radiusUpper = 10.0;
		float size;
		do{
			radiusUpper = radiusUpper*2;
			vec4 p1=vec4(position, 1);
			vec4 p2=vec4(position+normal*radiusUpper, 1);
			vec4 p3=projMat*mvMat*p1;
			vec4 p4=projMat*mvMat*p2;
			size=length((p4-p3).xyz);
		}while(size<screenSize);

		float radiusBot = 0;
		int tries = 0;
		do{
			r = (radiusBot+radiusUpper)*0.5;
			
			vec4 p1=vec4(position, 1);
			vec4 p2=vec4(position+normal*r, 1);
			vec4 p3=projMat*mvMat*p1;
			vec4 p4=projMat*mvMat*p2;
			size=length((p4-p3).xyz);

			diff = abs(size-screenSize);
			if(size>screenSize){
				radiusUpper = r;
			}
			else{
				radiusBot = r;
			}
			tries = tries + 1;
		}
		while(diff > screenSize*0.01 && tries < 100);
	}
	*/

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