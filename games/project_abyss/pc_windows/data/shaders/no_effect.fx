uniform sampler2D texture;

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
	
	// Fin
	gl_FragColor = color*intensity + srcColor*(1.0-intensity);
}