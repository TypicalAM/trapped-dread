#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform float divisor;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 color;

out vec4 inter_color;

void main(void) {
    float d = distance(V*M*vertex, vec4(0,0,0,1)); // vertex => przestrzen oka
    d = (d - 3.3)/divisor;
    inter_color = color*(1-d);
    gl_Position=P*V*M*vertex;
}
