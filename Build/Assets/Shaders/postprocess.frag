#version 430

#define INVERT_MASK (1 << 0)
#define GREYSCALE_MASK (1 << 1)
#define COLORTINT_MASK (1 << 2)
#define GRAIN_MASK (1 << 3)
#define SCANLINE_MASK (1 << 4)
#define CUSTOM_MASK (1 << 5)

//in layout(location = 0) vec3 color;
in layout(location = 0) vec2 ftexcoord;

out layout(location = 0) vec4 ocolor;


uniform float blend = 1;
uniform uint params = 0;
uniform vec3 tint = { 1 };

layout(binding = 0) uniform sampler2D screenTexture;

vec4 invert(in vec4 color){
	return vec4(vec3(1) - color.rgb, color.a);
}

vec4 greyScale(in vec4 color){
	return vec4(vec3((0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b) / 3), color.a);
}

vec4 colorTint(in vec4 color, in vec4 tint){
	return vec4(color.rgb * tint.rgb, color.a);
}

vec4 grain(in vec4 color){
	// Adjust the 'intensity' to control the strength of the grain effect
    float intensity = .5;
    vec3 noisyColor = color.rgb + intensity * (2.0 * fract(sin(dot(gl_FragCoord.xyz, vec3(12.9898, 78.233, 45.5433))) * 43758.5453) - 1.0);
    return vec4(noisyColor, color.a);
}

vec4 scanline(in vec4 color){
	return (int(gl_FragCoord.y)  % 5 != 0) ? vec4( 0, 0, 0, color.a) : color;
}

vec4 custom(in vec4 color){
	 // Adjust the 'intensity' to control the strength of the random effect
    float intensity = 0.1;
    vec3 randomColor = color.rgb + intensity * vec3(sin(gl_FragCoord.x), cos(gl_FragCoord.y), tan(gl_FragCoord.x + gl_FragCoord.y));
    return vec4(randomColor, color.a);
}

void main()
{
	vec4 basecolor = texture(screenTexture, ftexcoord);
	vec4 postprocess = basecolor;
	
	//if (texcolor.a < 0.8) discard;
	if (bool(params & INVERT_MASK))  postprocess = invert(postprocess);
	if (bool(params & GREYSCALE_MASK)) postprocess = greyScale(postprocess);
	if (bool(params & COLORTINT_MASK)) postprocess = colorTint(postprocess, vec4(tint, 1));
	if (bool(params & GRAIN_MASK)) postprocess = grain(postprocess);
	if (bool(params & SCANLINE_MASK)) postprocess = scanline(postprocess);
	if (bool(params & CUSTOM_MASK)) postprocess = custom(postprocess);


	ocolor = mix(basecolor, postprocess, blend);
}
