uniform sampler2D texture;

//uniform float intensity;

float blurSize = 1.0/(512.0);

void main()
{
	// ------------
	// EFFET
	// ------------
	
	vec4 sum = vec4(0.0);
	
	//for (int i = 0; i < 2; i++) 
	//{
		// Couleur originale
	vec4 srcColor = texture2D(texture, gl_TexCoord[0].st);
		
		// Couleur de travail
		//vec4 color = srcColor;
		
		// Visibilite de l'effet
		//float intensity = 1.0;
	
		// blur in y (vertical)
		// take nine samples, with the distance blurSize between them
		sum += texture2D(texture, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y - 4.0*blurSize)) * 0.05;
		sum += texture2D(texture, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y - 3.0*blurSize)) * 0.09;
		sum += texture2D(texture, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y - 2.0*blurSize)) * 0.12;
		sum += texture2D(texture, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y - blurSize)) * 0.15;
		sum += texture2D(texture, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y)) * 0.16;
		sum += texture2D(texture, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y + blurSize)) * 0.15;
		sum += texture2D(texture, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y + 2.0*blurSize)) * 0.12;
		sum += texture2D(texture, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y + 3.0*blurSize)) * 0.09;
		sum += texture2D(texture, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y + 4.0*blurSize)) * 0.05;
		
		
	//}
	
	// Fin	
	//gl_FragColor = sum * intensity + srcColor * (1.0 - intensity);
	gl_FragColor = sum;
}