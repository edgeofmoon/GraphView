//#version 330

uniform sampler2D texUnit;
uniform mat4x4 mvMat;
uniform mat4x4 projMat;

in vec3 position;
in vec3 normal;
in vec2 texCoord;
in vec4 color;

out vec4 colourV;
out vec3 fnormal;
out vec2 ftexCoord;
out vec3 fposition;

void main (void)
{
	vec3 tposition = position;//-normal*0.4+normal*color.g*0.5;
	fposition = vec3(mvMat*vec4(tposition,1));
    gl_Position = projMat*mvMat*vec4(tposition,1);

	colourV = color;
	fnormal = normalize(vec3(transpose(inverse(mvMat))*vec4(normal,0)));
	ftexCoord = texCoord;
}