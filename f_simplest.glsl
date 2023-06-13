#version 330

uniform sampler2D textureMap0; 

out vec4 pixelColor; 
in vec2 iTexCoord0;
in vec4 ic; 
in vec4 n;
in vec4 l;
in vec4 v;

in vec4 coneLights_l[4];
in vec4 coneLights_cols[4];

in vec4 downVec;
in float iner_cutoff;
in float outer_cutoff;

in float icDist;


float quadratic  = 0.32f;
float linear  = 0.09f;



void main(void) {

	vec3 my_output = vec3(0.0);
	
	//point light source
	//Znormalizowane interpolowane wektory
	vec4 ml = normalize(l);  // do swiatla
	vec4 mn = normalize(n); // normalny
	vec4 mv = normalize(v); // do viewera
	//Wektor odbity
	vec4 mr = reflect(-ml, mn);



	//Parametry powierzchni
	vec4 kd = texture(textureMap0, iTexCoord0);
	vec4 ks = vec4(	0.91, 0.478, 0.31, 0);
	vec4 ks_spot = vec4(1);

	float nl = clamp(dot(mn, ml), 0, 1);
	float rv = pow(clamp(dot(mr, mv), 0, 1), 50);
	my_output = kd.rgb * nl + ks.rgb*rv;

	// atenumation
	float attenuation = 1.0 / (1 + linear * icDist + quadratic * (icDist * icDist)); 
	my_output *= attenuation ; 


	vec4 norDown = normalize(downVec);
	// time for cone lights
	for (int i = 0; i < 4; i++){
		vec4 norm_spot_l = normalize(coneLights_l[i]);
		
		float theta = dot(norm_spot_l, norDown);

		if (theta > outer_cutoff){
			// swietlamy
			float nlspot = clamp(dot(mn, -norm_spot_l), 0, 1);
			

			float epsilon = iner_cutoff - outer_cutoff;
			float intensity = clamp((theta - outer_cutoff) / epsilon, 0.0, 1.0);    
			
			my_output +=  intensity * kd.rgb * nlspot * coneLights_cols[i].rgb;
		}
	}

	pixelColor = vec4(my_output, kd.a);
}
