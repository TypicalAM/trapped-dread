#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Atrybuty
in vec4 vertex;
in vec4 color; 
in vec4 normal; 
in vec2 texCoord0;


//Zmienne interpolowane
out vec4 ic;
out vec4 l;
out vec4 n;
out vec4 v;
out vec2 iTexCoord0; 

// distout
out float icDist;

//cone lights
uniform vec4 coneLightpositions[4];
uniform vec4 coneLightcolors[4];

uniform float cutoffin;
uniform float cutoffout;

out vec4 coneLights_l[4];
out vec4 coneLights_cols[4];

out vec4 downVec;

out float iner_cutoff;
out float outer_cutoff;

void main(void) {
    
    float color_mult = 0.5; // <-- zmienna do testow

    iner_cutoff = cutoffin;
    outer_cutoff = cutoffout;

    icDist = length(V*M*vertex - vec4(0,0,0,1)); // vertex => przestrzen oka

    downVec = V * vec4(0,-1,0, 0); // wektor w dol
    for (int i = 0; i < 4; i++) {
		coneLights_l[i] = normalize(V*coneLightpositions[i] - V*M*vertex); // wektor do swiatla
        coneLights_cols[i] = coneLightcolors[i] * color_mult;
	  }


    // swiatlo
    vec4 lp = vec4(0, 0, 0, 1);
    l = normalize(lp - V*M*vertex);
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex);
    n = normalize(V * M * normal);
    
    iTexCoord0 = texCoord0;
    ic = color;
    
    gl_Position=P*V*M*vertex;
}
