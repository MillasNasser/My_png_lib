#include <stdlib.h>
#include <stdio.h>
#include "funcoes.h"

void graysale(RGBA* vector, int size){
    int i;
    for(i = 0; i < size; i++){
		float luminosidade = vector[i].r + vector[i].g + vector[i].b;
		vector[i].r = (int) luminosidade/3.0; 
		vector[i].g = (int) luminosidade/3.0; 
		vector[i].b = (int) luminosidade/3.0; 
	}
}

int main(int argc, char *argv[]){
	PNG_DATA *imagem = read_png_file("image.png");
	imagem->pixels = translate_px_to_vec(imagem);
	graysale(imagem->pixels, imagem->size);
	translate_vec_to_px(imagem->pixels, imagem);
    write_png_file("saida.png", imagem);

    return 0;
}
