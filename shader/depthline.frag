#version 330

uniform sampler2D deptexUnit;

in vec3 fposition;
in vec3 ftangent;

out vec4 fragColour;

float LinearizeDepth(float z, float n, float f)
{
  return (2.0 * n) / (f + n - z * (f - n));	
}

// where zb = z from buffer
// bufferToCamera= z from camera
float bufferToCamera(float zb, float n, float f)
{
	//return - (f * n) / (zb * (f – n) – f);
	return f*n /( f - zb*(f-n) );
}

void main (void)
{
	float window_width = 1920;
	float window_height = 1066;
	float depth = texture(deptexUnit, vec2((gl_FragCoord.x)/window_width,(gl_FragCoord.y)/window_height)).x;
	float d = gl_FragCoord.z;
	fragColour = vec4(0.0,0.0,0.0,0);
	
	float camTubeDist = bufferToCamera(depth, 30, 200);
	float camLineDist = bufferToCamera(d, 30, 200);
	

	if(camLineDist > camTubeDist + 5){
		//fragColour = vec4(0.2,0.2,0.2,1);
	}
	float dist = length(fposition)/300;
	if(dist >= depth + 0.1/300){
		vec3 lightDir = vec3(0,1,0);
		vec3 eyeDir = normalize(-fposition);
		vec3 tangent = normalize(ftangent);
		float lt = dot(lightDir, tangent);
		float vt = dot(eyeDir, tangent);
		float value = max(lt*vt*sqrt(1-lt*lt)*sqrt(1-vt*vt),0);
		float specular = 0.15*pow(value, 1/32.0);
		float diffusion = 0.3*sqrt(1-lt*lt);
		float ambient = 0.1;
		float sum = ambient+diffusion+specular;
		fragColour = vec4(sum,sum,sum,1);
	}
}