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

void main(void) {
    vec4 lp = vec4(0, 0, 0, 1);
    l = normalize(lp - V*M*vertex);
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex);
    n = normalize(V * M * normal);
    
    iTexCoord0 = texCoord0;
    ic = color;
    
    gl_Position=P*V*M*vertex;
}
/*
void x(void) {
    float d = distance(V*M*vertex, vec4(0,0,0,1)); // vertex => przestrzen oka
    //d = (d - 3.3)/divisor;
    //inter_color_prawie = color*(1-d);
    //gl_Position_prawie=P*V*M*vertex;
}
*/