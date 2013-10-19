uniform sampler2D texture;
uniform sampler2D mask;

void main()
{
	// Couleur originale
	vec4 srcColor = texture2D(texture, gl_TexCoord[0].st);
	
	// ------------
	// EFFET
	// ------------
	vec4 maskColor = texture2D(mask, gl_TexCoord[0].st);
	
	srcColor.a = srcColor.a*maskColor.r;
	
	
	// Fin
	gl_FragColor = srcColor;
}