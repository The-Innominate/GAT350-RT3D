#version 430

in layout(location = 0) vec3 vposition;
in layout(location = 2) vec3 vnormal;

out layout(location = 0) vec3 otexcoord;

uniform float ior = 2.417;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec3 vreflect(vec3 i, vec3 n){
	return i-(n*dot(n,i)) * 2;
}

void main()
{
	//Transform vertex position/normal to world space
	vec3 position = vec3(model * vec4(vposition, 1));
	vec3 normal = normalize(mat3(model) * vnormal);

	// Inverse of view space -> world space
	// Last Column of mat4 is position
	vec3 viewPosition = inverse(view)[3].xyz;
	// When you have 2 positions always subtract head from tail
	vec3 viewDir = normalize(position - viewPosition);



	otexcoord = refract(viewDir, normal, 1.0 / ior);

	mat4 vp = projection * view * model;
	gl_Position = vp * vec4(vposition, 1.0);
}
