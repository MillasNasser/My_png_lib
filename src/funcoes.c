#include "funcoes.h"

PNG_DATA* read_png_file(char *filename);

void write_png_file(char *filename, PNG_DATA* dados);

void copy_pixels(RGBA *position, png_bytep px2){
	position->r = px2[0];
	position->g = px2[1];
	position->b = px2[2];
	position->a = px2[3];
}

void copy_positions(RGBA position, png_bytep px2){
	px2[0] = position.r;
	px2[1] = position.g;
	px2[2] = position.b;
	px2[3] = position.a;
}

RGBA* translate_px_to_vec(PNG_DATA* dados) {
    int x,y;
	int height = dados->height;
	int width = dados->width;
	RGBA *pixels = calloc(sizeof(RGBA), dados->size);
	for (y = 0; y < height; y++) {
		png_bytep row = dados->row_pointers[y];
		for (x = 0; x < width; x++) {
			png_bytep px = &(row[x * 4]);
			copy_pixels(&(pixels[y*width+x]),px);
		}
	}
	return pixels;
}

void translate_vec_to_px(RGBA* input, PNG_DATA* dados){
    int x,y;
	int height = dados->height;
	int width = dados->width;
	for (y = 0; y < height; y++) {
		png_bytep row = dados->row_pointers[y];
		for (x = 0; x < width; x++) {
			png_bytep px = &(row[x * 4]);
			copy_positions(input[y*width+x],px);
		}
	}
}

/*** Leitura e escrita no png ***/
PNG_DATA* read_png_file(char *filename) {
    int y;
	PNG_DATA *new = malloc(sizeof(PNG_DATA));
	FILE *fp = fopen(filename, "rb");

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info = png_create_info_struct(png);
	if (!png || !info)
		abort();

	if (setjmp(png_jmpbuf(png)))
		abort();

	png_init_io(png, fp);

	png_read_info(png, info);

	new->width = png_get_image_width(png, info);
	new->height = png_get_image_height(png, info);
	new->size = new->width*new->height;
	new->color_type = png_get_color_type(png, info);
	new->bit_depth = png_get_bit_depth(png, info);

	// Read any color_type into 8bit depth, RGBA format.
	// See http://www.libpng.org/pub/png/libpng-manual.txt

	if (new->bit_depth == 16)
		png_set_strip_16(png);

	if (new->color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png);

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if (new->color_type == PNG_COLOR_TYPE_GRAY && new->bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png);

	if (png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png);

	// These color_type don't have an alpha channel then fill it with 0xff.
	if (new->color_type == PNG_COLOR_TYPE_RGB || new->color_type == PNG_COLOR_TYPE_GRAY ||
			new->color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

	if (new->color_type == PNG_COLOR_TYPE_GRAY ||
			new->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png);

	png_read_update_info(png, info);

	new->row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * new->height);
	for (y = 0; y < new->height; y++) {
		new->row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png, info));
	}

	png_read_image(png, new->row_pointers);
	fclose(fp);
	return new;
}

void write_png_file(char *filename, PNG_DATA* dados) {
	int y;
	int width = dados->width;
	int height = dados->height;

	FILE *fp = fopen(filename, "wb");
	if (!fp)
		abort();

	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info = png_create_info_struct(png);
	if (!png || !info)
		abort();

	if (setjmp(png_jmpbuf(png)))
		abort();

	png_init_io(png, fp);

	png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGBA,
							 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
							 PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png, info);

	png_write_image(png, dados->row_pointers);
	png_write_end(png, NULL);

	for (y = 0; y < height; y++) {
		free(dados->row_pointers[y]);
	}
	free(dados->row_pointers);

	fclose(fp);
}

int teste(int argc, char *argv[]) {
	if (argc != 3)
		abort();

    int i;
	PNG_DATA* data = read_png_file(argv[1]);
	data->pixels = translate_px_to_vec(data);
	for(i = 0; i < data->size; i++){
		float luminosidade = data->pixels[i].r + data->pixels[i].g + data->pixels[i].b;
		data->pixels[i].r = (int) luminosidade/3.0; 
		data->pixels[i].g = (int) luminosidade/3.0; 
		data->pixels[i].b = (int) luminosidade/3.0; 
	}
	
	translate_vec_to_px(data->pixels, data);
	write_png_file(argv[2],data);

	return 0;
}