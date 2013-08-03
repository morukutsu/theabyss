uniform sampler2D texture;
uniform float brightPassThreshold = 0.4;

void main()
{
	// Couleur originale
	vec4 srcColor = texture2D(texture, gl_TexCoord[0].st);
	
	// Couleur de travail
	vec4 color = srcColor;

	// ------------
	// EFFET
	// ------------
	vec3 luminanceVector = vec3(0.2125, 0.7154, 0.0721);
	float luminance = dot(luminanceVector, color.rgb);
    luminance = max(0.0, luminance - brightPassThreshold);
    color.rgb *= sign(luminance);
	
	
	// Augmenttion lumiere
	color.rgb *= 1.25;
	color.rgb = clamp(color.rgb, 0.0, 1.0);
	
    color.a = 1.0;

	
	// Fin
	gl_FragColor = color;
}