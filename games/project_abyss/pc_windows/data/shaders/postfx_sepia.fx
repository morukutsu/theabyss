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
	mat4 colorMatrix = mat4(0.3588, 0.7044, 0.1368, 0.0,
							0.2990, 0.5870, 0.1140, 0.0,
							0.2392, 0.4696, 0.0912, 0.0,
							0, 0, 0, 1.0);
						
	color = srcColor * colorMatrix;
	
	// Fin
	gl_FragColor = color*intensity + srcColor*(1.0-intensity);
}