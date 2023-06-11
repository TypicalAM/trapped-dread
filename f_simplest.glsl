#version 330

uniform sampler2D textureMap0; 

out vec4 pixelColor; 
in vec2 iTexCoord0;
in vec4 ic; 
in vec4 n;
in vec4 l;
in vec4 v;

void main(void) {

	//Znormalizowane interpolowane wektory
	vec4 ml = normalize(l);
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);
	//Wektor odbity
	vec4 mr = reflect(-ml, mn);

	//Parametry powierzchni
	vec4 kd = texture(textureMap0, iTexCoord0);
	
	vec4 ks = kd;

	float nl = clamp(dot(mn, ml), 0, 1);
	float rv = pow(clamp(dot(mr, mv), 0, 1), 50);
	pixelColor= vec4(kd.rgb * nl, kd.a) + vec4(ks.rgb*rv, 0);
}
