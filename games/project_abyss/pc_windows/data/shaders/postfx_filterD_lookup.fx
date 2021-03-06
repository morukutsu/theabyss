uniform sampler2D texture;
uniform sampler2D lookup;

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
	float blueColor = color.b * 63.0;
	
    vec2 quad1;
    quad1.y = floor(floor(blueColor) / 8.0);
    quad1.x = floor(blueColor) - (quad1.y * 8.0);
    
    vec2 quad2;
    quad2.y = floor(ceil(blueColor) / 8.0);
    quad2.x = ceil(blueColor) - (quad2.y * 8.0);
    
    vec2 texPos1;
    texPos1.x = (quad1.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * color.r);
    texPos1.y = (quad1.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * color.g);
    
    vec2 texPos2;
    texPos2.x = (quad2.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * color.r);
    texPos2.y = (quad2.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * color.g);
    
    vec4 newColor1 = texture2D(lookup, texPos1);
    vec4 newColor2 = texture2D(lookup, texPos2);
    
    vec4 newColor = mix(newColor1, newColor2, fract(blueColor));
	color = vec4(newColor.rgb, color.w);
	
	// Fin
	gl_FragColor = color*intensity + srcColor*(1.0-intensity);
}