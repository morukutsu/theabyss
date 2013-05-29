uniform sampler2D texture;

uniform float intensity;

float height;

#define distortion 0.4

// Distorsion radiale
vec2 radialDistortion(vec2 coord)
{
	//vec2 cc = coord - vec2(0.5, +0.25);
	vec2 cc = coord - vec2(0.5, 0.25);
	float dist = dot(cc, cc) * distortion;
	cc = (coord + cc * (1.0 + dist) * dist);
	return cc;
}
		
void main()
{
	// Couleur originale
	vec2 uv = gl_TexCoord[0].st;
	
	//uv = radialDistortion(uv);
	
	vec4 srcColor = texture2D(texture, uv);
	vec4 newColor = srcColor;
	
	// SCANLINES
	// 3 scanlines tout les 6 pixels
	int a = int(clamp(mod(gl_FragCoord.y, 6), 0.0, 1.0 ));
	int b = int(clamp(mod(gl_FragCoord.y + 1, 6), 0.0, 1.0 ));
	int c = int(clamp(mod(gl_FragCoord.y + 2, 6), 0.0, 1.0 ));
	
    int m = int(min(a, min(b, c) ));
	
	// Ajout d'une touche de bleu sur les scanlines noires
	if(m == 0) 
	{
		newColor.r = 0;
		newColor.g = 0;
		newColor.b = 0.2;
		newColor.a = 1;
		
		newColor = min(newColor, srcColor);
	}
	
	// DISTORSION
	
	// Fin
	gl_FragColor = newColor * intensity;
}