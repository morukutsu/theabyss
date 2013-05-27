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
	
	color.rgb *= 1.1;
	const float TonemapOversat = 7.0;
	const float TonemapCurve = 0.28;

	color.rgb = (color.rgb * (1.0 + color.rgb/TonemapOversat))/(color.rgb + TonemapCurve);
	
	color = color*(1.0 + 0.08) - 0.08;
	
	color = clamp(color, 0.0, 1.0);
	
	// Fin
	gl_FragColor = color*intensity + srcColor*(1.0-intensity);
}