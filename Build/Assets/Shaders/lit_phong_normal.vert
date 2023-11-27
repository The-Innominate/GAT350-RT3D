#version 430

in layout(location = 0) vec3 vposition;
in layout(location = 1) vec2 vtexcoord;
in layout(location = 2) vec3 vnormal;
in layout(location = 3) vec3 vtangent;

out layout(location = 0) vec3 oposition;
out layout(location = 1) vec2 otexcoord;
// Added shadow mapping variable
out layout(location = 2) vec4 oshadowcoord;
out layout(location = 3) mat3 otbn;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Added uniforms
uniform mat4 shadowVP;

uniform struct Material{
	uint params;
	vec3 albedo;
	vec3 specular;
	vec3 emissive;
	float shininess;

	vec2 offset;
	vec2 tiling;
}material;

void main()
{
	otexcoord = (vtexcoord * material.tiling) + material.offset;
	mat4 modelView = view * model;
	oposition = vec3(modelView * vec4(vposition, 1));

	//Change made to read over the shadows to the frag
	oshadowcoord = shadowVP * model * vec4(vposition, 1.0);

	vec3 normal = normalize(mat3(modelView) * vnormal);
	vec3 tangent = normalize(mat3(modelView) * vtangent);
	vec3 bitangent = cross(normal, tangent);

	otbn = mat3(tangent, bitangent, normal);

	mat4 mvp = projection * view * model;
	//float xo = sin(time * 3 + position.y + position.x) + 0.5;
	gl_Position = mvp * vec4(vposition, 1.0);
}
