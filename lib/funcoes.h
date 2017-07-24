#ifndef _PNG_DATA_
#define _PNG_DATA_

#include <png.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int r;
	int g;
	int b;
	int a;
} RGBA;

typedef struct{
	int size;
	int width, height;
	png_byte color_type;
	png_byte bit_depth;
	png_bytep *row_pointers;
	RGBA* pixels;
}PNG_DATA;
#endif /*_PNG_DATA_*/

#ifndef _PNG_FUNC_
    ///Auxiliares
    void copy_pixels(RGBA *position, png_bytep px2); //Copia pixels para uma posição do vetor
    void copy_positions(RGBA position, png_bytep px2); //Copia uma posição do vetor para pixels
    
    ///Realiza a leitura de um arquivo png e devolve uma struct com os dados
    PNG_DATA* read_png_file(char *filename);

    ///Realiza a escrita de um arquivo png a partir de uma struct
    void write_png_file(char *filename, PNG_DATA* dados);

    ///Realiza a tradução dos pixels para o vetor de struct RGBA
    RGBA* translate_px_to_vec(PNG_DATA* dados);

    ///Realiza a tradução das posições do vetor de struct RGBA para os pixels da imagem
    void translate_vec_to_px(RGBA* input, PNG_DATA* dados);

    ///Um pequeno teste de uso das funções
    int teste(int argc,char **argv);
#endif /*_PNG_FUNC_*/