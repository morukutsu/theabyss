uniform sampler2D texture;

void main()
{
	// Couleur originale
	vec4 srcColor = texture2D(texture, gl_TexCoord[0].st);
	
	// Couleur de travail
	vec4 color = srcColor;
	
	// Colore en noir l'élément
	color.r = 0.0;
	color.g = 0.0;
	color.b = 0.0;
	
	if(color.a > 0.10)
	{
		color.a = 1.0;
	}
	
	// Fin
	gl_FragColor = color;
}