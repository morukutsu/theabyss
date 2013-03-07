uniform sampler2D texture;

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
	
	
	
	// Fin
	gl_FragColor = color*intensity + srcColor*(1.0-intensity);
}