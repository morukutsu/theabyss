uniform sampler2D texture;
uniform float radius = 4;
uniform float darkness = 0.2;
uniform float intensity;

void main()
{
	// Couleur originale
	vec4 srcColor = texture2D(texture, gl_TexCoord[0].st);
	
	// Couleur de travail
	vec4 color = srcColor;

	// ------------
	// EFFET
	// ------------
	vec2 inTex = gl_TexCoord[0].st - 0.5;
	float vignette  = 1 - dot(inTex, inTex);
	color.rgb    *= clamp(pow(vignette, radius) + darkness, 0.0, 1.0);
	
	// Fin
	gl_FragColor = color*intensity + srcColor*(1.0-intensity);
}