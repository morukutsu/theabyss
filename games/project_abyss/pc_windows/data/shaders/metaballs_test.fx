uniform sampler2D texture;

void main()
{
	// Couleur originale
	vec4 srcColor = texture2D(texture, gl_TexCoord[0].st);
	
	// Couleur de travail
	vec4 color = srcColor;

	if(color.r > 0.55)
	{
		color.r = 1.0;
		color.g = 1.0;
		color.b = 1.0;
	}
	
	// Fin
	gl_FragColor = color;
}