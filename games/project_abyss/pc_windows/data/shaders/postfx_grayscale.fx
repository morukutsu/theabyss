uniform sampler2D texture;

vec3 W = vec3(0.2125, 0.7154, 0.0721);

void main()
{
	// Couleur originale
	vec4 srcColor = texture2D(texture, gl_TexCoord[0].st);
	
	// Couleur de travail
	vec4 color = srcColor;
	
	// Visibilite de l'effet
	float intensity = 1.0;
	
	// ------------
	// EFFET
	// ------------
	float luminance = dot(color.rgb, W);
	color = vec4(vec3(luminance), color.a);
	
	// Fin
	gl_FragColor = color*intensity + srcColor*(1.0-intensity);
}