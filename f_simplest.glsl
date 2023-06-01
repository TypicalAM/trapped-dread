#version 330


out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela
in vec4 inter_color;

void main(void) {
	pixelColor = inter_color;
}

